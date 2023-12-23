//
//  TODO;
//    proper suspend/wakeup detection
//    string descriptors?
//
#include "cortex_m4.h"
#include "usb.h"
#include "stm32g4usb.h"

// for debug print statements
#include "printf.h"
extern size_t u2puts(const char *buf, size_t len);	// in main.c

static enum usb_state_t _usb_state		 = USB_UNATTACHED;
//static enum usb_state_t _usb_state_saved = USB_UNATTACHED;	// when state is suspended, the state to return to on wakeup

enum usb_state_t usb_state() { return _usb_state; }

static const char *const _sstr[] = {"UNATTACHED", "DEFAULT", "ADDRESS", "CONFIGURED", "SUSPENDED", "UNDEFINED"};

const char *usb_state_str(enum usb_state_t s) {
	size_t ss = s;
	if (ss >= sizeof _sstr) {
		ss = sizeof _sstr - 1;
	}
	return _sstr[ss];
}

void usb_init() {

	_usb_state = USB_UNATTACHED;

	USB.CNTR   = USB_CNTR_FRES;	 // hold in reset, clear power down
	USB.ISTR   = 0;				 // clear pending interrupts
	USB.DADDR  = 0;				 // Device address is zero
	USB.BTABLE = 0;				 // btable at start of packet mem
	USB.LPMCSR = 0;				 // no low-power support
	USB.BCDR   = 0;				 // no battery charging detector support

	for (int i = 0; i < 8; ++i) {
		usb_ep_reset(i);
	}

	// 1024 bytes in USB_PMA
	// btable: 8 * 4 * 2 = 64 bytes
	USB_PMA.btable[0].ADDR_TX = 1 * 64;	 // control endpoint 0x00/0x80
	USB_PMA.btable[0].ADDR_RX = 2 * 64;
	usb_ep_set_rx_size(0, 64);

	USB_PMA.btable[1].ADDR_TX = 3 * 64;	 // bulk endpoint 0x01/0x81
	USB_PMA.btable[1].ADDR_RX = 4 * 64;
	usb_ep_set_rx_size(1, 64);

	// bring out of reset and enable interrupts
	USB.CNTR = USB_CNTR_CTRM | USB_CNTR_RESETM; // | USB_CNTR_WKUPM | USB_CNTR_SUSPM;

	USB.BCDR |= USB_BCDR_DPPU;	// connect DP pullup
}

void usb_shutdown() {
	USB.CNTR = USB_CNTR_FRES;
	USB.ISTR = 0;
	USB.BCDR = 0;  // disconnect
	USB.CNTR = USB_CNTR_FRES | USB_CNTR_PDWN;

	_usb_state = USB_UNATTACHED;
}

// copy buf[:min(len,sz)] from the rx packet buffer of endpoint ep
static size_t read_buffer(uint8_t ep, uint8_t *buf, size_t sz) {
	uint16_t len = usb_ep_get_rx_count(ep);
	if (len > sz) {
		len = sz;
	}

	const volatile uint8_t *src = usb_ep_rx_buf(ep);
	for (size_t i = 0; i < len; ++i) {
		buf[i] = src[i];
	}

	return len;
}

// copy buf[:sz] to the tx packet buffer of endpoint ep
static size_t write_buffer(uint8_t ep, const uint8_t *buf, size_t len) {
	if (len > 64) {
		len = 64;
	}

	volatile uint8_t *dst = usb_ep_tx_buf(ep);
	for (size_t i = 0; i < len; ++i) {
		dst[i] = buf[i];
	}

	usb_ep_set_tx_count(ep, len);
	return len;
}

static void handle_ep0(void);  // below

size_t usb_recv(uint8_t *buf, size_t sz) {

	uint16_t istr = USB.ISTR;

	USB.ISTR &= ~(USB_ISTR_SOF | USB_ISTR_ESOF | USB_ISTR_ERR | USB_ISTR_PMAOVR | USB_ISTR_L1REQ);

	// the other ISTR bits are r or rc_w0, meaning to clear them write ~bit to the register, avoid read-mod-write.

	if (istr & USB_ISTR_RESET) {

		cbprintf(u2puts, " reset\n");

		USB.ISTR &= ~(USB_ISTR_RESET | USB_ISTR_WKUP | USB_ISTR_SUSP);
		USB.CNTR &= ~(USB_CNTR_RESUME | USB_CNTR_FSUSP | USB_CNTR_LPMODE | USB_CNTR_PDWN | USB_CNTR_FRES);

		usb_ep_config(0, USB_EP_TYPE_CONTROL, 0);
		usb_ep_set_stat_tx(0, USB_EP_STAT_STALL);  // only setup will succeed
		usb_ep_set_stat_rx(0, USB_EP_STAT_STALL);

		for (int i = 1; i < 8; ++i) {
			usb_ep_reset(i);
		}

		usb_daddr_set_add(0);
		USB.DADDR |= USB_DADDR_EF;

		_usb_state = USB_DEFAULT;
		return 0;
	}

	if ((istr & USB_ISTR_CTR) != 0) {
		uint8_t ep = usb_istr_get_ep_id();

		cbprintf(u2puts, " ctr %d\n", ep);

		if (ep == 0) {
			handle_ep0();
			return 0;
		}

		// assert (ep == 1)
		if (USB.EPR[ep] & USB_EPRx_CTR_TX) {
			// IN: last transmit succeeded.
			// assert(usb_ep_get_stat_tx() == USB_EP_STAT_NAK)
			// hardware will have toggled DTOG
			usb_ep_clr_ctr_tx(ep);
		}

		if (USB.EPR[ep] & USB_EPRx_CTR_RX) {
			// OUT transaction
			// assert(usb_ep_get_stat_rx() == USB_EP_STAT_NAK)
			usb_ep_clr_ctr_rx(ep);

			size_t len = read_buffer(ep, buf, sz);
			usb_ep_set_stat_rx(ep, USB_EP_STAT_VALID);
			return len;
		}
	}

	return 0;
}

size_t usb_send(const uint8_t *buf, size_t len) {
	// still not done with the previous one, or perhaps unconfigured
	if (usb_ep_get_stat_tx(1) != USB_EP_STAT_NAK) {
		return 0;
	}

	len = write_buffer(1, buf, len);
	usb_ep_set_stat_tx(1, USB_EP_STAT_VALID);

	return len;
}

// Setup and standard request handling

static const uint8_t _deviceDescriptor[] = {
		18,				// length of this descriptor
		0x01,			// DEVICE Descriptor Type
		0x00, 0x02,		// USB version 2.00
		0,				// Device Class per interface
		0,	  0,		// subclass, protocol 0,0
		64,				//  Max Packet Size ep0
		0x83, 0x04,		// VendorID  = 0x0483 (STMicroelectronics)
		0x22, 0x57,		// ProductID = 0x5722 (Bulk demo)
		0x00, 0x02,		// Device Version 2.0
		0,	  0,	0,	// Manufacturer/Product/SerialNumber strings not set
		1,				// NumConfigurations
};

static const uint8_t _configDescriptor[] = {
		// Config 0 header
		9,				   //  Length
		0x02,			   //  CONFIGURATION Descriptor Type
		9 + 9 + 7 + 7, 0,  //  TotalLength
		1,				   //  NumInterfaces
		1,				   //  ConfigurationValue
		0,				   //  Configuration string not set
		0x80,			   //  Attributes 0x80 for historical reasons
		50,				   //  MaxPower 100mA

		// interface 0
		9,	   // Length
		0x04,  // INTERFACE Descriptor Type
		0, 0,  // Interface Number, Alternate Setting
		2,	   // Num Endpoints
		0x0A,  // InterfaceClass: USB_CLASS_DATA
		0,	   // InterfaceSubClass
		0,	   // InterfaceProtocol
		0,	   // Interface string not set

		// endpoint 0x1
		7,		//  Length
		0x05,	//  ENDPOINT Descriptor Type
		0x01,	//  Endpoint Address: 1-OUT
		0x02,	//  Attributes: BULK
		64, 0,	//  MaxPacketSize
		0,		//  Interval, ignored for BULK

		// endpoint 0x81
		7,		//  Length
		0x05,	//  ENDPOINT Descriptor Type
		0x81,	//  Endpoint Address 1-IN
		0x02,	//  Attributes: BULK
		64, 0,	//  MaxPacketSize
		0,		//  Interval, ignored for BULK
};

enum {
	REQ_TYPE_TX = 1 << 7,  // bit 7 direction: 1: device->host

   	//	REQ_TYPE_VENDOR 		= 1<<6, // bits 6..5 : type
   	//	REQ_TYPE_CLASS 			= 1<<5, //  00 = standard, 11 is reserved

	//	REQ_TYPE_DEVICE 		=  0x00,
	REQ_TYPE_INTERFACE = 0x01,
	REQ_TYPE_ENDPOINT  = 0x02,
	REQ_TYPE_OTHER	   = 0x03,

	// USB Standard Request Codes - Table 9-4
	REQ_GET_STATUS	= (0 << 8) | REQ_TYPE_TX,  // return 1: self-powered, 2: remote wakeup
	REQ_CLR_FEATURE = (1 << 8),				   // 1 remote wakeup-enable, 2: test mode (high-speed only)
	REQ_SET_FEATURE = (3 << 8),
	REQ_SET_ADDRESS = (5 << 8),	 // device only
	REQ_GET_DESCRIPTOR = (6 << 8) | REQ_TYPE_TX,	 // device only

	 //	REQ_SET_DESCRIPTOR 		= (7<<8),                // device only       // we don't support
	REQ_GET_CONFIGURATION = (8 << 8) | REQ_TYPE_TX,	 // device only       // return state == USB_CONFIGURED ? 1 : 0
	REQ_SET_CONFIGURATION = (9 << 8),  // device only       // 0-> state to ADDRESS,  1 -> state to CONFIGURED (and configure)
	REQ_GET_INTERFACE	  = (10 << 8) | REQ_TYPE_TX | REQ_TYPE_INTERFACE,  // interface only
	REQ_SET_INTERFACE	  = (11 << 8) | REQ_TYPE_INTERFACE,				   // interface only
	//	REQ_SYNC_FRAME 			= (12<<8)               | REQ_TYPE_ENDPOINT,  // endpoint, synch mode only, not supported

	REQ_GET_STATUS_INTERFACE  = REQ_GET_STATUS | REQ_TYPE_INTERFACE,   // return 0x0000
	REQ_CLR_FEATURE_INTERFACE = REQ_CLR_FEATURE | REQ_TYPE_INTERFACE,  // noop
	REQ_SET_FEATURE_INTERFACE = REQ_SET_FEATURE | REQ_TYPE_INTERFACE,  // noop

	REQ_GET_STATUS_ENDPOINT	 = REQ_GET_STATUS | REQ_TYPE_ENDPOINT,	 // return 0x1 if feature 'HALT' is set
	REQ_CLR_FEATURE_ENDPOINT = REQ_CLR_FEATURE | REQ_TYPE_ENDPOINT,	 // clear HALT (wvalue = 0), windex = 0x008f (dir/epnr)
	REQ_SET_FEATURE_ENDPOINT = REQ_SET_FEATURE | REQ_TYPE_ENDPOINT,	 // set HALT
};

// Cf. USB2.0 sections 5.5.5 and 8.5.3 there are 3 possible control flows:
// req.len > 0 &&  (req.req & REQ_TYPE_TX)  rx:SETUP (-> tx:DATA-IN-PART)*  -> tx:DATA-IN  -> rx:STATUS-OUT
// req.len > 0 && !(req.req & REQ_TYPE_TX)  rx:SETUP (-> rx:DATA-OUT-PART)* -> rx:DATA-OUT -> tx:STATUS-IN (zero lenght packet)
// (req.len == 0)                           rx:SETUP  -> tx:STATUS-IN  (zero lenght packet)
// Cf. USB2.0 section 9.4,  the second one (DATA-OUT/STATUS-IN) we don't need to support because it is only
// required by SET_DESCRIPTOR(device), which we don't support.
// Another simplification follows from the fact that the responses for the first flow are 1, 2 or
// len(descriptor) bytes, which in our case is always shorter than 64 (the pipe packet size),
// so we always can send the response in 1 go.
// Therefore, we have 3 events: rx:SETUP, tx:DATA-IN, rx:STATUS-OUT

// The most recently received SETUP request
struct {
	uint16_t req;  // lower byte: Type, upper byte request code
	uint16_t val;
	uint16_t idx;
	uint16_t len;
} _ctrl_req = {0, 0, 0, 0};

// false on failure, true on success
static int handle_set_request() {
	switch (_ctrl_req.req) {
	case REQ_SET_ADDRESS:
		// actually setting the address is handled after the ACK has been sent
		// here we just check for validity
		return (_usb_state != USB_CONFIGURED) && (_ctrl_req.val < 128) && (_ctrl_req.idx == 0);

	case REQ_SET_CONFIGURATION:
		switch (_usb_state) {
		case USB_ADDRESS:
			switch (_ctrl_req.val) {
			case 1:
				// configure our endpoint 0x01/0x81
				usb_ep_config(1, USB_EP_TYPE_BULK, 0x01);
				usb_ep_set_stat_rx(1, USB_EP_STAT_VALID);
				usb_ep_set_stat_tx(1, USB_EP_STAT_NAK);
				_usb_state = USB_CONFIGURED;
				// fallthrough
			case 0:
				return 1;
			}
			return 0;

		case USB_CONFIGURED:
			switch (_ctrl_req.val) {
			case 0:
				// unconfigure our endpoints 0x01/0x81
				usb_ep_reset(1);
				_usb_state = USB_ADDRESS;
				// fallthrough
			case 1:
				return 1;
			}
		default:
			break;
		}
		return 0;

	case REQ_SET_INTERFACE:
		return (_usb_state == USB_CONFIGURED) && (_ctrl_req.idx == 0);
		// case REQ_CLR_FEATURE:
		// case REQ_SET_FEATURE:
		// case REQ_CLR_FEATURE_INTERFACE:
		// case REQ_SET_FEATURE_INTERFACE:
		//    return 0; // no features implemented at device or interface level

	case REQ_CLR_FEATURE_ENDPOINT:
		if ((_usb_state != USB_CONFIGURED) || ((_ctrl_req.idx & 0xf) != 1)) {
			return 0;
		}
		if (_ctrl_req.idx & 0x80) {
			usb_ep_set_stat_tx(1, USB_EP_STAT_NAK);
			usb_ep_clr_dtog_tx(1);
		} else {
			usb_ep_set_stat_rx(1, USB_EP_STAT_NAK);
			usb_ep_clr_dtog_rx(1);
		}
		return 1;

	case REQ_SET_FEATURE_ENDPOINT:
		if ((_usb_state != USB_CONFIGURED) || ((_ctrl_req.idx & 0xf) != 1)) {
			return 0;
		}
		if (_ctrl_req.idx & 0x80) {
			usb_ep_set_stat_tx(1, USB_EP_STAT_STALL);
		} else {
			usb_ep_set_stat_rx(1, USB_EP_STAT_STALL);
		}
		return 1;
	}

	return 0;
}

// false on failure, true on success
static int handle_get_request() {
	uint8_t data[2] = {0, 0};
	size_t	len		= 0;

	switch (_ctrl_req.req) {
	case REQ_GET_DESCRIPTOR:
		if (_ctrl_req.idx != 0) {
			return 0;
		}
		switch (_ctrl_req.val) {
		case 0x0100:
			len = write_buffer(0, _deviceDescriptor, sizeof _deviceDescriptor);
			break;
		case 0x0200:
			len = write_buffer(0, _configDescriptor, sizeof _configDescriptor);
			break;
		default:
			return 0;
		}
		if (len > _ctrl_req.len) {
			usb_ep_set_tx_count(0, _ctrl_req.len);
		}
		return 1;

	case REQ_GET_STATUS:
		if ((_usb_state == USB_DEFAULT) || (_ctrl_req.val != 0) || (_ctrl_req.idx != 0) || (_ctrl_req.len != 2)) {
			return 0;
		}
		break;

	case REQ_GET_STATUS_INTERFACE:
		if ((_usb_state == USB_DEFAULT) || (_ctrl_req.val != 0) || (_ctrl_req.idx != 0) || (_ctrl_req.len != 2)) {
			return 0;
		}
		break;

	case REQ_GET_STATUS_ENDPOINT:
		if ((_usb_state == USB_DEFAULT) || (_ctrl_req.val != 0) || (_ctrl_req.len != 2)) {
			return 0;
		}
		switch (_ctrl_req.idx) {
		case 0x00:
			data[0] = (usb_ep_get_stat_rx(0) == USB_EP_STAT_STALL) ? 1 : 0;
			break;
		case 0x80:
			data[0] = (usb_ep_get_stat_tx(0) == USB_EP_STAT_STALL) ? 1 : 0;
			break;
		case 0x01:
			data[0] = (usb_ep_get_stat_rx(1) == USB_EP_STAT_STALL) ? 1 : 0;
			break;
		case 0x81:
			data[0] = (usb_ep_get_stat_tx(1) == USB_EP_STAT_STALL) ? 1 : 0;
			break;
		default:
			return 0;
		}
		break;

	case REQ_GET_CONFIGURATION:
		if ((_usb_state == USB_DEFAULT) || (_ctrl_req.len != 1)) {
			return 0;
		}
		data[0] = (_usb_state == USB_CONFIGURED) ? 1 : 0;
		break;

	case REQ_GET_INTERFACE:
		if ((_usb_state != USB_CONFIGURED) || (_ctrl_req.len != 1)) {
			return 0;
		}
		break;
	}
				cbprintf(u2puts, "get req len %d\n", _ctrl_req.len);

	return write_buffer(0, data, _ctrl_req.len) == _ctrl_req.len;
}

static void handle_ep0(void) {
	switch (USB.EPR[0] & (USB_EPRx_CTR_RX | USB_EPRx_SETUP | USB_EPRx_CTR_TX)) {
	case USB_EPRx_CTR_RX | USB_EPRx_SETUP:

		// assert(usb_ep_get_stat_tx() == USB_EP_STAT_NAK)
		// assert(usb_ep_get_stat_rx() == USB_EP_STAT_NAK)
		// assert dir == 1

		if (usb_ep_get_rx_count(0) != 8) {
			cbprintf(u2puts, " invalid setup packet %d\n", usb_ep_get_rx_count(0));
			break;
		}

		// this relies on the platform being little-endian
		read_buffer(0, (uint8_t*)&_ctrl_req, sizeof _ctrl_req);
		usb_ep_clr_ctr_rx(0);

		cbprintf(u2puts, "setup %x %x %x %x\n", _ctrl_req.req, _ctrl_req.val, _ctrl_req.idx, _ctrl_req.len);



		// if non-zero length request and direction is OUT
		// there's no request we can handle so bail out straightaway
		if ((_ctrl_req.len > 0) && !(_ctrl_req.req & REQ_TYPE_TX)) {
			break;
		}

		if (_ctrl_req.len == 0) {
			if (!handle_set_request()) {
				break;
			}
			cbprintf(u2puts, "handled set req\n");

			usb_ep_set_tx_count(0, 0);	// ZLP status-in reply
		} else {
			if (!handle_get_request()) {  // sets up reply buffer
				break;
			}
			cbprintf(u2puts, "handled get req\n");

		}

		usb_ep_set_stat_tx(0, USB_EP_STAT_VALID);
		return;

	case USB_EPRx_CTR_TX:  // USB IN: sent the reply for the most recent GET or the ACK for the most recent SET

		usb_ep_clr_ctr_tx(0);

		if (_ctrl_req.len == 0) {
			// last request was a SET, so we are here because we sent the ACK
			// if the request was set_address, we should execute it here
			if (_ctrl_req.req == REQ_SET_ADDRESS) {
				usb_daddr_set_add(_ctrl_req.val);
				USB.DADDR |= USB_DADDR_EF;
				_usb_state = (_ctrl_req.val == 0) ? USB_DEFAULT : USB_ADDRESS;
			}
		} else {
			// last request was a GET, so we are here because we sent the reply
			// next thing should be the RX of the host's STATUS_OUT
			// the hardware has a special mechanism to deal with this when we set the 'kind' bit
			usb_ep_set_stat_rx(0, USB_EP_STAT_VALID);
		}

		return;

	case USB_EPRx_CTR_RX:  // RX, USB OUT the final status zero lenght reply from the host to the GET

		if (usb_ep_get_rx_count(0) != 0) {
			break;
		}

		usb_ep_clr_ctr_rx(0);  // should be done automatically by STATUS_OUT mechanism, but apparently not

		return;
	}

	// if we got here we didn't handle the transfer, abort it
	usb_ep_set_stat_rx(0, USB_EP_STAT_STALL);
	usb_ep_set_stat_tx(0, USB_EP_STAT_STALL);

	return;
}

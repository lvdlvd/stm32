#include "printf.h"

size_t ringbuffer_puts(struct Ringbuffer *rb, const char *buf, size_t len) {
	const size_t l = ringbuffer_free(rb);
	if (l < len) {
		len = l;
	}
	for (size_t i = 0; i < len; ++i) {
		ringbuffer_put_head(rb, buf[i]);
	}
	return len;
}

#define STB_SPRINTF_STATIC
#define STB_SPRINTF_MIN 32
#define STB_SPRINTF_NOFLOAT
#define STB_SPRINTF_IMPLEMENTATION
#define STB_SPRINTF_NOUNALIGNED	 // Required on PIC32 or printing will cause Adress Exception on load

#include "stb_sprintf.h"

// a little signature adapter
static char *rb_putcb(char *buf, void *user, int len) {
	puts_t *callback = (puts_t *)user;
	size_t	ln		 = len;	 // explicit cast
	if (callback(buf, len) < ln) {
		return NULL;
	}
	return buf;
}

int cbprintf(puts_t *callback, const char *fmt, ...) {
	va_list ap;
	va_start(ap, fmt);
	char b[STB_SPRINTF_MIN];
	stbsp_set_separators('\'', '.');
	int rv = stbsp_vsprintfcb(rb_putcb, callback, b, fmt, ap);
	va_end(ap);
	return rv;
}

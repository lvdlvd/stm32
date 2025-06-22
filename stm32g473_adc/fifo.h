#pragma once

#include <assert.h>
#include <stddef.h>	 // for size_t
#include <stdint.h>	 // for uintX_t
#include <string.h>	 // for memmove

// A fifo is a circular byte buffer of a fixed size.
// size must be power of two or '% sizeof(fifo->buf)' cant be optimized to '& (sizeof(...)-1)'
// the first two members, buf and szmsk must be assigned in an initializer like so:
//     uint8_t buf[1<<12];
//     struct Fifo fifo = { buf, sizeof buf -1, 0, 0 };
// Since buf and szmsk are const a Fifo has to be statically initialized.
//
// A fifo can safely connect readers and writers at different irq priority levels, but 
// multiple readers of the same fifo, or multiple writers to the same fifo should not
// pre-empt eachother. 
struct Fifo {
	uint8_t* const buf;	  
	const size_t szmsk;     // size - 1
	volatile size_t head;	// writes happen here
	volatile size_t tail;	// reads happen here
};

// generate an initializer list given a byte array of fixed size
#define FIFO_INITIALIZER(buf) { buf, sizeof buf - 1, 0, 0 }

// invariant:
// as long as you only put_head() if !full(), and get_tail() if !empty(), tail <= head <= tail + sizeof buf.
// this is a define so the lineinfo makes it to the output.
#define  fifo_assert(fifo) assert(/*(0 <= (fifo->head - fifo->tail)) && */ (((fifo)->head - (fifo)->tail) <= ((fifo)->szmsk + 1)) && ((((fifo)->szmsk + 1) & (fifo)->szmsk) == 0))
 
// reset the fifo to the empty state
static inline void	  fifo_reset(struct Fifo *fifo) 		{ fifo->tail = fifo->head = 0; }

// returns how many bytes are available for fifo_get_tail(), 0...size
static inline size_t  fifo_avail(const struct Fifo *fifo) 	{ return fifo->head - fifo->tail; }

// returns how many bytes are available for fifo_put_head(),  size...0
static inline size_t  fifo_free(const struct Fifo *fifo) 	{ return fifo->tail + (fifo->szmsk + 1) - fifo->head; }

// returns true iff the fifo has no bytes for fifo_get_tail()
static inline int	  fifo_empty(const struct Fifo *fifo)	{ return fifo->head == fifo->tail; }

// returns true iff the fifo has no space for fifo_put_head()
static inline int	  fifo_full(const struct Fifo *fifo) 	{ return fifo->head == fifo->tail + (fifo->szmsk + 1); }

// retrieve the byte at offset idx  (0 <= idx < fifo_avail() ) from the tail.
static inline uint8_t fifo_at(const struct Fifo *fifo, size_t idx) { return fifo->buf[(fifo->tail + idx) & fifo->szmsk]; }

// put one character at the fifo head
static inline void fifo_put_head(struct Fifo *fifo, uint8_t c)  { 
	//  impose strict order (vs fifo->buf[fifo->head++ & fifo->szmsk])
	const size_t h = fifo->head;
	fifo->buf[h & fifo->szmsk] = c; 
	fifo->head = h + 1; 
}

// get one character from the fifo tail
static inline uint8_t fifo_get_tail(struct Fifo *fifo) {
	// impose strict order
	const size_t t = fifo->tail;
	uint8_t c = fifo->buf[t & fifo->szmsk];
	fifo->tail = t + 1; 
	return c;
}

// block access: pointer to the tail/head.  from here we can either address the entire contents in
// one access, or we have the wrap-around at the end of the buffer, in which case we need to access in two iterations
static inline uint8_t *fifo_tail(struct Fifo *fifo) { return fifo->buf + (fifo->tail & fifo->szmsk); }
static inline uint8_t *fifo_head(struct Fifo *fifo) { return fifo->buf + (fifo->head & fifo->szmsk); }

// as long as you pop_tail less than or equal to tail_size, and push_head less than or equal to head size, you maintain the invariant.
static inline void fifo_pop_tail( struct Fifo *fifo, size_t len) { fifo->tail += len; }
static inline void fifo_push_head(struct Fifo *fifo, size_t len) { fifo->head += len; }

// returns the largest contiguous chunk that can be popped at the tail
static inline size_t fifo_tail_size(const struct Fifo *fifo) {
	size_t h = fifo->head & fifo->szmsk;
	size_t t = fifo->tail & fifo->szmsk;

	if ((t < h) || fifo_empty(fifo)) {
		return h - t;
	}
	// if t == h then fifo full
	return (fifo->szmsk + 1) - t;
}

// returns the largest contiguous chunk that can be pushed at the head
static inline size_t fifo_head_size(const struct Fifo *fifo) {
	size_t h = fifo->head & fifo->szmsk;
	size_t t = fifo->tail & fifo->szmsk;
	if ((h < t) || fifo_full(fifo)) {
		return t - h;
	}
	// if t == h then fifo empty
	return (fifo->szmsk + 1) - h;
}

// write all or nothing to the head, in one or two memmoves
// returns the number of bytes written (0 or len)
static inline size_t fifo_write(struct Fifo *fifo, const void *buf, size_t len) {
	if (fifo_free(fifo) < len) {
		return 0;
	}
	size_t l = fifo_head_size(fifo);
	if (l > len) {
		l = len;
	}
	memmove(fifo_head(fifo), buf, l);
	fifo_push_head(fifo, l);
	if (l < len) {
		memmove(fifo_head(fifo), buf + l, len - l);
		fifo_push_head(fifo, len - l);
	}
	return len;
}

// read up to len from the tail, in one or two memmoves
// returns the number of bytes read (0..len)
// note that unlike write(), a read can succeed partially
static inline size_t fifo_read(struct Fifo *fifo, void *buf, size_t size) {
	size_t l = fifo_tail_size(fifo);
	if (l > size) {
		l = size;
	}
	memmove(buf, fifo_tail(fifo), l);
	fifo_pop_tail(fifo, l);
	buf += l;
	size -= l;
	size_t l2 = fifo_tail_size(fifo);
	if (l2 > size) {
		l2 = size;
	}
	if (l2 > 0) {
		memmove(buf, fifo_tail(fifo), l2);
		fifo_pop_tail(fifo, l2);
	}
	return l + l2;
}

// todo optimize
static inline size_t fifo_copy(struct Fifo *to, struct Fifo *from) {
	size_t n = 0;
	while(!fifo_empty(from) && !fifo_full(to)) {
		fifo_put_head(to, fifo_get_tail(from));
		++n;
	}
	return n;
}

// return the index wrt tail of the first occurrence of chr, or -1 if chr is not found.
static inline int fifo_chr(const struct Fifo* f, uint8_t chr) {
	size_t h = f->head;
	size_t t = f->tail; 
	for (size_t i = t; i != h; ++i) {
		if (f->buf[i & f->szmsk] == chr) {
			return i - t;
		}
	}
	return -1;
}
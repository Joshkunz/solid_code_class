#ifndef HUFF_ENCODE_H
#define HUFF_ENCODE_H

#include "common.h"

/* the 8k overhead of 256 32-byte buffers was deemed less of a penalty
 * than trying to deal with memory allocation issues. */
struct huff_enc_entry {
    int bitlen;
    uint8_t bits[256 / 8];
};

struct huff_encoder {
    struct huff_enc_entry table[256];
    uint8_t buffer;
    int buffer_used;
};

/* fill the given encoder with the information for the supplied
 * translation table. */
int huff_make_encoder(struct huff_encoder * encoder, char * ttable[246]);

/* Given a byte, encode it's bit representation into 'buffer', returns the
 * number of bytes the supplied byte was translated into. Note: Many bytes
 * will encode to a bitstring that isn't divisible by 8. If that's the case,
 * the extra bits are stored in 'buffer' and are appended to the beginning
 * of the next 'encode' call. If the buffer is non-zero, always remember to
 * write the buffer to the file before closing it. */
int huff_encode(uint8_t byte, uint8_t buffer[32], struct huff_encoder*);

#endif

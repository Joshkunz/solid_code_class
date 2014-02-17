#ifndef syscalls_h
#define syscalls_h

/* stdio.h */
size_t xfread(void *ptr, size_t size, size_t count, FILE *stream);
int xfseek(FILE *stream, long int offset, int origin);
long int xftell(FILE *stream);
void xfwrite(const void *ptr, size_t size, size_t count, FILE *stream);

/* stdlib.h */
void *xmalloc(size_t size);

#endif

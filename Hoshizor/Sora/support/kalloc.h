#ifndef _KALLOC_H_
#define _KALLOC_H_

#include <stdlib.h>

#ifdef _KSYS_ALLOC
#define kmalloc(x) malloc(x)
#define krealloc(p, x) realloc((p), (x))
#define kfree(p) free(p)
#define kr_stat()
#define kr_set_pow2(x)
#define kr_destroy()
#define kr_size(x) 0ul
#else
#define kr_size(x) ((((kheader_t*)(x))-1)->size * sizeof(kheader_t))

#ifdef __cplusplus
extern "C" {
#endif

	void *kmalloc(size_t);
	void *krealloc(void*, size_t);
	void kfree(void*);

	void kr_stat();
	void kr_set_pow2(int);
	void kr_destroy();

#ifdef __cplusplus
}
#endif

#endif

#endif

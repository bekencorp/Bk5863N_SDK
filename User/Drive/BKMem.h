#ifndef _BKMEM_H_
#define _BKMEM_H_

#include "Type.h"


void *BKmmgmt_malloc(uint32_t size);
void *BKmmgmt_realloc(void *ptr, size_t size);
uint32_t BKmmgmt_alloc_space(void);
void BKmmgmt_free(void *mem_ptr);

#endif

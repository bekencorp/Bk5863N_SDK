#include "BKmem.h"
#include "Includes.h"
/**************************************************
  动态内存分配函数，请根据自身使用的动态内存填入该
	参数
**************************************************/
/**************************************************
FunctionName: 
Describe: 
InputParameter: 
OutputParameter: 
Notice:
**************************************************/
void *BKmmgmt_malloc(uint32_t size)
{
	#if 1
	return mmgmt_malloc(size);
	#else
	return 0;
	#endif 
}
/**************************************************
FunctionName: 
Describe: 
InputParameter: 
OutputParameter: 
Notice:
**************************************************/
void *BKmmgmt_realloc(void *ptr, size_t size)
{
	#if 1
	return mmgmt_realloc(ptr,size);
	#else
	return 0;
	#endif 
}
/**************************************************
FunctionName: 
Describe: 
InputParameter: 
OutputParameter: 
Notice:
**************************************************/
uint32_t BKmmgmt_alloc_space(void)
{
	#if 1
	return mmgmt_alloc_space();
	#else
	return 0;
	#endif 
}
/**************************************************
FunctionName: 
Describe: 
InputParameter: 
OutputParameter: 
Notice:
**************************************************/
void BKmmgmt_free(void *mem_ptr)
{
	#if 1
	mmgmt_free(mem_ptr);
	#else
	return;
	#endif 
}













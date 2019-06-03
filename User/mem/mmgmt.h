/**
 ****************************************************************************************
 *
 * @file ke_mem.h
 *
 * @brief API for the heap management module.
 *
 * Copyright (C) RivieraWaves 2011-2016
 *
 * $Rev:  $
 *
 ****************************************************************************************
 */

#ifndef _MMGMT_H_
#define _MMGMT_H_
#include "type.h"
#include "gernel.h"
#ifndef ASSERT
#define ASSERT(exp)                                 \
{                                                   \
    if ( !(exp) )                                   \
    {                                               \
        while(1);                                   \
    }                                               \
} 
#endif

#define CO_ALIGN4_HI(val) (((val)+3)&~3)

extern void __enable_irq(void);
extern void __enable_fiq(void);
extern int  __disable_irq(void);
extern int  __disable_fiq(void);

#define GLOBAL_INT_START()         do{\
										__enable_fiq();\
										__enable_irq();\
									}while(0)
#define GLOBAL_INT_STOP()            do{\
										__disable_fiq();\
										__disable_irq();\
									}while(0)
#ifndef GLOBAL_INT_DECLARATION
#define GLOBAL_INT_DECLARATION()   uint32_t fiq_tmp, irq_tmp
#define GLOBAL_INT_DISABLE()       do{\
										fiq_tmp = __disable_fiq();\
										irq_tmp = __disable_irq();\
									}while(0)


#define GLOBAL_INT_RESTORE()       do{                         \
                                        if(!fiq_tmp)           \
                                        {                      \
                                            __enable_fiq();    \
                                        }                      \
                                        if(!irq_tmp)           \
                                        {                      \
                                            __enable_irq();    \
                                        }                      \
                                      }while(0)
#endif
/**
 ****************************************************************************************
 * @defgroup KE_MEM KE_MEM
 * @ingroup KERNEL
 * @brief Heap management module.
 *
 * The KEMEM module implements heap management functions that allow initializing heap,
 * allocating and freeing memory.
 *
 * @{
 ****************************************************************************************
 */
#define KE_HEAP_SIZE       ((4*512)+0)//      CFG_HEAP_SIZE

/// Free memory block delimiter structure (size must be word multiple)
struct mblock_free
{
    struct mblock_free *next;   ///< Pointer to the next block    
    uint32_t size;              ///< Size of the current free block (including delimiter)
};

/// Used memory block delimiter structure (size must be word multiple)
struct mblock_used
{
    uint32_t size;              ///< Size of the current used block (including delimiter)
};

// forward declarations
struct mblock_free;

extern void sys_mem_init(void);

/**
 ****************************************************************************************
 * @brief Heap initialization.
 *
 * This function performs the following operations:
 * - sanity checks
 * - check memory allocated is at least large enough to hold two block descriptors to hold
 * start and end
 * - initialize the first and last descriptors
 * - save the pointer to the first free descriptor
 *
 * @return The pointer to the first free block.
 *
 ****************************************************************************************
 */
struct mblock_free *mmgmt_init(void);

/**
 ****************************************************************************************
 * @brief Allocation of a block of memory.
 *
 * Allocates a memory block whose size is size; if no memory is available return
 * NULL
 *
 * @param[in] size Size of the memory area that need to be allocated.
 *
 * @return A pointer to the allocated memory area.
 *
 ****************************************************************************************
 */
void *mmgmt_malloc(uint32_t size);
void *mmgmt_realloc(void *ptr, size_t size);
uint32_t mmgmt_alloc_space(void);

/**
 ****************************************************************************************
 * @brief Freeing of a block of memory.
 *
 * Free the memory area pointed by mem_ptr : mark the block as free and insert it in
 * the pool of free block.
 *
 * @param[in] mem_ptr Pointer to the memory area that need to be freed.
 *
 ****************************************************************************************
 */
void mmgmt_free(void *mem_ptr);

///@} MEM

#endif // _MMGMT_H_


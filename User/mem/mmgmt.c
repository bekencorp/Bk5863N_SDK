/*
 * INCLUDE FILES
 ****************************************************************************************
 */
#include "Includes.h"
//#include "arm_arch.h"
//#include "mem_pub.h"
#include "mmgmt.h"
//#include "ll.h"

/*
 * GLOBAL VARIABLES
 ****************************************************************************************
 */
static uint8_t mmgmt_heap[KE_HEAP_SIZE];
static struct mblock_free *mblock_first;

/*
 * FUNCTION DEFINITIONS
 ****************************************************************************************
 */
void sys_mem_init(void)
{
    mblock_first = mmgmt_init();
}

struct mblock_free *mmgmt_init(void)
{
    struct mblock_free *first;
    
    GLOBAL_INT_DECLARATION();

    // align first free descriptor to word boundary
    first = (struct mblock_free*)CO_ALIGN4_HI((uint32_t)mmgmt_heap);

    // protect accesses to descriptors
    GLOBAL_INT_DISABLE();

    // initialize the first block
    //  + compute the size from the last aligned word before heap_end
    first->size = ((uint32_t)&mmgmt_heap[KE_HEAP_SIZE] & (~3)) - (uint32_t)first;
    first->next = NULL;

    // end of protection
    GLOBAL_INT_RESTORE();

    // save the pointer to the first free block
    return first;
}

uint32_t mmgmt_alloc_space(void)
{
    struct mblock_free *node;
    uint32_t valid_size = 0;

    node = mblock_first;

    while (node != NULL)
    {
        ASSERT(node->size);
        valid_size += node->size;
        
        node = node->next;
    }
    
    return valid_size;
}

void *mmgmt_malloc(uint32_t size)
{
    struct mblock_free *node, *found;
    struct mblock_used *alloc = 0;
    uint32_t totalsize;

    GLOBAL_INT_DECLARATION();

    if(0 == size)
    {
        goto alloc_exit;
    }
    
    // initialize the pointers
    found = NULL;

    // compute overall block size (including requested size PLUS descriptor size)
    totalsize = CO_ALIGN4_HI(size) + sizeof(struct mblock_used);

    // sanity check: the totalsize should be large enough to hold free block descriptor
    ASSERT(totalsize >= sizeof(struct mblock_free));

    node = mblock_first;

    // protect accesses to descriptors
    GLOBAL_INT_DISABLE();

    // go through free memory blocks list
    while (node != NULL)
    {
        // check if there is enough room in this free block
        ASSERT(node->size);
        if (node->size >= (totalsize + sizeof(struct mblock_free)))
        {
            // if a match was already found, check if this one is smaller
            if ((found == NULL) || (found->size > node->size))
            {
                found = node;
            }
        }
        // move to next block
        node = node->next;
    }

    if(0 == found)
    {
        GLOBAL_INT_RESTORE();
        goto alloc_exit;
    }
    
    // found a free block that matches, subtract the allocation size from the
    // free block size. If equal, the free block will be kept with 0 size... but
    // moving it out of the linked list is too much work.
    found->size -= totalsize;

    // compute the pointer to the beginning of the free space
    alloc = (struct mblock_used*) ((uint32_t)found + found->size);

    // sanity check: allocation should always succeed
    ASSERT(found != NULL);

    // save the size of the allocated block (use low bit to indicate mem type)
    alloc->size = totalsize;

    // move to the user memory space
    alloc++;

    // end of protection (as early as possible)
    GLOBAL_INT_RESTORE();

alloc_exit:
	ASSERT(0 == ((UINT32)alloc & 3));
    //ASSERT(alloc);
    return (void*)alloc;
}

void *mmgmt_realloc(void *ptr, size_t size)
{
    struct mblock_used *a;
	size_t copy_len;
	void *n;

	if (ptr == NULL)
		return mmgmt_malloc(size);
	
	n = mmgmt_malloc(size);
	if (n == NULL)
		return NULL;
	
    a = ((struct mblock_used *)ptr) - 1;
	copy_len = a->size;
	if (copy_len > size)
		copy_len = size;
	
	memcpy(n, a + 1, copy_len);
	
	mmgmt_free(ptr);
	
	return n;
}

void mmgmt_free(void* mem_ptr)
{
    struct mblock_used *freed;
    struct mblock_free *node, *prev_node, *next_node;
    uint32_t size;
    
    GLOBAL_INT_DECLARATION();

    // point to the block descriptor (before user memory so decrement)
    freed = ((struct mblock_used *)mem_ptr) - 1;

    // point to the first node of the free elements linked list
    size = freed->size;
    node = mblock_first;
    prev_node = NULL;

    // sanity checks
    ASSERT(size);
    ASSERT(mem_ptr != NULL);
    ASSERT((uint32_t)mem_ptr > (uint32_t)node);

    // protect accesses to descriptors
    GLOBAL_INT_DISABLE();

    while (node != NULL)
    {
        // check if the freed block is right after the current block
        if ((uint32_t)freed == ((uint32_t)node + node->size))
        {
            // append the freed block to the current one
            node->size += size;

            // check if this merge made the link between free blocks
            if ((uint32_t)node->next == ((uint32_t)node + node->size))
            {
                next_node = node->next;
                // add the size of the next node to the current node
                node->size += next_node->size;
                // update the next of the current node
                node->next = next_node->next;
            }
            goto free_end;
        }
        else if ((uint32_t)freed < (uint32_t)node)
        {
            // sanity check: can not happen before first node
            ASSERT(prev_node != NULL);

            // update the next pointer of the previous node
            prev_node->next = (struct mblock_free*)freed;

            // check if the released node is right before the free block
            if (((uint32_t)freed + size) == (uint32_t)node)
            {
                // merge the two nodes
                ((struct mblock_free*)freed)->next = node->next;
                ((struct mblock_free*)freed)->size = node->size + (uint32_t)node - (uint32_t)freed;
            }
            else
            {
                // insert the new node
                ((struct mblock_free*)freed)->next = node;
                ((struct mblock_free*)freed)->size = size;
            }
            goto free_end;
        }

        // move to the next free block node
        prev_node = node;
        node = node->next;
    }
	
    // if reached here, freed block is after last free block and not contiguous
    prev_node->next = (struct mblock_free*)freed;
    ((struct mblock_free*)freed)->next = NULL;
    ((struct mblock_free*)freed)->size = size;

free_end:
	
    // end of protection
    GLOBAL_INT_RESTORE();
}

/// @}

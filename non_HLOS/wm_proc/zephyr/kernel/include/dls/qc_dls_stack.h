#ifndef DLS_STACK_H_
#define DLS_STACK_H_

#ifdef CONFIG_QC_DLS_STACK

#include <zephyr/kernel/thread_stack.h>

#define __dlsstackmem Z_GENERIC_SECTION(.dls_noinit)

/**
 * @brief Define a toplevel thread stack memory region in DLS memory
 *
 * This defines a region of memory suitable for use as a thread's stack in DLS memory.
 *
 * This is the generic, historical definition. Align to Z_THREAD_STACK_OBJ_ALIGN
 * and put in 'noinit' section so that it isn't zeroed at boot
 *
 * The defined symbol will always be a k_thread_stack_t which can be passed to
 * k_thread_create(), but should otherwise not be manipulated. If the buffer
 * inside needs to be examined, examine thread->stack_info for the associated
 * thread object to obtain the boundaries.
 *
 * It is legal to precede this definition with the 'static' keyword.
 *
 * It is NOT legal to take the sizeof(sym) and pass that to the stackSize
 * parameter of k_thread_create(), it may not be the same as the
 * 'size' parameter. Use K_THREAD_STACK_SIZEOF() instead.
 *
 * Some arches may round the size of the usable stack region up to satisfy
 * alignment constraints. K_THREAD_STACK_SIZEOF() will return the aligned
 * size.
 *
 * @param sym Thread stack symbol name
 * @param size Size of the stack memory region
 */
#define K_DLS_THREAD_STACK_DEFINE(sym, size) \
	Z_THREAD_STACK_DEFINE_IN(sym, size, __dlsstackmem)


/**
 * @brief Define a toplevel array of thread stack memory regions in DLS memory
 *
 * Create an array of equally sized stacks. See K_DLS_THREAD_STACK_DEFINE
 * definition for additional details and constraints.
 *
 * This is the generic, historical definition. Align to Z_THREAD_STACK_OBJ_ALIGN
 * and put in 'noinit' section so that it isn't zeroed at boot
 *
 * @param sym Thread stack symbol name
 * @param nmemb Number of stacks to define
 * @param size Size of the stack memory region
 */
#define K_DLS_THREAD_STACK_ARRAY_DEFINE(sym, nmemb, size) \
	Z_THREAD_STACK_ARRAY_DEFINE_IN(sym, nmemb, size, __dlsstackmem)

#endif /* CONFIG_QC_DLS_STACK */
#endif /* DLS_STACK_H */

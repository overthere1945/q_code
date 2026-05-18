#ifndef QURT_SEM_H
#define QURT_SEM_H 

#include <zephyr/kernel.h>

#ifdef __cplusplus
extern "C" {
#endif

/*=============================================================================
												TYPEDEFS
=============================================================================*/
/** @addtogroup semaphore_types
@{ */

typedef struct k_sem qurt_sem_t;

/** @} */ /* end_addtogroup semaphore_types */
/*=============================================================================
												FUNCTIONS
=============================================================================*/

/**@ingroup func_qurt_sem_up  
  Releases access to a shared resource. When a thread performs an up operation on a semaphore,
  the semaphore count value is incremented. The result depends on the number of threads waiting 
  on the semaphore: \n
  - When no threads are waiting, the current thread releases access to the shared resource
     and continues executing.\n
  - When one or more threads are waiting and the semaphore count value is nonzero, then
     the kernel awakens the highest-priority waiting thread and decrements the
     semaphore count value. If the awakened thread has higher priority than the current
     thread, a context switch can occur.

   @datatypes
   #qurt_sem_t
   
   @param[in]  sem  Pointer to the semaphore object to access.

   @return 
   Unused integer value.

   @dependencies
   None.  
 */
int qurt_sem_up(qurt_sem_t *sem);

/**@ingroup func_qurt_sem_down  
  Requests access to a shared resource. When a thread performs a down operation on a 
  semaphore, the result depends on the semaphore count value: \n
  - When the count value is nonzero, it is decremented, and the thread gains access to the
     shared resource and continues executing.\n
  - When the count value is zero, it is not decremented, and the thread is suspended on the
     semaphore. When the count value becomes nonzero (because another thread
     released the semaphore) it is decremented, and the suspended thread is awakened
     and gains access to the shared resource.
  
   @datatypes
   #qurt_sem_t
   
   @param[in]  sem  Pointer to the semaphore object to access.

   @return 
   Unused integer value.

   @dependencies
   None.
 */
int qurt_sem_down(qurt_sem_t *sem);

/**@ingroup func_qurt_sem_try_down
  @xreflabel{hdr:qurt_sem_try_down}
  Requests access to a shared resource (without suspend). When a thread performs a try down
  operation on a semaphore, the result depends on the semaphore count value: \n
  - The count value is decremented when it is nonzero. The down operation returns 0 as
     the function result, and the thread gains access to the shared resource and is free to
     continue executing.\n
  - The count value is not decremented when it is zero. The down operation returns -1
     as the function result, and the thread does not gain access to the shared resource
     and should not continue executing.
 
   @datatypes
   #qurt_sem_t
   
   @param[in]  sem  Pointer to the semaphore object to access. 

   @return 
   0 -- Success. \n
   -1 -- Failure. 

   @dependencies
   None.
   
 */
int qurt_sem_try_down(qurt_sem_t *sem);

/**@ingroup func_qurt_sem_init
  Initializes a semaphore object.
  The default initial value of the semaphore count value is 1.

  @param[out]  sem  Pointer to the initialized semaphore object.

  @return 
  None.

  @dependencies
  None.
  
 */
void qurt_sem_init(qurt_sem_t *sem);

/**@ingroup func_qurt_sem_destroy
  Destroys the specified semaphore.\n
  @note1hang Semaphores must be destroyed when they are no longer in use. Failure to do
             this causes resource leaks in the QuRT kernel.\n
  @note1cont Semaphores must not be destroyed while they are still in use. If this happens,
             the behavior of QuRT is undefined.

  @datatypes
  #qurt_sem_t

  @param[in]  sem  Pointer to the semaphore object to destroy. 
 
  @return
  None.

  @dependencies
  None.
 */
void qurt_sem_destroy(qurt_sem_t *sem);

/**@ingroup func_qurt_sem_init_val
  Initializes a semaphore object with the specified value.

   @datatypes
   #qurt_sem_t
   
   @param[out]  sem  Pointer to the initialized semaphore object. 
   @param[in]  val   Initial value of the semaphore count value.

   @return
   None.

   @dependencies
   None.
  
 */
void qurt_sem_init_val(qurt_sem_t *sem, unsigned short val);

/**@ingroup func_qurt_sem_get_val
  Gets the semaphore count value.\n
  Returns the current count value of the specified semaphore.

  @datatypes
  #qurt_sem_t
  
  @param[in]   sem Pointer to the semaphore object to access.

  @return
  Integer semaphore count value

  @dependencies
  None.
 */
unsigned short qurt_sem_get_val(qurt_sem_t *sem );

/**@ingroup func_qurt_sem_down_timed  
  When a thread performs a down operation on a semaphore, the result depends on the
  semaphore count value: \n
  - When the count value is nonzero, it is decremented, and the thread gains access to the
     shared resource and continues executing.\n
  - When the count value is zero, it is not decremented, and the thread is suspended on the
     semaphore. When the count value becomes nonzero (because another thread
     released the semaphore) it is decremented, and the suspended thread is awakened
     and gains access to the shared resource. Terminate the wait when the specified timeout expires.
   If timeout expires, terminate this wait and grant no access to the shared resource.
  
   @datatypes
   #qurt_sem_t
   
   @param[in]  sem     Pointer to the semaphore object to access. 
   @param[in] duration Interval (in microseconds) duration value must be between #QURT_TIMER_MIN_DURATION and
                       #QURT_TIMER_MAX_DURATION 

   @return 
   #QURT_EOK -- Success \n
   #QURT_ETIMEDOUT -- Timeout

   @dependencies
   None.
 */
int qurt_sem_down_timed(qurt_sem_t *sem, unsigned long long int duration_in_us);

#ifdef __cplusplus
} /* closing brace for extern "C" */
#endif

#endif /* QURT_SEM_H */

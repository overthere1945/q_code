/*==============================================================================

qurt_sem.c

GENERAL DESCRIPTION
    QuRT Shim Layer functions for Semaphore APIs

EXTERNAL FUNCTIONS
    qurt_sem_init
    qurt_sem_init_val
    qurt_sem_up
    qurt_sem_down
    qurt_sem_try_down
    qurt_sem_destroy
    qurt_sem_get_val
    qurt_sem_down_timed

INITIALIZATION AND SEQUENCING REQUIREMENTS
    None.

Copyright (c) 2023 Qualcomm Technologies, Inc.
All Rights Reserved.
Confidential and Proprietary - Qualcomm Technologies, Inc.
==============================================================================*/

#include <assert.h>

#include <qurt_error.h>
#include <qurt_sclk.h>
#include <qurt_sem.h>

// Here UINT16_MAX is used because of limitation
//  with return type of qurt_sem_get_val
#define QURT_SEM_MAX_VAL (UINT16_MAX)

void qurt_sem_init(qurt_sem_t *sem)
{
    int ret = k_sem_init(sem, 1, QURT_SEM_MAX_VAL);
#ifdef CONFIG_ASSERT
    assert(ret == 0);
#else
    ARG_UNUSED(ret);
#endif
}

void qurt_sem_init_val(qurt_sem_t *sem, unsigned short val)
{
    int ret = k_sem_init(sem, val, QURT_SEM_MAX_VAL);
#ifdef CONFIG_ASSERT
    assert(ret == 0);
#else
    ARG_UNUSED(ret);
#endif
}

int qurt_sem_up(qurt_sem_t *sem)
{
    k_sem_give(sem);
    return 0;
}

int qurt_sem_down(qurt_sem_t *sem)
{
    return k_sem_take(sem, K_FOREVER);
}

int qurt_sem_try_down(qurt_sem_t *sem)
{
    int ret = k_sem_take(sem, K_NO_WAIT);
    if (ret == -EBUSY)
    {
        // setting same return value as QuRT
        ret = QURT_EFATAL;
    }

    return ret;
}

void qurt_sem_destroy(qurt_sem_t *sem)
{
    k_sem_reset(sem);
}

unsigned short qurt_sem_get_val(qurt_sem_t *sem)
{
    return k_sem_count_get(sem);
}

int qurt_sem_down_timed(qurt_sem_t *sem, unsigned long long int duration_in_us)
{
    if (QURT_TIMER_IS_DURATION_VALID(duration_in_us) != QURT_EOK)
    {
        return QURT_EINVALID;
    }

    int ret_val = k_sem_take(sem, K_USEC(duration_in_us));
    switch (ret_val)
    {
    case 0:
    {
        return QURT_EOK;
    }
    break;
    case -EAGAIN:
    case -ETIMEDOUT:
    {
        return QURT_ETIMEDOUT;
    }
    break;
    case -EBUSY:
    default:
    {
        return QURT_EFATAL;
    }
    break;
    }
}

/*
 * Copyright (c) 2017, Realtek Semiconductor Corporation. All rights reserved.
 */

#ifndef GAP_TIMER_H
#define GAP_TIMER_H

#include "platform_opts_bt.h"

#if UPPER_STACK_VERSION == VERSION_2021

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \defgroup    GAP_TIMER  GAP Timer
 *
 * \brief   Register timeout callback functions, start and stop software timers.
 * \details The GAP Timer Module allows to register timeout callback functions
 *          which will be called when timeout, also provides start/stop timer
 *          functions. GAP Timer Module provides a method for APP to handle timeout
 *          events in its own task.\n
 *
 */

/**
 * gap_timer.h
 *
 * \brief   Prototype of callback functions other modules registered to handle timeout events.
 * \details Timer ID and timer channel are the same as those used in \ref gap_start_timer.\n
 *
 * \ingroup  GAP_TIMER
 */
typedef void (* P_TIMEOUT_CB)(uint8_t timer_id, uint16_t timer_chann);

/**
 * gap_timer.h
 *
 * \brief   Init GAP Timer Module.
 *
 * \param[in]   evt_handle         Event queue handle which represents event queue created by APP task.
 * \param[in]   timer_q_num        Number of elements in GAP timer message queue which will be create by GAP timer Module.
 *
 * \return      None.
 *
 * <b>Example usage</b>
   \code{.c}
   #define EVENT_GAP_TIMER             0x0A

   #define MAX_NUMBER_OF_GAP_TIMER     0x10

   void *evt_q_handle;

   void app_task(void *p_param)
   {
       uint8_t event;

       while (true)
       {
           if (os_msg_recv(evt_q_handle, &event, 0xFFFFFFFF) == true)
           {
               if (event == EVENT_GAP_TIMER)
               {
                   gap_handle_msg(event);
               }
           }
   }

   // Init GAP Timer Module.
   int test(void)
   {
       os_msg_queue_create(&evt_q_handle, MAX_NUMBER_OF_GAP_TIMER, sizeof(uint8_t));

       gap_init_timer(evt_q_handle, MAX_NUMBER_OF_GAP_TIMER);

       // ...
       // Create app task.
       // ...

       os_sched_start();

       return 0;
   }
   \endcode
 *
 * \ingroup  GAP_TIMER
 */
bool gap_init_timer(void *evt_handle, uint8_t timer_q_num);

/**
 * gap_timer.h
 *
 * \brief   Register a timeout callback function in GAP Timer Module. A queue ID is
 *          returned which will be used when start timer by calling \ref gap_start_timer.
 *          When timeout event occurs, GAP Timer Module will find the exact
 *          timeout function by queue ID and execute it.
 *
 * \param[in]   callback         Timeout callback function. Callback functions
 *                               must have the prototype defined as \ref P_TIMEOUT_CB.
 *
 * \param[out]  p_queue_id       Used to pass back a queue ID that will be used when start timer.
 *
 * \return           The status of registering timeout callback function.
 * \retval true      Callback function was registered successfully.
 * \retval false     Callback function was failed to register.
 *
 * <b>Example usage</b>
   \code{.c}
   #define TIMER_ID          0x01
   #define TIMER_CHANN       0x1000

   uint8_t timer_queue_id;
   void   *timer_handle;

   // timer callback function.
   void timer_callback(uint8_t timer_id, uint16_t timer_chann)
   {
       if (timer_id == TIMER_ID && timer_chann == TIMER_CHANN)
       {
            // Delete the specified timer.
            gap_stop_timer(&timer_handle);
       }
   }

   // register timer callback function and start timer.
   int test(void)
   {
       bool ret;

       // ...
       // Init GAP Timer Module.
       // Create app task.
       // ...

       ret = gap_reg_timer_cb(timer_callback, &timer_queue_id);

       if (ret)
       {
           gap_start_timer(&timer_handle, "test_timer", timer_queue_id,
                           TIMER_ID, TIMER_CHANN, 2000);
       }

       return 0;
   }
   \endcode
 *
 * \ingroup  GAP_TIMER
 */
bool gap_reg_timer_cb(P_TIMEOUT_CB callback, uint8_t *p_queue_id);

/**
 * gap_timer.h
 *
 * \brief   Create a new GAP timer and start it. This allocates the storage required by
 *          the new timer, starts this timer if create successfully, and returns a handle
 *          by which the new timer can be referenced. <b>GAP timer is always a one-shot timer</b>
 *
 * \param[out]  pp_handle        Used to pass back a handle by which the created timer
 *                               can be referenced.
 *
 * \param[in]   p_name           A descriptive name for the timer.
 *
 * \param[in]   queue_id         Timer queue ID assigned by GAP Timer Module when register
 *                               timer callback function using \ref gap_reg_timer_cb.
 *                               Queue ID would be used to identify which timeout function will be execute.
 *
 * \param[in]   timer_id         Timer ID assigned by module which starts this timer
 *
 * \param[in]   timer_chann      Timer channel assigned by module which start this timer.
 *                               Typically timer ID and timer channel would be used in the
 *                               timer callback function to identify which timer expired when
 *                               the same callback function is assigned to more than one timer.
 *
 * \param[in]   timeout_ms       Timeout value in milliseconds.
 *
 * \return           The status of the timer creation and starting.
 * \retval true      Timer was created and started successfully.
 * \retval false     Timer was failed to create or start.
 *
 * <b>Example usage</b>
   \code{.c}
   #define TIMER_ID          0x01
   #define TIMER_CHANN       0x1000

   uint8_t timer_queue_id;
   void   *timer_handle;

   // timer callback function.
   void timer_callback(uint8_t timer_id, uint16_t timer_chann)
   {
       if (timer_id == TIMER_ID && timer_chann == TIMER_CHANN)
       {
            // Delete the specified timer.
            gap_stop_timer(&timer_handle);
       }
   }

   // register timer callback function and start timer.
   int test(void)
   {
       bool ret;

       // ...
       // Init GAP Timer Module.
       // Create app task.
       // ...

       ret = gap_reg_timer_cb(timer_callback, &timer_queue_id);

       if (ret)
       {
           gap_start_timer(&timer_handle, "test_timer", timer_queue_id,
                           TIMER_ID, TIMER_CHANN, 2000);
       }

       return 0;
   }
   \endcode
 *
 * \ingroup  GAP_TIMER
 */
bool gap_start_timer(void **pp_handle, const char *p_name, uint8_t queue_id,
					 uint8_t timer_id, uint16_t timer_chann, uint32_t timeout_ms);

/**
 * gap_timer.h
 *
 * \brief   Stop a timer that was previously started using \ref gap_start_timer.
 *
 * \param[in] pp_handle Pointer to the handle of timer being stopped.
 *
 * \return           The status of the timer stopping.
 * \retval true      Timer was stopped successfully.
 * \retval false     Timer was failed to stop.
 *
 * <b>Example usage</b>
   \code{.c}
   #define TIMER_ID          0x01
   #define TIMER_CHANN       0x1000

   uint8_t timer_queue_id;
   void   *timer_handle;

   // timer callback function.
   void timer_callback(uint8_t timer_id, uint16_t timer_chann)
   {
       if (timer_id == TIMER_ID && timer_chann == TIMER_CHANN)
       {
            // Delete the specified timer.
            gap_stop_timer(&timer_handle);
       }
   }

   // register timer callback function and start timer.
   int test(void)
   {
       bool ret;

       // ...
       // Init GAP Timer Module.
       // Create app task.
       // ...

       ret = gap_reg_timer_cb(timer_callback, &timer_queue_id);

       if (ret)
       {
           gap_start_timer(&timer_handle, "test_timer", timer_queue_id,
                           TIMER_ID, TIMER_CHANN, 2000);
       }

       return 0;
   }
   \endcode
 *
 * \ingroup  GAP_TIMER
 */
bool gap_stop_timer(void **pp_handle);

void gap_deinit_timer(void);

#ifdef __cplusplus
}
#endif

#endif

#endif /* GAP_TIMER_H */

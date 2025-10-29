// Copyright 2021-2025 Beken
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef _PM_SLEEP_CALLBACK_H_
#define _PM_SLEEP_CALLBACK_H_

#ifdef __cplusplus
extern "C" {
#endif

/****************************************************************************
 * Public Types
 ****************************************************************************/

/**
 * Sleep callback function type
 * 
 * @param arg  User-defined argument passed during registration
 * 
 * Note: Callback functions are called at specific points in sleep cycle:
 *   - Pre-sleep callback:  Called before entering sleep mode
 *   - Post-sleep callback: Called after waking up from sleep
 */
typedef void (*sleep_callback_t)(void *arg);

/****************************************************************************
 * Public Definitions - Callback Priority Levels
 ****************************************************************************/

/* Standard priority definitions for callback execution order
 * Lower value = Higher priority = Executes first
 * Range: 0 (highest) to 255 (lowest)
 */
#define PM_CALLBACK_PRIORITY_CRITICAL    0    /**< Critical hardware (clocks, PMU, power domains) */
#define PM_CALLBACK_PRIORITY_HIGH        50   /**< Important peripherals (UART, SPI, I2C) */
#define PM_CALLBACK_PRIORITY_NORMAL      100  /**< Standard drivers (GPIO, ADC, PWM) */
#define PM_CALLBACK_PRIORITY_LOW         150  /**< Non-critical modules (sensors, LEDs) */
#define PM_CALLBACK_PRIORITY_LOWEST      200  /**< Optional features (debug, logging, statistics) */

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/

/**
 * @brief Register a pre-sleep callback with execution priority
 * 
 * This callback will be called BEFORE the system enters sleep mode.
 * Callbacks are executed in priority order (0=first, 255=last).
 * 
 * Typical use cases:
 *   - Save peripheral states
 *   - Power down devices
 *   - Flush buffers
 * 
 * @param callback  Function to call before sleep (must not be NULL)
 * @param arg       User argument passed to callback (can be NULL)
 * @param priority  Execution priority: 0 (highest) to 255 (lowest)
 *                  Use predefined constants: PM_CALLBACK_PRIORITY_*
 * 
 * @return  0  Success
 *         -1  Invalid callback (NULL pointer)
 *         -2  Out of memory
 * 
 * Memory cost: 16 bytes per registration (32-bit ARM, aligned)
 * Time complexity: O(n) insertion, O(n) execution
 * 
 * Example:
 *   void uart_pre_sleep(void *arg) {
 *       // Save UART state and power down
 *   }
 *   bk_pm_pre_sleep_callback_register(uart_pre_sleep, NULL, PM_CALLBACK_PRIORITY_HIGH);
 * 
 * Priority table example:
 *   Priority 0   (CRITICAL): Clock manager, PMU
 *   Priority 50  (HIGH):     UART, SPI
 *   Priority 100 (NORMAL):   GPIO, ADC
 *   Priority 150 (LOW):      Sensors
 *   Priority 200 (LOWEST):   Debug logging
 */
bk_err_t bk_pm_pre_sleep_callback_register(sleep_callback_t callback, void *arg, uint8_t priority);

/**
 * @brief Unregister a pre-sleep callback
 * 
 * @param callback  Function to remove
 * 
 * @return  0  Success
 *         -1  Invalid callback (NULL pointer)
 *         -2  Callback not found
 * 
 * Memory: Frees 12 bytes
 * Time complexity: O(n) where n is number of registered callbacks
 * 
 * Example:
 *   bk_pm_pre_sleep_callback_unregister(uart_pre_sleep);
 */
bk_err_t bk_pm_pre_sleep_callback_unregister(sleep_callback_t callback);

/**
 * @brief Execute all pre-sleep callbacks
 * 
 * This function is called automatically by the PM system before entering sleep.
 * Application modules should NOT call this directly.
 * 
 * Execution order: LIFO (Last-In-First-Out)
 * - The most recently registered callback executes first
 * 
 * Time complexity: O(n)
 */
bk_err_t bk_pm_pre_sleep_callback_execute(void);

/**
 * @brief Register a post-sleep (wakeup) callback with execution priority
 * 
 * This callback will be called AFTER the system wakes up from sleep.
 * Callbacks are executed in priority order (0=first, 255=last).
 * 
 * Typical use cases:
 *   - Restore peripheral states
 *   - Power up devices
 *   - Reconfigure clocks
 * 
 * @param callback  Function to call after wakeup (must not be NULL)
 * @param arg       User argument passed to callback (can be NULL)
 * @param priority  Execution priority: 0 (highest) to 255 (lowest)
 *                  Use predefined constants: PM_CALLBACK_PRIORITY_*
 * 
 * @return  0  Success
 *         -1  Invalid callback (NULL pointer)
 *         -2  Out of memory
 * 
 * Memory cost: 16 bytes per registration (32-bit ARM, aligned)
 * Time complexity: O(n) insertion, O(n) execution
 * 
 * Example:
 *   void uart_post_sleep(void *arg) {
 *       // Restore UART state and power up
 *   }
 *   bk_pm_post_sleep_callback_register(uart_post_sleep, NULL, PM_CALLBACK_PRIORITY_HIGH);
 */
bk_err_t bk_pm_post_sleep_callback_register(sleep_callback_t callback, void *arg, uint8_t priority);

/**
 * @brief Unregister a post-sleep callback
 * 
 * @param callback  Function to remove
 * 
 * @return  0  Success
 *         -1  Invalid callback (NULL pointer)
 *         -2  Callback not found
 * 
 * Memory: Frees 12 bytes
 * Time complexity: O(n)
 * 
 * Example:
 *   bk_pm_post_sleep_callback_unregister(uart_post_sleep);
 */
bk_err_t bk_pm_post_sleep_callback_unregister(sleep_callback_t callback);

/**
 * @brief Execute all post-sleep callbacks
 * 
 * This function is called automatically by the PM system after waking up.
 * Application modules should NOT call this directly.
 * 
 * Execution order: LIFO (Last-In-First-Out)
 * 
 * Time complexity: O(n)
 */
bk_err_t bk_pm_post_sleep_callback_execute(void);


#ifdef __cplusplus
}
#endif

#endif /* _PM_SLEEP_CALLBACK_H_ */


/*
 * Copyright (c) 2016-2017 ARM Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/**
 * \file timer_cmsdk.h
 * \brief Generic driver for CMSDK APB Timers.
 *        The timer is a 32-bit down-counter with the following features:
 *        - optional programmable external clock source
 *        - programmable interrupt source, triggered if counter reaches 0
 *        - automatic reload if counter reaches 0
 */

#ifndef __CMSDK_TIMER_DRV_H__
#define __CMSDK_TIMER_DRV_H__

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Maximum reload value */
#define CMSDK_TIMER_MAX_RELOAD        INT32_MAX /* max of 32-bit */
#define CMSDK_TIMER_DEFAULT_RELOAD    CMSDK_TIMER_MAX_RELOAD

/** CMSDK timer device configuration structure */
struct cmsdk_timer_dev_cfg_t {
    const uintptr_t base;  /*!< Timer base address */
};

/** CMSDK timer device data structure */
struct cmsdk_timer_dev_data_t {
    bool is_initialized;  /*!< Indicates if the timer is initialized */
};

/* CMSDK timer device structure */
struct cmsdk_timer_dev_t {
    const struct cmsdk_timer_dev_cfg_t* const cfg;  /*!< Timer configuration */
    struct cmsdk_timer_dev_data_t* const data;      /*!< Timer data */
};

/**
 * \brief Initializes timer to a known default state, which is:
 *          - timer disabled
 *          - timer interrupt disabled
 *          - clock source set to internal
 *          - external input disabled
 *          - reload value maxed out
 *        Init should be called prior to any other process and
 *        it's the caller's responsibility to follow proper call order.
 *
 * \param[in] dev Timer configuration \ref cmsdk_timer_dev_t
 */
void cmsdk_timer_init(const struct cmsdk_timer_dev_t* dev);

/**
 * \brief Checks if a timer is initialized.
 *
 * \param[in] dev Timer configuration \ref cmsdk_timer_dev_t
 *
 * \return true if initialized, false otherwise
 */
bool cmsdk_timer_is_initialized(const struct cmsdk_timer_dev_t* dev);

/**
 * \brief Enables external input, which could be used as clock source
 *        by calling \ref cmsdk_timer_set_clock_to_external.
 *
 * \param[in] dev  Timer configuration \ref cmsdk_timer_dev_t
 */
void cmsdk_timer_enable_external_input(const struct cmsdk_timer_dev_t* dev);

/**
 * \brief Disables external input.
 *        Make sure if the timer is explicitly wanted to be stopped or set
 *        the clock source to internal by \ref cmsdk_timer_set_clock_to_internal.
 *
 * \param[in] dev  Timer configuration \ref cmsdk_timer_dev_t
 */
void cmsdk_timer_disable_external_input(const struct cmsdk_timer_dev_t* dev);

/**
 * \brief Checks if external input is enabled.
 *
 * \param[in] dev  Timer configuration \ref cmsdk_timer_dev_t
 *
 * \return true if enabled, false otherwise
 */
bool cmsdk_timer_is_external_input_enabled(const struct cmsdk_timer_dev_t* dev);

/**
 * \brief Sets the clock source to internal.
 *
 * \param[in] dev  Timer configuration \ref cmsdk_timer_dev_t
 */
void cmsdk_timer_set_clock_to_internal(const struct cmsdk_timer_dev_t* dev);

/**
 * \brief Sets the clock source to external.
 *        Make sure external input is enabled correspondingly
 *        by \ref cmsdk_timer_enable_external_input.
 *
 * \param[in] dev  Timer configuration \ref cmsdk_timer_dev_t
 */
void cmsdk_timer_set_clock_to_external(const struct cmsdk_timer_dev_t* dev);

/**
 * \brief Checks if clock source is external input.
 *
 * \param[in] dev  Timer configuration \ref cmsdk_timer_dev_t
 *
 * \return true if external, false if internal
 */
bool cmsdk_timer_is_clock_external(const struct cmsdk_timer_dev_t* dev);

/**
 * \brief Enables timer operation.
 *
 * \param[in] dev Timer configuration \ref cmsdk_timer_dev_t
 */
void cmsdk_timer_enable(const struct cmsdk_timer_dev_t* dev);

/**
 * \brief Disables the given hardware timer.
 *
 * \param[in] dev  Timer configuration \ref cmsdk_timer_dev_t
 */
void cmsdk_timer_disable(const struct cmsdk_timer_dev_t* dev);

/**
 * \brief Checks if a timer is enabled.
 *
 * \param[in] dev  Timer configuration \ref cmsdk_timer_dev_t
 *
 * \return true if enabled, false otherwise
 */
bool cmsdk_timer_is_enabled(const struct cmsdk_timer_dev_t* dev);

/**
 * \brief Enables timer interrupt.
 *
 * \param[in] dev       Timer configuration \ref cmsdk_timer_dev_t
 */
void cmsdk_timer_enable_interrupt(const struct cmsdk_timer_dev_t* dev);

/**
 * \brief Disables timer interrupt.
 *
 * \param[in] dev  Timer configuration \ref cmsdk_timer_dev_t
 */
void cmsdk_timer_disable_interrupt(const struct cmsdk_timer_dev_t* dev);

/**
 * \brief Checks if a timer interrupt is enabled.
 *
 * \param[in] dev  Timer configuration \ref cmsdk_timer_dev_t
 *
 * \return true if enabled, false otherwise
 */
bool cmsdk_timer_is_interrupt_enabled(const struct cmsdk_timer_dev_t* dev);

/**
 * \brief Gets timer interrupt status
 *
 * \param[in] dev  Timer configuration \ref cmsdk_timer_dev_t
 *
 * * \return true if active, false otherwise
 */
bool cmsdk_timer_is_interrupt_active(const struct cmsdk_timer_dev_t* dev);

/**
 * \brief Clears timer interrupt
 *        The interrupt request is held until it is cleared.
 *
 * \param[in] dev  Timer configuration \ref cmsdk_timer_dev_t
 */
void cmsdk_timer_clear_interrupt(const struct cmsdk_timer_dev_t* dev);

/**
 * \brief Reads timer current value.
 *
 * \param[in] dev  Timer configuration \ref cmsdk_timer_dev_t
 *
 * \return Timer value
 */
uint32_t cmsdk_timer_get_current_value(const struct cmsdk_timer_dev_t* dev);

/**
 * \brief Sets the reload value of the selected timer.
 *
 *        New reload value takes effect when:
 *        - timer is restarted
 *        - on timer underflow
 *        - when cmsdk_timer_reset is called
 *
 * \note  In r1p0 technical reference manual it's incorrectly stated
 *        writing the reload value automatically sets the current value also.
 *        r1p1 technical reference manual includes the fix.
 *
 * \param[in] dev  Timer configuration \ref cmsdk_timer_dev_t
 * \param[in] reload Timer reload value to set.
 *            This is the start value of the 32-bit down counter,
 *            which automatically reloaded if 0 is reached.
 */
void cmsdk_timer_set_reload_value(const struct cmsdk_timer_dev_t* dev,
                                  uint32_t reload);

/**
 * \brief Resets the timer counter to the reload value instantly
 *        (i.e. without waiting for underflow).
 *
 * \param[in] dev  Timer configuration \ref cmsdk_timer_dev_t
 */
void cmsdk_timer_reset(const struct cmsdk_timer_dev_t* dev);

/**
 * \brief Gets the reload value of the selected timer.
 *        This is the start value of the 32-bit down counter,
 *        which is automatically reloaded if 0 is reached by the counter.
 *
 * \param[in] dev  Timer configuration \ref cmsdk_timer_dev_t
 *
 * \return Reload value of the selected timer.
 */
uint32_t cmsdk_timer_get_reload_value(const struct cmsdk_timer_dev_t* dev);

/**
 * \brief Reads the number of ticks elapsed in the current cycle.
 *
 * \param[in] dev  Timer configuration \ref cmsdk_timer_dev_t
 *
 * \return Get elapsed number of ticks since last reload was set.
 *         Elapsed = (Reload value - Current value)
 */
uint32_t cmsdk_timer_get_elapsed_value(const struct cmsdk_timer_dev_t* dev);

#ifdef __cplusplus
}
#endif
#endif /* __CMSDK_TIMER_DRV_H__ */

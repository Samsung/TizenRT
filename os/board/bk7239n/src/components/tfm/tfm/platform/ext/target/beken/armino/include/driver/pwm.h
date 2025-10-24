// Copyright 2020-2021 Beken
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
//
#pragma once
#include <common/bk_include.h>
#include <driver/pwm_types.h>

#ifdef __cplusplus
extern "C" {
#endif

/* @brief Overview about this API header
 *
 */

/**
 * @brief PWM API
 * @defgroup bk_api_pwm PWM API group
 * @{
 */

/**
 * @brief     Init the PWM driver
 *
 * This API init the resoure common to all PWM channels:
 *   - Init PWM driver control memory
 *   - Configure PWM common clock to 26M
 *
 * This API should be called before any other PWM APIs.
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_pwm_driver_init(void);

/**
 * @brief     Deinit the PWM driver
 *
 * This API free all resource related to PWM and power down all PWM channels.
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_pwm_driver_deinit(void);

/**
 * @brief     Init the PWM channel
 *
 * This API init the PWM channel:
 *  - Power up the PWM channel
 *  - Configure the PWM channel clock to 26M
 *  - Map the PWM channel to dedicated GPIO port
 *  - Set the period and duty cycle.
 *
 * For duty/cycle relationship, refer to bk_pwm_set_period_duty()
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_PWM_NOT_INIT: PWM driver not init
 *    - BK_ERR_NULL_PARAM: config is NULL
 *    - BK_ERR_PWM_CHAN_ID: invalid PWM channel
 *    - BK_ERR_PWM_PERIOD_DUTY: PWM duty/period relationship is invalid
 *    - others: other errors.
 */
bk_err_t bk_pwm_init(pwm_chan_t chan, const pwm_init_config_t *config);

/**
 * @brief     Deinit a PWM channel
 *
 * This API deinit the PWM channel:
 *   - Stop the PWM channel
 *   - Reset all configuration of PWM channel to default value
 *   - Power down the PWM channel
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_pwm_deinit(pwm_chan_t chan);

/**
 * @brief     Stop a PWM channel
 *
 * This API init the resoure common to all PWM channels:
 *   - Init PWM driver control memory
 *   - Configure PWM common clock to 26M
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_pwm_start(pwm_chan_t chan);

/**
 * @brief     Stop a PWM channel
 *
 * This API init the resoure common to all PWM channels:
 *   - Init PWM driver control memory
 *   - Configure PWM common clock to 26M
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_pwm_stop(pwm_chan_t chan);

/**
 * @brief     Configure the PWM period and duty cycle.
 *
 * This API is used to configure the period and duty time. The unit is cycle of PWM channel clock,
 * since the PWM channel clock is default to 26M, the unit is 1/26M (1/26 000 000) seconds.
 *
 * The beken chip supports up to 3 duties, the exact duty numbers depends on the target type.
 *
 * EXample1 - the simplest case::
 *
 *     Clock cycle = 1/26M
 *     Initial signal is High
 *     duty_cycle = D1 = 3
 *     duty2_cycle = 0
 *     duty3_cycle = 0
 *     period_cycle = 3 + 2 = 5
 *
 *                       |<-C->|
 *                        __    __    __    __    __    __    __    __    __    __    __    __
 *     Clock Signal:   __|  |__|  |__|  |__|  |__|  |__|  |__|  |__|  |__|  |__|  |__|  |__|  |__
 *                       __________________             _________________             ___________
 *     PWM Signal:    __|                  |___________|                 |___________|
 *
 *     Duty1:           |<------- D1 ----->|
 *
 *     Period:          |<---------- Period1 --------->|<------ Period2 ------------>|
 *
 *
 * Example2 - 3 duty with initial signal high::
 *
 *     Clock cycle = 1/26M
 *     Duty_cycle = D1 = 1
 *     Duty2_cycle = D2 = 2
 *     Duty3_cycle = D3 = 3
 *     period_cycle = D1 + D2 + D3 = 6
 *
 *                       |<-C->|
 *                        __    __    __    __    __    __    __    __    __    __    __    __    __
 *     Clock Signal:   __|  |__|  |__|  |__|  |__|  |__|  |__|  |__|  |__|  |__|  |__|  |__|  |__|
 *                       ______             _________________ _____             _________________
 *     PWM Signal:    __|      |___________|                 |     |___________|                 |__
 *
 *     Duty1:           |<-D1->|<----D2--->|<------D3------->|
 *
 *     Period:          |<--------------- Period1 ---------->|<------------ Period2 ------------>|
 *
 *
 * Example3 - 3 duty with initial signal low::
 *
 *     Clock cycle = 1/26M
 *     Duty_cycle = D1 = 1
 *     Duty2_cycle = D2 = 2
 *     Duty3_cycle = D3 = 3
 *     period_cycle = D1 + D2 + D3 = 6
 *
 *                       |<-C->|
 *                        __    __    __    __    __    __    __    __    __    __    __    __    __
 *     Clock Signal:   __|  |__|  |__|  |__|  |__|  |__|  |__|  |__|  |__|  |__|  |__|  |__|  |__|
 *                     __       ___________                         ___________
 *     PWM Signal:       |_____|           |_________________|_____|           |_________________|__
 *
 *     Duty1:           |<-D1->|<----D2--->|<------D3------->|
 *
 *     Period:          |<--------------- Period1 ---------->|<------------ Period2 -------------|
 *
 *
 * Example4 - 3 duty with initial signal low and period > (D1 + D2 + D3)::
 *
 *     Clock cycle = 1/26M
 *     Duty_cycle = D1 = 1
 *     Duty2_cycle = D2 = 2
 *     Duty3_cycle = D3 = 3
 *     period_cycle = D1 + D2 + D3 + 1 = 7
 *
 *                       |<-C->|
 *                        __    __    __    __    __    __    __    __    __    __    __    __    __
 *     Clock Signal:   __|  |__|  |__|  |__|  |__|  |__|  |__|  |__|  |__|  |__|  |__|  |__|  |__|
 *                     __       ___________                   _____       ___________
 *     PWM Signal:       |_____|           |_________________|     |_____|           |______________
 *
 *     Duty1:           |<-D1->|<----D2--->|<------D3------->|
 *
 *     Period:          |<--------------- Period ----------------->|
 *
 * @attention 1. The period should great or equal to (D1 + D2 + D3)
 * @attention 2. If the duty Dx is 0, then Dx+1 to D3 should also be 0. If D1 is 0, it indicates the
 *               duty ratio is 0% and the signal is always low.
 * @attention 3. If D1 equals period, it indicates the duty ratio is 100% and the signal is always high.
 * @attention 4. The signal strengh between two successive duties is opposite, e.g. if signal of D1
 *               is high, then signal of D2 is low.
 * @attention 5. The initial signal is the signal strengh of D1 if D1 is not 0.
 * @attention 6. The duty/period configuration is very flexible, different configurations may have
 *               same result.
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_NULL_PARAM: config is NULL
 *    - BK_ERR_PWM_CHAN_NOT_INIT: PWM channel not init
 *    - BK_ERR_PWM_CHAN_ID: invalid PWM channel
 *    - BK_ERR_PWM_PERIOD_DUTY: PWM duty/period relationship is invalid
 *    - others: other errors.
 */
bk_err_t bk_pwm_set_period_duty(pwm_chan_t chan, pwm_period_duty_config_t *config);

/**
 * @brief     Set the initial signal to low
 *
 * Set the initial signal level of the first duty which is indicated by duty_cycle.
 *
 * @attention The signal will always be high if the duty_cycle equals period cycle
 *            (duty ratio is 100%), regardless whether this API is called or NOT.
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_PWM_CHAN_NOT_INIT: PWM channel not init
 *    - BK_ERR_PWM_CHAN_ID: invalid PWM channel
 *    - others: other errors.
 */
bk_err_t bk_pwm_set_init_signal_low(pwm_chan_t chan);

/**
 * @brief     Set the initial signal to high
 *
 * Set the initial signal level of the first duty which is indicated by duty_cycle.
 *
 * @attention The signal will always be low if the duty_cycle is 0 (duty ratio is 0%),
 *            regardless whether this API is called or NOT.
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_PWM_CHAN_NOT_INIT: PWM channel not init
 *    - BK_ERR_PWM_CHAN_ID: invalid PWM channel
 *    - others: other errors.
 */
bk_err_t bk_pwm_set_init_signal_high(pwm_chan_t chan);

/**
 * @brief     Register the interrupt service routine for PWM channel
 *
 * @attention 1. Both bk_pwm_capture_init() and this API can set the ISR, the one called later
 *            makes sense.
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_PWM_NOT_INIT: PWM driver not init
 *    - BK_ERR_PWM_CHAN_ID: invalid PWM channel
 *    - others: other errors.
 */
bk_err_t bk_pwm_register_isr(pwm_chan_t chan, pwm_isr_t isr);

/**
 * @brief     Enable interrupt of PWM channel
 *
 * The PWM channel interrupt is disabled by default when starting the channel by bk_pwm_start() or
 * bk_pwm_group_start(), while it's enabled by default when starting the channel by
 * bk_pwm_capture_start().
 *
 * This API is designed mainly for debug purpose, we can use it to enable the interrupt of any PWM
 * channel, regardless of whether the channel is started by bk_pwm_start() or bk_pwm_group_start() or
 * bk_pwm_capture_start().
 *
 * The bk_pwm_stop() and bk_pwm_group_stop() also disable the interrupt, but the bk_pwm_start()
 * and bk_pwm_group_start will not enable the interrupt. So if the channel interrupt is enabled,
 * and then stop and start the channel, we need to call this API to re-enable the interrupt.
 *
 * @attention 1. The interrupt can be enabled only after PWM channel is started by bk_pwm_start().
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_PWM_NOT_INIT: PWM driver not init
 *    - BK_ERR_PWM_CHAN_ID: invalid PWM channel
 *    - BK_ERR_PWM_CHAN_NOT_START: PWM channel is not started
 *    - others: other errors.
 */
bk_err_t bk_pwm_enable_interrupt(pwm_chan_t chan);

/**
 * @brief     Disable interrupt of PWM channel
 *
 * Stop the interrupt of any PWM channel, regardless whether the channel is in PWM mode, or
 * PWM capture mode, or PWM group mode.
 *
 * @attention 1. bk_pwm_capture_start() will enable the interrupt anyway, so if we want to disable
 *             the interrupt of PWM capture channel, we need to call this API anytime
 *             bk_pwm_capture_start() is called.
 *
 * @param chan the channel ID
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_PWM_NOT_INIT: PWM driver not init
 *    - BK_ERR_PWM_CHAN_ID: invalid PWM channel
 *    - others: other errors.
 */
bk_err_t bk_pwm_disable_interrupt(pwm_chan_t chan);

/**
 * @brief     Init the PWM group
 *
 * This API init the PWM group.
 * The PWM group is a channel pair that has following attributes:
 *  - The period is same
 *  - The initial signal level is opposite
 *  - Start and stop at the same time
 *
 * Below is a picture to demonstrate the relationship between duty and period::
 *
 *                  |<------------------------ period_cycle ---------------------------->|
 *                  |<--chan1_duty_cycle -->|<- D1 --|<--- chan2_duty_cycle --->|<- D2 ->|
 *                   _______________________
 *      channe1 1   |                       |____________________________________________|
 *                                                    __________________________
 *      channel 2   |________________________________|                          |________|
 *
 * The D1/D2 indicates the dead time cycle, it's calculated based on following formula:
 * D1 = D2 = (period - chan1_duty_cycle - chan2_duty_cycle)/2
 *
 * @attention 1. This API doesn't check whether the PWM channel is already used in other mode.
 *            e.g. If the API doesn't returns error if config->chan1 is already used in capture
 *            mode, the application need to make sure the channel configured in the group is not
 *            used for other purpose.
 * @attention 2. The chan2_duty_cycle is actually the 2nd signal wave for channel 2, as depicted
 *            in above picture.
 * @attention 3. This API supports to group any two available channel into a PWM group, however,
 *            we recommend to group 0/1, 2/3, 4/5 into a group, because hardware can support these
 *            group directly and we can start the channels in the group at exactly same time.
 * @attention 4. The chan1_duty_cycle and chan2_duty_cycle should be great than 0.
 *
 * @param config configuration of PWM group
 * @param group store the group ID if the API is succeed.
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_PWM_NOT_INIT: PWM driver not init
 *    - BK_ERR_NULL_PARAM: config or group is NULL
 *    - BK_ERR_PWM_CHAN_ID: channel 1 or 2 in the group is invalid
 *    - BK_ERR_PWM_GROUP_SAME_CHAN: channel 1 and 2 are same
 *    - BK_ERR_PWM_GROUP_EXIST: a group with channel 1 and 2 is already exists
 *    - BK_ERR_PWM_GROUP_CHAN_USED: a existing group has a channel equals to channel 1 or 2
 *    - BK_ERR_PWM_GROUP_DUTY: period cycle < (chan1_duty_cycle + chan2_duty_cycle)
 *    - BK_ERR_PWM_PERIOD_DUTY: PWM duty/period relationship is invalid
 *    - others: other errors.
 */
bk_err_t bk_pwm_group_init(const pwm_group_init_config_t *config, pwm_group_t *group);

/**
 * @brief     Deinit a PWM group
 *
 * This API stops and powers down channels in a PWM group.
 *
 * @attention This API returns BK_OK directly if the group doesn't exist.
 *
 * @param group the group ID returned bk bk_pwm_group_init()
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_pwm_group_deinit(pwm_group_t group);

/**
 * @brief     Configure the PWM group
 *
 * Configure the duty and period of a PWM group, for duty/period relationship refer to
 * bk_pwm_group_init().
 *
 * @attention The configuration can't take effect until the current period is ended
 *
 * @param group the group ID to be configured
 * @param config configuration of PWM group
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_NULL_PARAM: config or group is NULL
 *    - BK_ERR_PWM_GROUP_NOT_EXIST: the group doesn't exist
 *    - BK_ERR_PWM_GROUP_DUTY: period cycle < (chan1_duty_cycle + chan2_duty_cycle)
 *    - BK_ERR_PWM_PERIOD_DUTY: PWM duty/period relationship is invalid
 *    - others: other errors.
 */
bk_err_t bk_pwm_group_set_config(pwm_group_t group, const pwm_group_config_t *config);

/**
 * @brief     Start the PWM group
 *
 * @param group the group ID to be started
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_PWM_GROUP_NOT_EXIST: the group doesn't exist
 *    - others: other errors.
 */
bk_err_t bk_pwm_group_start(pwm_group_t group);

/**
 * @brief     Stop the PWM group
 *
 * @param group the group ID to be stopped
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_PWM_GROUP_NOT_EXIST: the group doesn't exist
 *    - others: other errors.
 */
bk_err_t bk_pwm_group_stop(pwm_group_t group);

/**
 * @brief     Init the PWM capture
 *
 * The PWM capture can calculate the cycles between two different edges, currently we support
 * following mode:
 * 1. PWM_CAPTURE_POS - calcualte cycles between two pos-edge, the interrupt trigger in post edge
 * 2. PWM_CAPTURE_NEG - calcualte cycles between two neg-edge, the interrupt trigger in neg edge
 * 3. PWM_CAPTURE_EDGE - calculate cycles between two edge (pos or neg), the interrupt trigger
 *                       in post or neg edge.
 *
 * Example 1 - init the capture in PWM_CAPTURE_POS mode::
 *
 *                     |<------ C1 ---->|<------- C2 ------->|
 *                      ______           ___________          ________________            _
 *      PWM input     _|      |_________|           |________|                |__________|
 *
 *      Capture value                   |    C1              |             C2            |
 *
 * @param chan the capture channel
 * @param config the configuration of capture
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_PWM_NOT_INIT: PWM driver not init
 *    - BK_ERR_NULL_PARAM: config is NULL
 *    - BK_ERR_PWM_CHAN_ID: invalid channel
 *    - BK_ERR_PWM_CAPTURE_EDGE: capture edge is invalid or not supported.
 *    - others: other errors.
 */
bk_err_t bk_pwm_capture_init(pwm_chan_t chan, const pwm_capture_init_config_t *config);

/**
 * @brief     Deinit the PWM capture
 *
 * @param chan the capture channel
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_PWM_NOT_INIT: PWM driver not init
 *    - BK_ERR_PWM_CHAN_ID: invalid channel
 *    - others: other errors.
 */
bk_err_t bk_pwm_capture_deinit(pwm_chan_t chan);

/**
 * @brief     Start the PWM capture
 *
 * @param chan channel to be started
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_PWM_NOT_INIT: PWM driver not init
 *    - BK_ERR_PWM_CHAN_ID: invalid PWM channel
 *    - others: other errors.
 */
bk_err_t bk_pwm_capture_start(pwm_chan_t chan);

/**
 * @brief     Stop the PWM capture
 *
 * @param chan channel to be stopped
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_PWM_NOT_INIT: PWM driver not init
 *    - BK_ERR_PWM_CHAN_ID: invalid PWM channel
 *    - others: other errors.
 */
bk_err_t bk_pwm_capture_stop(pwm_chan_t chan);

/**
 * @brief     Get the capture value
 *
 * @attention The caller need to make sure the parameter chan is correct!
 *
 * @param chan PWM channel
 *
 * @return capture value
 */
uint32_t bk_pwm_capture_get_value(pwm_chan_t chan);

/**
 * @brief     Set pwm channel as timer mode
 *
 * @attention The caller need to make sure the parameter chan is correct!
 *
 * @param chan PWM channel
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_PWM_CHAN_ID: invalid PWM channel
 *    - others: other errors.
 */
bk_err_t bk_pwm_set_mode_timer(pwm_chan_t chan);


/**
 * @}
 */

#ifdef __cplusplus
}
#endif

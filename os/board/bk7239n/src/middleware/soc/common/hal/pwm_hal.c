#include <common/bk_include.h>
#include "bk_gpio.h"
#include "pwm_hal.h"
#include "pwm_ll.h"

bk_err_t pwm_hal_init(pwm_hal_t *hal)
{
	hal->hw = (pwm_hw_t *)PWM_LL_REG_BASE(hal->id);
	pwm_ll_init(hal->hw);
	return BK_OK;
}

bk_err_t pwm_hal_init_pwm(pwm_hal_t *hal, pwm_chan_t chan, const pwm_hal_config_t *config)
{
	/* Import notes:
	* On BK7251, we found that, if we power up the channel and disable the PWM
	* channel immediately, then the PWM interrupt will not happen.
	*
	* So don't disable the PWM channel in PWM channel init API.
	**/
	//pwm_ll_disable(hal->hw, chan);

	pwm_ll_disable_interrupt(hal->hw, chan);
	pwm_ll_clear_chan_interrupt_status(hal->hw, chan);
	pwm_ll_set_mode_pwm(hal->hw, chan);

	pwm_ll_set_t1(hal->hw, chan, config->t1);
	pwm_ll_set_t2(hal->hw, chan, config->t2);
	pwm_ll_set_t3(hal->hw, chan, config->t3);
	pwm_ll_set_t4(hal->hw, chan, config->t4);
	return BK_OK;
}

bk_err_t pwm_hal_start_common(pwm_hal_t *hal, pwm_chan_t chan)
{
	pwm_ll_enable(hal->hw, chan);
	return BK_OK;
}

bk_err_t pwm_hal_stop_common(pwm_hal_t *hal, pwm_chan_t chan)
{
	/* We should disable the PWM interrupt first before stopping
	     * PWM. Otherwise the interrupt will always be triggered and the
	     * interrupt status can't be cleared (Observed on BK7251).
	 **/
	pwm_ll_disable_interrupt(hal->hw, chan);
	pwm_ll_clear_chan_interrupt_status(hal->hw, chan);
	pwm_ll_disable(hal->hw, chan);

	//The original implementation for BK7231N will reset the hardware configuration here!
	//Now we do it in deinit API.
	return BK_OK;
}

bk_err_t pwm_hal_init_capture(pwm_hal_t *hal, pwm_chan_t chan, const pwm_hal_capture_config_t *config)
{
	//pwm_ll_disable(hal->hw, chan);
	pwm_ll_disable_interrupt(hal->hw, chan);

	if (config->edge == PWM_CAPTURE_POS)
		pwm_ll_set_mode_capture_pos(hal->hw, chan);
	else if (config->edge == PWM_CAPTURE_NEG)
		pwm_ll_set_mode_capture_neg(hal->hw, chan);
	else if (config->edge == PWM_CAPTURE_EDGE)
		pwm_ll_set_mode_capture_edge(hal->hw, chan);
	else {
		//Never be here, caller validate the parameter
	}

	pwm_ll_set_t1(hal->hw, chan, 0);
	pwm_ll_set_t2(hal->hw, chan, 0);
	pwm_ll_set_t3(hal->hw, chan, 0);
	pwm_ll_set_t4(hal->hw, chan, 0);
	return BK_OK;
}

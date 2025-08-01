/*
 * Copyright (c) 2024 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */
 
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/device.h>
#include <zephyr/drivers/pwm.h>
#include <dk_buttons_and_leds.h>

LOG_MODULE_REGISTER(Lesson4_Exercise2, LOG_LEVEL_INF);

#define PWM_LED0        DT_ALIAS(pwm_led0)

static const struct pwm_dt_spec pwm_led0 = PWM_DT_SPEC_GET(PWM_LED0);

/* STEP 5.4 - Retrieve the device structure for the servo motor */
#define CLK_OUT     DT_NODELABEL(clkout) 
static const struct pwm_dt_spec pwm_clk_out = PWM_DT_SPEC_GET(CLK_OUT);

/* STEP 5.5 - Use DT_PROP() to obtain the minimum and maximum duty cycle */
#define PWM_CLK_MIN_PULSE_WIDTH    DT_PROP(CLK_OUT, min_pulse)
#define PWM_CLK_MAX_PULSE_WIDTH    DT_PROP(CLK_OUT, max_pulse)

#define PWM_CLK_PULSE_WIDTH PWM_USEC(25)
#define PWM_CLK_PERIOD   PWM_USEC(50)


#define PWM_PERIOD   PWM_MSEC(20)

/* STEP  2.2 - Define minimum and maximum duty cycle */
/* STEP 4.2 - Change the duty cycles for the LED */
#define PWM_MIN_PULSE_WIDTH 20000000
#define PWM_MAX_PULSE_WIDTH 50000000


/* STEP 2.1 - Create a function to set the angle of the motor */
/* STEP 5.8 - Change set_motor_angle() to use the pwm_servo device */

int start_clk()
{
	int err;
    
    err = pwm_set_dt(&pwm_clk_out, PWM_CLK_PERIOD, PWM_CLK_PULSE_WIDTH);
    if (err) {
        LOG_ERR("pwm_set_dt_returned %d", err);
    }
    return err;

}

int stop_clk()
{
	int err;
    
    err = pwm_set_dt(&pwm_clk_out, PWM_CLK_PERIOD, 0);
    if (err) {
        LOG_ERR("pwm_set_dt_returned %d", err);
    }
    return err;

}

int set_motor_angle(uint32_t pulse_width_ns)
{
    int err;
    
    err = pwm_set_dt(&pwm_clk_out, PWM_CLK_PERIOD, pulse_width_ns);
    if (err) {
        LOG_ERR("pwm_set_dt_returned %d", err);
    }
    return err;
}
/* STEP 4.3 - Create a function to set the duty cycle of a PWM LED */
int set_led_blink(uint32_t period, uint32_t pulse_width_ns){
    int err;
    err = pwm_set_dt(&pwm_led0, period, pulse_width_ns);
        if (err) {
        LOG_ERR("pwm_set_dt_returned %d", err);
    }
    return err;
}

void button_handler(uint32_t button_state, uint32_t has_changed)
{
    int err = 0;
	if (has_changed & button_state)
	{
		switch (has_changed)
		{
            /* STEP 2.4 - Change motor angle when a button is pressed */
            /* STEP 5.6 - Update the button handler with the new duty cycle */
            case DK_BTN1_MSK:
                LOG_INF("Button 1 pressed");
                //err = set_motor_angle(0);
				err = stop_clk();
                break;
            case DK_BTN2_MSK:
                LOG_INF("Button 2 pressed");
                //err = set_motor_angle(PWM_CLK_PULSE_WIDTH);
				err = start_clk();
                break;
            /* STEP 4.4 - Change LED when a button is pressed */
            case DK_BTN3_MSK:
                LOG_INF("Button 3 pressed");
                err = set_led_blink(2*PWM_PERIOD, PWM_MIN_PULSE_WIDTH);
                break;
            case DK_BTN4_MSK:
                LOG_INF("Button 4 pressed");
                err = set_led_blink(4*PWM_PERIOD, PWM_MAX_PULSE_WIDTH);
                break;
			default:
				break;
		}
        if (err) {
            LOG_ERR("Error: couldn't set duty cycle, err %d", err);
        }
	}
}

int main(void)
{

    int err = 0;
    
	
    if (dk_buttons_init(button_handler)) {
        LOG_ERR("Failed to initialize the buttons library");
    }
	

    /* STEP 2.3 - Check if the device is ready and set its initial value */

	/*
    if (!pwm_is_ready_dt(&pwm_led0)) {
        LOG_ERR("Error: PWM device %s is not ready", pwm_led0.dev->name);
        return 0;
	}
    err = pwm_set_dt(&pwm_led0, 2*PWM_PERIOD, PWM_MIN_PULSE_WIDTH);
    if (err) {
        LOG_ERR("pwm_set_dt returned %d", err);
        return 0;
    }
	*/

    /* STEP 5.7 - Check if the motor device is ready and set its initial value */
    LOG_INF("Setting CLK");
    if (!pwm_is_ready_dt(&pwm_clk_out)) {
        LOG_ERR("Error: PWM device %s is not ready", pwm_clk_out.dev->name);
        return 0;
	}

    err = pwm_set_dt(&pwm_clk_out, PWM_CLK_PERIOD, PWM_CLK_PULSE_WIDTH);
    if (err) {
        LOG_ERR("pwm_set_dt returned %d", err);
        return 0;
    }

    return 0;
}
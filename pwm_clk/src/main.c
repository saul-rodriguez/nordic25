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

#include "pwm_clk.h"

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
				err = pwm_clk_stop();
                break;
            case DK_BTN2_MSK:
                LOG_INF("Button 2 pressed");
                //err = set_motor_angle(PWM_CLK_PULSE_WIDTH);
				err = pwm_clk_start();
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
	
    err = pwm_clk_setup();
    if (!err) {
        LOG_ERR("Error: PWM_CLK device %s is not ready");
    } else {
        pwm_clk_start();
    }

    return 0;
}
/*
 * Copyright (c) 2016 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdio.h>
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/spi.h>
#include <zephyr/sys/printk.h>

#include "aska.h"

/* 1000 msec = 1 sec */
#define SLEEP_TIME_MS   10000

/* The devicetree node identifier for the "led0" alias. */
#define LED0_NODE DT_ALIAS(led0)

/*
 * A build error on this line means your board is unsupported.
 * See the sample documentation for information on how to fix this.
 */
static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED0_NODE, gpios);

int main(void)
{
	int ret;
	bool led_state = true;

	printk("Start NMES ASKA\n");
	
	if (!gpio_is_ready_dt(&led)) {
		return 0;
	}

	ret = gpio_pin_configure_dt(&led, GPIO_OUTPUT_ACTIVE);
	if (ret < 0) {
		return 0;
	}

	ret = ASKA_spi_setup();

	uint8_t ic_add;

	ic_add = IC_ADDRESS_0;

	while (1) {
		ret = gpio_pin_toggle_dt(&led);
		if (ret < 0) {
			return 0;
		}

		led_state = !led_state;
		//printk("LED state: %s\n", led_state ? "ON" : "OFF");

		//spi_write_buff();
		//ASKA_spi_write_buff(data,5);
		ASKA_setProgram(ic_add,50,50,10,30,10,1,4,1,2); //50 Hz, 50mA,1s/3s,ramp 1s, pd 200
		//printk("50 Hz, 50mA,1s/3s,ramp 1s, pd 200\n");
        //k_msleep(SLEEP_TIME_MS);
		k_busy_wait(10000000);

        ASKA_setProgram(ic_add,10,50,10,10,5,1,4,1,2); //10 Hz, 50mA,1s/1s,ramp 0.5s, pd 200
		//printk("10 Hz, 50mA,1s/1s,ramp 0.5s, pd 200\n");        
		k_busy_wait(10000000);

		ASKA_setProgram(ic_add,50,25,10,30,10,1,4,1,2); //50 Hz, 50mA,1s/3s,ramp 1s, pd 200
		//printk("50 Hz, 50mA,1s/3s,ramp 1s, pd 200\n");
        //k_msleep(SLEEP_TIME_MS);
		k_busy_wait(10000000);

		//k_msleep(SLEEP_TIME_MS);		
        ASKA_setProgram(ic_add,10,50,10,10,0,1,4,1,2); //10 Hz, 50mA,1s/1s,ramp 0s, pd 200
       // printk("10 Hz, 50mA,1s/1s,ramp 0s, pd 200");
	   k_busy_wait(10000000);

		//k_msleep(SLEEP_TIME_MS);
        //ASKA_setProgram(ic_add,25,50,10,10,0,1,7,1,2); //25 Hz, 50mA,1s/1s,ramp 0s, pd 200
		//printk("25 Hz, 50mA,1s/1s,ramp 0s, pd 200");
		//k_msleep(SLEEP_TIME_MS);
		
	}
	return 0;
}

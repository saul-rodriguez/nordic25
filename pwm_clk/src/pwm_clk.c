#include "pwm_clk.h"


static const struct pwm_dt_spec pwm_clk_out = PWM_DT_SPEC_GET(CLK_OUT);

int pwm_clk_setup()
{
     int err = 0;

    //LOG_INF("Setting CLK");
    if (!pwm_is_ready_dt(&pwm_clk_out)) {
        //LOG_ERR("Error: PWM device %s is not ready", pwm_clk_out.dev->name);
        return 0;
	}
}

int pwm_clk_start()
{
    int err;
    
    err = pwm_set_dt(&pwm_clk_out, PWM_CLK_PERIOD, PWM_CLK_PULSE_WIDTH);
    if (err) {
        //LOG_ERR("pwm_set_dt_returned %d", err);
    }
    return err;
}

int pwm_clk_stop()
{
    int err;
    
    err = pwm_set_dt(&pwm_clk_out, PWM_CLK_PERIOD, 0);
    if (err) {
        //LOG_ERR("pwm_set_dt_returned %d", err);
    }
    return err;
}


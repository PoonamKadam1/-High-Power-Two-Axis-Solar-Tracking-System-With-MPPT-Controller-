/*
 * Example integration outline for the MPPT logic.
 *
 * Implement these functions for the actual MCU/ADC/PWM board. This file is
 * intentionally a template: the report does not provide ADC calibration,
 * converter switching frequency, PWM register setup or safe duty limits.
 */

#include <stdint.h>
#include <stdbool.h>

/* Include the MPPT state and function declarations from mppt_po.c. */
typedef struct
{
    float voltage_volts;
    float current_amperes;
} pv_sample_t;

typedef struct
{
    float previous_power;
    float previous_voltage;
    float duty_cycle;
    int8_t perturb_direction;
    bool initialized;
} mppt_state_t;

extern void mppt_board_init(void);
extern pv_sample_t read_pv_measurements(void);
extern void set_converter_duty(float duty);
extern float mppt_perturb_and_observe(mppt_state_t *state,
                                      const pv_sample_t *sample);
extern void delay_ms(uint16_t milliseconds);

int main(void)
{
    mppt_state_t state = {0};
    pv_sample_t sample;
    float duty;

    mppt_board_init();

    for (;;)
    {
        sample = read_pv_measurements();
        duty = mppt_perturb_and_observe(&state, &sample);

        /*
         * Do not enable this output until duty polarity, limits, over-current,
         * over-voltage, thermal and battery protections have been validated.
         */
        set_converter_duty(duty);

        delay_ms(100);
    }

    return 0;
}

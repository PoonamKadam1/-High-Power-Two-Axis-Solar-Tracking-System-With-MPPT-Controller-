/*
 * Maximum Power Point Tracking (MPPT) - Perturb and Observe algorithm
 *
 * The project report describes a DC-DC buck-boost converter and explains
 * the Perturb & Observe (P&O) method. It states that converter duty cycle
 * is adjusted to move the PV operating point toward maximum power.
 *
 * This file implements the P&O decision logic only. It does NOT configure
 * PWM hardware or directly drive a power converter. Duty-cycle limits,
 * scaling, polarity, switching frequency and protection thresholds must
 * come from the actual power-stage design.
 */

#include <stdint.h>
#include <stdbool.h>

#define DUTY_MIN       0.05f
#define DUTY_MAX       0.90f
#define DUTY_STEP      0.005f
#define POWER_EPSILON  0.001f

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

/*
 * Convert measured PV voltage and current to power.
 * ADC-to-voltage/current calibration must be completed in the board layer.
 */
static float calculate_power(const pv_sample_t *sample)
{
    return sample->voltage_volts * sample->current_amperes;
}

static float clamp_duty(float duty)
{
    if (duty > DUTY_MAX)
    {
        return DUTY_MAX;
    }

    if (duty < DUTY_MIN)
    {
        return DUTY_MIN;
    }

    return duty;
}

/*
 * Update the duty-cycle command using Perturb & Observe.
 *
 * General P&O decision:
 *   - If power increases after perturbation, continue in that direction.
 *   - If power decreases, reverse the perturbation direction.
 *
 * Converter duty-cycle polarity is topology-dependent. Verify whether
 * increasing duty raises or lowers PV voltage for the actual converter.
 */
float mppt_perturb_and_observe(mppt_state_t *state,
                               const pv_sample_t *sample)
{
    float power;
    float delta_power;
    float delta_voltage;

    power = calculate_power(sample);

    if (!state->initialized)
    {
        state->previous_power = power;
        state->previous_voltage = sample->voltage_volts;
        state->duty_cycle = clamp_duty(state->duty_cycle);
        state->perturb_direction = 1;
        state->initialized = true;
        return state->duty_cycle;
    }

    delta_power = power - state->previous_power;
    delta_voltage = sample->voltage_volts - state->previous_voltage;

    /*
     * Reverse direction when the last voltage perturbation reduced power.
     * The sign convention must be validated with the selected converter.
     */
    if (delta_power < -POWER_EPSILON)
    {
        state->perturb_direction = (int8_t)(-state->perturb_direction);
    }
    else if (delta_power > POWER_EPSILON)
    {
        /* Continue perturbing in the same direction. */
    }
    else
    {
        /* Near the same power: retain direction to continue searching. */
    }

    (void)delta_voltage; /* Retained for logging/diagnostics if desired. */

    state->duty_cycle +=
        ((float)state->perturb_direction * DUTY_STEP);

    state->duty_cycle = clamp_duty(state->duty_cycle);
    state->previous_power = power;
    state->previous_voltage = sample->voltage_volts;

    return state->duty_cycle;
}

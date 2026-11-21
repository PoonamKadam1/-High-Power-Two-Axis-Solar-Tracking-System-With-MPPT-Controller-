/*
 * High Power Two Axis Solar Tracking System With MPPT Controller
 * Solar tracking logic based on the project report by Poonam Autade.
 *
 * arrangement:
 *   - Four LDR sensors arranged around the panel
 *   - One sensor pair for east-west tracking
 *   - One sensor pair for north-south tracking
 *   - Two 12 V geared stepper motors
 *   - L293D motor driver is mentioned in the report
 *   - An additional LDR is described for night-mode detection
 *
 

#include <stdint.h>
#include <stdbool.h>
 */
#define LDR_DEADBAND       40u
#define TRACKING_DELAY_MS  200u
#define NIGHT_THRESHOLD    100u

typedef enum
{
    MOTOR_STOP = 0,
    MOTOR_FORWARD,
    MOTOR_REVERSE
} motor_direction_t;

typedef struct
{
    uint16_t east;
    uint16_t west;
    uint16_t north;
    uint16_t south;
    uint16_t night;
} ldr_readings_t;

/*
 * Hardware abstraction functions.
 * Supply implementations for the selected MCU, ADC and motor driver.
 */
extern void board_init(void);
extern ldr_readings_t read_ldr_sensors(void);
extern void east_west_motor(motor_direction_t direction);
extern void north_south_motor(motor_direction_t direction);
extern void delay_ms(uint16_t milliseconds);

/* Return motor direction from a pair of LDR readings. */
static motor_direction_t compare_ldr_pair(uint16_t first,
                                          uint16_t second)
{
    if ((first > second) && ((first - second) > LDR_DEADBAND))
    {
        return MOTOR_FORWARD;
    }

    if ((second > first) && ((second - first) > LDR_DEADBAND))
    {
        return MOTOR_REVERSE;
    }

    return MOTOR_STOP;
}

/*
 * The exact LDR-to-motor polarity is not specified in the report.
 * If a motor moves away from the brighter sensor during testing,
 * reverse the corresponding FORWARD/REVERSE mapping.
 */
static void track_east_west(const ldr_readings_t *ldr)
{
    motor_direction_t direction;

    direction = compare_ldr_pair(ldr->east, ldr->west);
    east_west_motor(direction);
}

static void track_north_south(const ldr_readings_t *ldr)
{
    motor_direction_t direction;

    direction = compare_ldr_pair(ldr->north, ldr->south);
    north_south_motor(direction);
}

static bool is_night(const ldr_readings_t *ldr)
{
    return (ldr->night < NIGHT_THRESHOLD);
}

int main(void)
{
    ldr_readings_t ldr;

    board_init();

    for (;;)
    {
        ldr = read_ldr_sensors();

        /*
         * The report describes a separate LDR for night-mode sensing.
         * At night the motors are stopped. The report also describes
         * returning the panel to its initial position after sunset, but
         * does not provide a complete homing/limit-switch circuit or
         * return sequence. Add that only after confirming the hardware.
         */
        if (is_night(&ldr))
        {
            east_west_motor(MOTOR_STOP);
            north_south_motor(MOTOR_STOP);
        }
        else
        {
            track_east_west(&ldr);
            track_north_south(&ldr);
        }

        delay_ms(TRACKING_DELAY_MS);
    }

    return 0;
}

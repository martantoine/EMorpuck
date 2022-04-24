#include <ch.h>
#include <hal.h>
#include "motors.h"
#include "leds.h"
#include "behaviors.h"

#define MOTOR_TIMER_FREQ 100000 // [Hz]
#define THRESV 650 // This is the speed under which the power save feature is active.

static const uint8_t step_halt[4] = {0, 0, 0, 0};
//table of the differents steps of to control the motors
//it corresponds to microsteps.
//in the reality two microsteps correspond to 1 step of the motor.
static const uint8_t step_table[8][4] = {
    {1, 0, 1, 0},
	{0, 0, 1, 0},
    {0, 1, 1, 0},
	{0, 1, 0, 0},
    {0, 1, 0, 1},
	{0, 0, 0, 1},
    {1, 0, 0, 1},
	{1, 0, 0, 0},
};

struct stepper_motor_pos_s {
    uint8_t step_index;
    int32_t count;  //in microsteps
    void (*update)(const uint8_t *out);
    PWMDriver *timer;
    int desired_speed;
};

struct stepper_motor_pos_s right_motor;
struct stepper_motor_pos_s left_motor;

/***************************INTERNAL FUNCTIONS************************************/

 /**
 * @brief   Updates the right motor state
 *
 * @param[in] out       pointer to the table containing the next state
 *
 */
static void right_motor_update(const uint8_t *out)
{
    out[0] ? palSetPad(GPIOE, GPIOE_MOT_R_IN1) : palClearPad(GPIOE, GPIOE_MOT_R_IN1);
    out[1] ? palSetPad(GPIOE, GPIOE_MOT_R_IN2) : palClearPad(GPIOE, GPIOE_MOT_R_IN2);
    out[2] ? palSetPad(GPIOE, GPIOE_MOT_R_IN3) : palClearPad(GPIOE, GPIOE_MOT_R_IN3);
    out[3] ? palSetPad(GPIOE, GPIOE_MOT_R_IN4) : palClearPad(GPIOE, GPIOE_MOT_R_IN4);
}

static void left_motor_update(const uint8_t *out)
{
    out[0] ? palSetPad(GPIOE, GPIOE_MOT_L_IN1) : palClearPad(GPIOE, GPIOE_MOT_L_IN1);
    out[1] ? palSetPad(GPIOE, GPIOE_MOT_L_IN2) : palClearPad(GPIOE, GPIOE_MOT_L_IN2);
    out[2] ? palSetPad(GPIOE, GPIOE_MOT_L_IN3) : palClearPad(GPIOE, GPIOE_MOT_L_IN3);
    out[3] ? palSetPad(GPIOE, GPIOE_MOT_L_IN4) : palClearPad(GPIOE, GPIOE_MOT_L_IN4);
}

static void right_motor_timer_callback(PWMDriver *gptp)
{
    (void) gptp;
    if(right_motor.count > 0) {
        if (right_motor.count < 0) {
            right_motor.step_index = (right_motor.step_index - 1) & 7;
            right_motor.update(step_table[right_motor.step_index]);
            right_motor.count -= 1;
            right_motor.step_index = right_motor.step_index;
        } else if (right_motor.count > 0) {
            right_motor.step_index = (right_motor.step_index + 1) & 7;
            right_motor.update(step_table[right_motor.step_index]);
            right_motor.count += 1;
            right_motor.step_index = right_motor.step_index;
        } else {
            right_motor.update(step_halt);
        }
    }
}

static void left_motor_timer_callback(PWMDriver *gptp)
{
    (void) gptp;
    uint8_t i;
    if(left_motor.count > 0)
    {
        if (left_motor.direction == FORWARD) { // Inverted for the two motors
            i = (left_motor.step_index + 1) & 7;
            left_motor.update(step_table[i]);
            left_motor.count -= 1;
            left_motor.step_index = i;
        } else if (left_motor.direction == BACKWARD) {
            i = (left_motor.step_index - 1) & 7;
            left_motor.update(step_table[i]);
            left_motor.count -= 1;
            left_motor.step_index = i;
        } else {
            left_motor.update(step_halt);
        }
    }
}

 /**
 * @brief   Callback that turns off the power of the right motor
 *          after a certain time to save energy. Called if power_save is enabled
 *
 * @param[in] pwmp       pointer of the PWM driver (not used)
 *
 */
static void right_motor_pwm_ch1_cb(PWMDriver *pwmp) {
	(void)pwmp;
    palClearPad(GPIOE, GPIOE_MOT_R_IN1);
    palClearPad(GPIOE, GPIOE_MOT_R_IN2);
    palClearPad(GPIOE, GPIOE_MOT_R_IN3);
    palClearPad(GPIOE, GPIOE_MOT_R_IN4);
}

static void left_motor_pwm_ch1_cb(PWMDriver *pwmp) {
	(void)pwmp;
    palClearPad(GPIOE, GPIOE_MOT_L_IN1);
    palClearPad(GPIOE, GPIOE_MOT_L_IN2);
    palClearPad(GPIOE, GPIOE_MOT_L_IN3);
    palClearPad(GPIOE, GPIOE_MOT_L_IN4);
}

void motors_init(void)
{
    /* motor struct init */
    right_motor.direction = HALT;
    right_motor.step_index = 0;
    right_motor.count = 0;
    right_motor.update = right_motor_update;
    right_motor.timer = &PWMD3;

    left_motor.direction = HALT;
    left_motor.step_index = 0;
    left_motor.count = 0;
    left_motor.update = left_motor_update;
    left_motor.timer = &PWMD4;

    /* motor init halted*/
    right_motor_update(step_halt);
    left_motor_update(step_halt);

    /* timer init */
    static const PWMConfig pwmcfg_right_motor = {
        .frequency = MOTOR_TIMER_FREQ,
        .period = 0xFFFF,
        .cr2 = 0,
        .callback = right_motor_timer_callback,
        .channels = {
            // Channel 1 is used to handling motor power save feature.
            {.mode = PWM_OUTPUT_ACTIVE_HIGH, .callback = right_motor_pwm_ch1_cb},
            {.mode = PWM_OUTPUT_DISABLED, .callback = NULL},
            {.mode = PWM_OUTPUT_DISABLED, .callback = NULL},
            {.mode = PWM_OUTPUT_DISABLED, .callback = NULL},
        },
    };
    pwmStart(&PWMD3, &pwmcfg_right_motor);
    pwmEnablePeriodicNotification(&PWMD3); // PWM general interrupt at the beginning of the period to handle motor steps.

    static const PWMConfig pwmcfg_left_motor = {
        .frequency = MOTOR_TIMER_FREQ,
        .period = 0xFFFF,
        .cr2 = 0,
        .callback = left_motor_timer_callback,
        .channels = {
            // Channel 1 is used to handling motor power save feature.
            {.mode = PWM_OUTPUT_ACTIVE_HIGH, .callback = left_motor_pwm_ch1_cb},
            {.mode = PWM_OUTPUT_DISABLED, .callback = NULL},
            {.mode = PWM_OUTPUT_DISABLED, .callback = NULL},
            {.mode = PWM_OUTPUT_DISABLED, .callback = NULL},
        },
    };
    pwmStart(&PWMD4, &pwmcfg_left_motor);
    pwmEnablePeriodicNotification(&PWMD4); // PWM general interrupt at the beginning of the period to handle motor steps.

}

void motor_set_speed(struct stepper_motor_pos_s *m, int speed)
{
   /* limit motor speed */
   if (speed > MOTOR_SPEED_LIMIT) {
       speed = MOTOR_SPEED_LIMIT;
   } else if (speed < -MOTOR_SPEED_LIMIT) {
       speed = -MOTOR_SPEED_LIMIT;
   }
   m->desired_speed = speed;
   //twice the speed because we are doing microsteps,
   //which doubles the steps necessary to do one real step of the motor
   speed *=2;

   uint16_t interval;
   if (speed == 0) {
       m->direction = HALT;
       //Resolves a problem when the motors take about 650ms to restart
       interval = 1000;    //so the motors get updated at 100Hz when not used
       } else {
       if (speed > 0) {
           m->direction = FORWARD;
       } else {
           m->direction = BACKWARD;
           speed = -speed;
       }
       interval = MOTOR_TIMER_FREQ / speed;
   }

   /* change motor step interval */
   pwmChangePeriod(m->timer, interval);
}

void left_motor_set_speed(int speed)
{
    motor_set_speed(&left_motor, speed);
}

void right_motor_set_speed(int speed)
{
    motor_set_speed(&right_motor, speed);
}

void right_motor_move(int steps)
{
    if(steps > 0) {
        right_motor.direction = FORWARD;
        right_motor.count += 2*steps;
    }
    else if(steps < 0) {
        right_motor.direction = BACKWARD;
        right_motor.count -= 2*steps;
    }
    else
        right_motor.direction = HALT;
}
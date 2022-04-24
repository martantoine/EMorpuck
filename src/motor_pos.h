#ifndef MOTORS_POS_H
#define MOTORS_POS_H

#include <stdint.h>
#include <hal.h>

#define MOTOR_SPEED_LIMIT 1100 // [step/s]

extern struct stepper_motor_pos_s right_motor;
extern struct stepper_motor_pos_s left_motor;

void motors_init(void);
void motor_set_speed(struct stepper_motor_pos_s *m, int speed);
void left_motor_set_speed(int speed);
void right_motor_set_speed(int speed);
void left_motor_move(int steps);
void right_motor_move(int steps);

#endif /* MOTOR_H */

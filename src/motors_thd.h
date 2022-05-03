/**
 * @file motors_thd.h
 * @author Antoine Martin (martantoine@tuta.io)
 * @brief 
 * @version 0.1
 * @date 2022-04-23
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef MOTORS_THD_H
#define MOTORS_THD_H

void readpath(void);
void motors_thd_init(void);
thread_reference_t* motor_get_trp(void);

#endif /*MOTORS_THD_H*/

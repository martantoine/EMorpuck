/**
 * @file distance.h
 * @author Alexandre Duval(alduval0305@gmail.com)
 * @brief
 * @vesion 0.1
 * @date 2022-04-26
 *
 * @copyright Copyright (c) 2022
 *
 */
#ifndef DISTANCE_H
#define DISTANCE_H

#include "typedef.h"

void tof_init(void);
distnorm_t tof_measure(void);

#endif /*DISTANCE_H*/
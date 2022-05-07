#ifndef MAIN_H
#define MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdint.h>
#include <ch.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include <hal.h>
#include <memory_protection.h>
#include <usbcfg.h>
#include <chprintf.h>
#include "shared_var.h"
#include "msgbus/messagebus.h"
#include "parameter/parameter.h"
#include "infra.h"
#include "camera/dcmi_camera.h"
#include "color.h"
#include "distance.h"
/** Robot wide IPC bus. */

extern messagebus_t bus;
extern parameter_namespace_t parameter_root;

#ifdef __cplusplus
}
#endif

#endif

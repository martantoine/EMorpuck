#include <ch.h>
#include "motors_thd.h"
#include "shared_var.h"

static void inline updatePosition(); //inline because used only in one place
static void calibrationPosition();

static THD_WORKING_AREA(waMotors, 128);
static THD_FUNCTION(Motors, arg)
{
    for(;;) // equivalent to while(true) but a bit more optimized
    {
        chSemWait(&position_s);
        chSemWait(&path_s);
        if(path != NULL)
        {
            while(*path != STOP)
            {
                switch(*path)
                {
                    case FORWARD: 
                        forward();
                        break; 
                    case BACKWARD:
                        backward();
                        break; 
                    case LEFT: 
                        left();
                        break; 
                    case RIGHT:
                        right();
                        break;
                }
                updatePosition();
                path += sizeof(step_t);
            }
            free(path);
        }
        chSemSignal(&position_s);
        chSemSignal(&path_s);
        chThdSuspend();
    }
}

static void updatePosition()
{
    uint8_t delta = 0;

    if(position_t == (E || W))
        position_x += (*path == FORWARD) ? 1 : -1;
    else if(position_t == (N || S))
        position_y += (*path == FORWARD) ? 1 : -1;
    
    if(*path == LEFT)
        position_t = (position_t + 1) % 8;
    else if(*path == RIGHT)
        position_t = (position_t - 1) % 8;
}

void motors_thd_init()
{
    chThdCreateStatic(waMotors, sizeof(waMotors), NORMALPRIO, Motors, NULL);    
}

void calibrationPosition()
{
    /*
    Do some obscure things to align the robot with the game map
    */
}
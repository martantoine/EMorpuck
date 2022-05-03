#include <stdlib.h>
#include <ch.h>
#include "motors_thd.h"
#include "motors_driver.h"
#include "movement.h"
#include "shared_var.h"

static thread_reference_t motors_trp = NULL;

inline static void updatePosition(step_t* step); //inline because used only in one place
static void calibrationPosition(void);

/*
static THD_WORKING_AREA(waMotors, 256);
static THD_FUNCTION(Motors, arg)
{   
    chRegSetThreadName(__FUNCTION__);
    (void)arg;
    chSysInit();
    for(;;) // equivalent to while(true) but a bit more optimized
    {
        chSemWait(&position_s);
        chSemWait(&path_s);
        if(path != NULL)
        {
            chThdSetPriority(NORMALPRIO+1);
            uint16_t i = 0;
            while(*(path+i*sizeof(step_t)) != STOP)
            {
                while((left_motor_steps_left() != 0) || (right_motor_steps_left() != 0));
                switch(*(path+i*sizeof(step_t)))
                {
                    case FORWARD: 
                        mvt_forward();
                        break;
                    case BACKWARD:
                        mvt_backward();
                        break;
                    case LEFT: 
                        mvt_left();
                        break;
                    case RIGHT:
                        mvt_right();
                        break;
                    default:
                        break;
                }
                updatePosition(path+i*sizeof(step_t));
                i++;
                //chThdSleepSeconds(1);
            }
        }
        chSemSignal(&position_s);
        chSemSignal(&path_s);
        chThdSetPriority(NORMALPRIO-1);

        //chSysLock();
        //chThdSuspendS(&motors_trp);
        //chSysUnlock();
    }
}
*/
void readpath(void)
{
    if(path != NULL)
    {
        uint16_t i = 0;
        while(*(path+i*sizeof(step_t)) != STOP)
        {/*
            switch(*(path+i*sizeof(step_t)))
            {
                case FORWARD: 
                    mvt_forward();
                    break;
                case BACKWARD:
                    mvt_backward();
                    break;
                case LEFT: 
                    mvt_left();
                    break;
                case RIGHT:
                    mvt_right();
                    break;
                default:
                    break;
            }*/
            updatePosition(path+i*sizeof(step_t));
            i++;
        }
    }
}


thread_reference_t* motor_get_trp(void)
{
    return &motors_trp;
}

static void updatePosition(step_t* step)
{
    int16_t delta = 0;
    if(*step == FORWARD)
        delta = 1;
    else if(*step == BACKWARD)
        delta = -1;

    if(      ((*step == FORWARD) || (*step == BACKWARD)) && (position_t == E))
        position_x += delta;
    else if( ((*step == FORWARD) || (*step == BACKWARD)) && (position_t == W))
        position_x -= delta;
    else if( ((*step == FORWARD) || (*step == BACKWARD)) && (position_t == N))
        position_y -= delta;
    else if( ((*step == FORWARD) || (*step == BACKWARD)) && (position_t == S))
        position_y += delta;

    if(*step == LEFT) {
        switch(position_t) {
            case E: position_t = N; break;
            case N: position_t = W; break;
            case W: position_t = S; break;
            case S: position_t = E; break;
        }
    }
    else if(*step == RIGHT) {
        switch(position_t) {
            case E: position_t = S; break;
            case N: position_t = E; break;
            case W: position_t = N; break;
            case S: position_t = W; break;
        }
    }/*
    if(*step == LEFT)
        position_t = (position_t + 2 - 10) % 8 + 10;
    else if(*step == RIGHT)
        position_t = (position_t - 2 - 10) % 8 + 10;*/
}

void motors_thd_init(void)
{
    motors_init();
    right_motor_set_speed(400);
    left_motor_set_speed(400);
    //chThdCreateStatic(waMotors, sizeof(waMotors), NORMALPRIO-1, Motors, NULL);    
    calibrationPosition();
}

void calibrationPosition(void)
{
    /*
    Do some obscure things to align the robot with the game map
    */
}
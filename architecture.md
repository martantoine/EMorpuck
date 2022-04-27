# Shared data
current_x, current_y, current_t

# Modules
## scan
### SHARED VARIABLES

### FUNCS
func initGameMap()
func ScanMap()
func Cell2Check()

## Path
node_t** nodeMap
nodeMap[i][j] = malloc(sizeof(node_t))
*nodeMap[i][j] = 
type enum step = { x, y, t }
func pathFinding(target_x, target_y, target_t)
atomic movement : 1/4 of a cell

## motors_thd
### SHARED VARIABLES
semaphore_t step* path      // pointer array of next step to reach target
semaphore_t x, y, t         // current position (x,y,t=theta)

### FUNCS
func motors_thd
    while(true)
        chSemWait(path_s)
        chSemWait(position_s)
        if(path != nullptr)
            while(*path != STOP)
                switch *path
                    case FORWARD :  forward();
                    case BACKWARD : backwartd();
                    case LEFT :     left;
                    case RIGHT :    right();
                updatePosition() // update x, y, t corresponding to the last step executed
                path++
                if(path == nullptr) // in case of corrupted data
                    break
            free(path)
        chThdSuspend(path_s) // suspend the thread until another thread wakes it up
        
## movements
### SHARED VARIABLES

### FUNCS
func zero()
    place approximately the robot on the zero cell, the camera & TOF sensors will then be used to deduce the precise location of the robot
    then  
func forward() : move 1/4 of a cell width forward
func backward() : move 1/4 of a cell width backward
func left() : turn 45° left
func right() : turn 45° right

## motors_position
### FUNCS
func motors_init
func motor_set_speed

func right_motor_update
func right_motor_timer_callback()
func right_motor_pwm_ch1_cb()
func right_motor_enable_power_save()
func right_motor_disable_power_save()
func right_motor_set_speed()
func right_motor_move()
func right_motor_stop()

func left_motor_update
func left_motor_timer_callback()
func left_motor_pwm_ch1_cb()
func left_motor_enable_power_save()
func left_motor_disable_power_save()
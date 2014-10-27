/*********************************************
 *********************************************
 *
 *  Project: Penn
 *  Script: timers.c
 *  Authors: Mike Lautman
 *********************************************
 *********************************************
 */

#import "timers.h"
////////////////////////////////////////////////
// SET_LOOP_SPEED
//
// Functionality :
//   - setup main timer (timer_0) to run at loop_frequency
//
// Parameters
//   - frequency desired as an integer. (1000 -> 1khz)
//
// Returns
//   - success or failure
bool set_loop_speed(int freq){
    //TODO
    return false;
}


////////////////////////////////////////////////
// LOOP_READY
//
// Functionality:
//   - Checks the loop timer (timer_0) to see if
//      1/1000 seconds have passed since last flag
//
// Parameters
//
// Returns
//   - ready or not as boolean
bool loop_ready(void){
    //TODO
    return false;
}


////////////////////////////////////////////////
// CONFIG_STOPWATCH
//
// Functionality:
//   - setup timer (timer_3) to time things
//
// Parameters
//   - frequency desired as an integer. (1000 -> 1khz)
//
// Returns
//   - success or failure
bool config_stopWatch(){
    //TODO
    return false;

}


////////////////////////////////////////////////
// CLEAR_STOPWATCH
//
// Functionality:
//   - clear timer (timer_3) counter to 0
//
// Parameters:
//
// Returns
//   - success or failure
bool clear_stopWatch(){
    //TODO
    return false;
}



////////////////////////////////////////////////
// STOPWATCH_NOW
//
// Functionality:
//   - get the time since last stopWatch clear
//
// Parameters:
//
// Returns
//   - unsigned long (NEEDS UNITS!!!)
unsigned long stopWatch_now(void){
    //TODO
    return 1L;
}


////////////////////////////////////////////////
// STOPWATCH_GETDELTA
//
// Functionality:
//   - get difference between start and stopWatch_now
//
// Parameters:
//   - unsigned long start  : deltaStart
//
// Returns
//   - time delta : stopWatch_now - deltaStart
unsigned long stopWatch_getDelta(unsigned long deltaStart){
    return 1L;
}






/*********************************************
 *********************************************
 *
 *	Project: Penn
 * 	Script: timers.h
 *  Authors: Mike Lautman
 *********************************************
 *********************************************
 */

 #ifndef __TIMERS__PENN__
 #define __TIMERS__PENN__

////////////////////////////////////////////////
// SET_LOOP_SPEED
//
// Functionality :
//   - setup main timer (timer_3) to run at loop_frequency
//
// Parameters
//   - frequency desired as an integer. (1000 -> 1khz)
//
// Returns
//   - success or failure
bool set_loop_speed(int freq);


////////////////////////////////////////////////
// LOOP_READY
//
// Functionality:
//   - Checks the loop timer (timer_3) to see if
//      1/1000 seconds have passed since last flag
//
// Parameters
//
// Returns
//   - ready or not as boolean
bool loop_ready(void);


////////////////////////////////////////////////
// CONFIG_STOPWATCH
//
// Functionality:
//   - setup timer (timer_4) to time things
//
// Parameters
//   - frequency desired as an integer. (1000 -> 1khz)
//
// Returns
//   - success or failure
bool config_stopWatch();


////////////////////////////////////////////////
// CLEAR_STOPWATCH
//
// Functionality:
//   - clear timer (timer_4) counter to 0
//
// Parameters:
//
// Returns
//   - success or failure
bool clear_stopWatch();



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
unsigned long stopWatch_now(void);



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
unsigned long stopWatch_getDelta(unsigned long deltaStart);

#endif



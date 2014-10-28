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

#import "m_general.h"


////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////
//
// LOOP Stuff
//
////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////
// LOOP_ISSET
//
// Functionality :
//   - check to see if the loop is set and running
//
// Parameters
//   -
//
// Returns
//   - success or failure
bool loop_isSet(void);


////////////////////////////////////////////////
// SET_LOOP
//
// Functionality :
//   - setup main timer (timer_3) to run at loop_frequency
//
// Parameters
//   - frequency desired as an integer. (1000 -> 1khz)
//
// Returns
//   - success or failure
bool set_loop_speed(int16_t freq);


////////////////////////////////////////////////
// LOOP_ISSET
//
// Functionality :
//   - check to see if the loop is set and running
//
// Parameters
//   -
//
// Returns
//   - success or failure
bool loop_isSet(void);


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




////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////
//
// STOPWATCH Stuff
//
////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////
// STOPWATCH_ISSET
//
// Functionality :
//   - check to see if the loop is set and running
//
// Parameters
//   -
//
// Returns
//   - success or failure
bool stopwatch_isSet(void);


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
bool config_stopWatch(void);


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
bool clear_stopWatch(void);



////////////////////////////////////////////////
// STOPWATCH_NOW
//
// Functionality:
//   - get the time since last stopWatch clear
//
// Parameters:
//
// Returns
//   - uint32_t (NEEDS UNITS!!!)
uint32_t stopWatch_now(void);



////////////////////////////////////////////////
// STOPWATCH_GETDELTA
//
// Functionality:
//   - get difference between start and stopWatch_now
//
// Parameters:
//   - uint32_t start  : deltaStart
//
// Returns
//   - time delta : stopWatch_now - deltaStart
uint32_t stopWatch_getDelta(uint32_t deltaStart);

#endif



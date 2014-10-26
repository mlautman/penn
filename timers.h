/*********************************************
 *********************************************
 *
 *	Project: Penn
 * 	Script: timers.h
 *  Authors: Mike Lautman
 *********************************************
 *********************************************
 */


bool setup_loop_speed(int freq);

bool setup_timer();

bool timer_clear();

unsigned long timer_now();

unsigned long timer_delta(unsigned long start);
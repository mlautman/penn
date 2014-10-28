/*********************************************
 *********************************************
 *
 *	Project: Penn
 * 	Script: penn.c
 *  Author: Mike Lautman
 *********************************************
 *********************************************
 */

#include "penn.h"

char* receive_buffer;


volatile bool new_packet = false;

bool setup(void){
	// setup main loop timer
	return false;
}

int i;
void run(){
	i++;
}

int main(void) {
	// Variables
	bool usb_debug = true;

	// Setup
	bool setup_success = setup();

	// Run
	while(1){
		run();
	}
}


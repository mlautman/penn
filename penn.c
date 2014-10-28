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
#include "m_usb.h"

char* receive_buffer;


volatile bool new_packet = false;
uint16_t loop_freq = 100;
uint8_t receiver_addr = 0x24;
uint8_t receiver_chan = 1;
bool debug_activate = true;

uint8_t setup(){
	// setup main loop timer
	bool loop_set = set_loop_speed(loop_freq);
	bool stopWatch_set = config_stopWatch();
	bool imu_set = imu_init();
	bool rf_set = init_wireless(receiver_addr, receiver_chan);
	bool usb_set = usb_debug(debug_activate);

	uint8_t setup_mask = 0;
	setup_mask += (loop_set ? 1:0)		<<0;
	// setup_mask += (stopWatch_set ? 1:0) <<1;
	// setup_mask += (imu_set ? 1:0) 		<<2;
	// setup_mask += (rf_set ? 1:0) 		<<3;
	// setup_mask += (usb_set ? 1:0)	 	<<4;

	return setup_mask;
}

unsigned long cnt =0;
int i;
void run(){
	if(loop_ready()){
		m_usb_tx_string("ready\n\r");
		m_green(2);
	}
}

int main(void) {
	// Setup
	uint8_t setup_success = setup();
	// Run
	while(1){
		run();
	}
}


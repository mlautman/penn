/*********************************************
 *********************************************
 *
 *	Project: Penn
 * 	Script: receive.c
 *  Author: Mike Lautman
 *********************************************
*********************************************
*/



#include "receive.h"

int8_t* receive_buffer;
int i = 1;

volatile bool __new_packet = false;
int8_t rf_packet[ 1 + 1 + 2 + 9*2] = {0};


// Wireless packet length (refer to notes)
// int16_t rf_packet_len = 17;
// int8_t rf_pen_addr = 0x24;

// // Wireless packet types
// int8_t rf_packet_test = 0;
// int8_t rf_packet_IMU = 1;


uint8_t setup(){
	// setup main loop timer
	bool loop_set = set_loop_speed();
	bool stopWatch_set = config_stopWatch();
    int8_t rf_base_addr = 0x11;
    uint8_t receiver_chan = 1;
	bool rf_set =  init_wireless(rf_base_addr, receiver_chan);
	bool usb_set = usb_debug(true);

	uint8_t setup_mask = 0;
	setup_mask += (loop_set ? 1:0)		<<0;
	setup_mask += (stopWatch_set ? 1:0) <<1;
	setup_mask += (rf_set ? 1:0) 		<<3;
	setup_mask += (usb_set ? 1:0)	 	<<4;

	sei();
	return setup_mask;
}

uint32_t cnt =0;
int16_t i;
void run(){
	if(loop_ready()){
		m_green(2);
        if(__new_packet){
            __new_packet = false;
            read_packet(rf_packet);
            usb_debug_rf_data(rf_packet, 18);
        }
	}
}

int16_t main(void) {
	// Setup
	setup();
	// Run
	while(1){
		run();
	}
}


ISR (INT2_vect) {
	__new_packet = true;
}


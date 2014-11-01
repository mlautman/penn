/*********************************************
 *********************************************
 *
 *  Project: Penn
 *  Script: penn.c
 *  Author: Mike Lautman
 *********************************************
*********************************************
*/

#include "penn.h"
#include "m_usb.h"

int8_t* receive_buffer;


volatile bool new_packet = false;

bool debug_activate = false;
int8_t rf_packet[ 1 + 1 + 2 + 9*2] = {0};
int8_t imuData_char[9*2] = {0};


// Wireless packet length (refer to notes)
int8_t rf_base_addr = 0x11;
int8_t rf_pen_addr = 0x24;
uint8_t receiver_chan = 1;


// Wireless packet types
int8_t rf_packet_test = 0;
int8_t rf_packet_IMU = 1;

uint8_t setup(){
    // setup main loop timer
    bool loop_set = set_loop_speed();
    bool stopWatch_set = config_stopWatch();
    bool imu_set = imu_init();
    bool rf_set = init_wireless(rf_pen_addr, receiver_chan);
    bool usb_set = usb_debug(debug_activate);

    uint8_t setup_mask = 0;
    setup_mask += (loop_set ? 1:0)      <<0;
    setup_mask += (stopWatch_set ? 1:0) <<1;
    setup_mask += (imu_set ? 1:0)       <<2;
    setup_mask += (rf_set ? 1:0)        <<3;
    setup_mask += (usb_set ? 1:0)       <<4;

    sei();
    return setup_mask;
}

uint32_t cnt =0;
int16_t i;
void run(){
    if(loop_ready()){
        // cli();
        m_green(2);
        imu_rawData_get((int16_t*)imuData_char);
        uint32_t now = stopWatch_now();
        if (now  >= 100000 ){
            clear_stopWatch();
        }
        // usb_debug_rf_data( 1 , now, imuData_char, 17);
        send_packet(0, now, imuData_char, 17);
        // usb_debug_imu_tx((int16_t*)imuData_char, 9);
        // usb_debug_stopWatch(now);
    }
}

int16_t main(void) {
    // Setup
    uint8_t setup_success = setup();
    // Run
    while(1){
        run();
    }
}


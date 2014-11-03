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

bool debug_activate = true;
int8_t rf_packet[ 1 + 1 + 2 + 9*2] = {0};
int8_t imuData_char[9*2] = {0};
bool wifi_test = 0;


// Wireless packet length (refer to notes)
int8_t rf_base_addr = 0x11;
int8_t rf_pen_addr = 0x24;
uint8_t receiver_chan = 1;


// Wireless packet types
int8_t rf_packet_test = 0;
int8_t rf_packet_IMU = 1;

// Button for the pen
bool setup_button(void){
    clear(DDRD, 7);
    return true;
}

char check_button(void){
    return (check(PIND, 7) ? 1 : 0);
}

uint8_t setup(){
    // setup main loop timer
    bool loop_set = set_loop_speed();
    bool stopWatch_set = config_stopWatch();
    bool imu_set = imu_init();
    bool rf_set = init_wireless(rf_pen_addr, receiver_chan);
    bool usb_set = usb_debug(debug_activate);
    bool button_set = setup_button();

    uint8_t setup_mask = 0;
    setup_mask += (loop_set ? 1:0)      <<0;
    setup_mask += (stopWatch_set ? 1:0) <<1;
    setup_mask += (imu_set ? 1:0)       <<2;
    setup_mask += (rf_set ? 1:0)        <<3;
    setup_mask += (usb_set ? 1:0)       <<4;
    setup_mask += (button_set ? 1:0)    <<5;


    sei();
    return setup_mask;
}

uint32_t cnt = 0;
char last_button = 0;
char button = 0;
uint32_t now = 0 ;
void run(){
    if(loop_ready()){
        // cli();
        m_green(2);
        imu_rawData_get((int16_t*)imuData_char);
        button = check_button();

        if ( button == 1){
            m_red(OFF);
            if (last_button == 0){
                // clear_smak topWatch();
            }
            now = stopWatch_now();
            send_packet(0, now, imuData_char, 18, button);
            wifi_test = usb_tx_data(1, now, imuData_char, 6 , button);
        } else{ // button == 0
            now = stopWatch_now();
            wifi_test = usb_tx_data(0, now, imuData_char, 6 , button);
            if (last_button == 1){
//                 now = stopWatch_now();
//                 // send one last packet as a stop
// //                wifi_test = send_packet(0, now, imuData_char, 18, button);
//                 wifi_test = usb_tx_data(0, now, imuData_char, 6 , button);
                // if (wifi_test){
                    m_red(ON);
                // }

            }
        }
        last_button = button;
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


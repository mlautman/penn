/*********************************************
 *********************************************
 *
 *  Project: Penn
 *  Script: usb_debug.c
 *  Authors: Mike Lautman
 *********************************************
 *********************************************
 */

#include "usb_debug.h"

////////////////////////////////////////////////
// USB_DEBUG
//
// Functionality :
//   - set whether usb debugger is used
//
// Parameters
//   - bool activate : whether or not to use usb debugger
//
// Returns
//   - usb_debug state
bool usb_debug(bool activate){
    if (activate){
        m_red(ON);
        m_usb_init();
        while(!m_usb_isconnected());
        bool isConn = m_usb_isconnected();
        return isConn;
        m_red(OFF);
    } else {
        return FALSE;
    }
}


////////////////////////////////////////////////
// USB_DEBUG_send_setupMask
//
// Functionality :
//   - send success or failure of setup ops muxed into single value
//  [
//      wireless_isConnected,
//      imu_init,
//      loop_timer,
//      stopWatch_timer,
//      N/A,
//      N/A,
//      N/A,
//      N/A,
//  ]
//
// Parameters
//   - char setup_mask  : mask for setup success/fail bits
//
// Returns
//   - success/failure of transmission

bool usb_debug_send_setupMask(void){
    unsigned char wireless_isConnected = (test_connection()?1:0);
    unsigned char imu_init = (imu_isInit()?1:0);
    unsigned char loop_timer_running = loop_isSet()?1:0;
    unsigned char stopWatch_running = stopwatch_isSet()?1:0;

    unsigned char setupMask = 0;
    setupMask += (wireless_isConnected ? 1:0);
    setupMask += (imu_init ? 1:0)<<1;
    setupMask += (loop_timer_running ? 1:0)<<2;
    setupMask += (stopWatch_running ? 1:0)<<3;
    m_usb_tx_char(setupMask);
    return setupMask;
}



////////////////////////////////////////////////
// USB_DEBUG_IMU_TX
//
// Functionality :
//   - Sends Imu values over usb instead of wireless
//
// Parameters
//   - int* imu_data
//   - char dataLen : lenght of rf_data
//
// Returns
//   - success/failure of transmission
bool usb_debug_imu_tx(int* imu_data, char dataLen){
    m_usb_tx_string("IMU data:");
    bool txSuccess = true;

    int i;
    for(i = 0; i < dataLen; i++){
        txSuccess &= m_usb_tx_char('\t');
        m_usb_tx_int(imu_data[i]);
    }
    m_usb_tx_string("\n\r");
    return txSuccess;
}


////////////////////////////////////////////////
// USB_DEBUG_RF_TX
//
// Functionality :
//   - Sends rf packets values over usb instead of wireless
//
// Parameters
//   - char* rf_data : data that would normally be sent over rf
//   - char dataLen : lenght of rf_data
//
// Returns
//   - success/failure of transmission
int rf_packet_len_debug = 17;
bool usb_debug_rf_data(char packet_type, unsigned long time_stamp, char* data, unsigned int data_len){
    m_usb_tx_string("rf_data:");

    char toSend [rf_packet_len_debug];
    toSend[0] = packet_type;
    int ii;
    for (ii=0; ii<4; ii++) {
        toSend[ii+1] = ((char*)&time_stamp)[ii]; //assumes long is 4 bytes
    }
    if (data_len > (rf_packet_len_debug - 5)) {
        data_len = rf_packet_len_debug - 5; //TODO: FIX THIS SILENT FAILURE
    }
    for (ii=0;ii<data_len;ii++) {
        toSend[ii+5] = data[ii];
    }


    bool txSuccess = true;
    int i;
    m_usb_tx_int((int)toSend[0]);
    m_usb_tx_string("\t");
    m_usb_tx_long((unsigned long)((unsigned long*)&toSend[1])[0]);
    for(i = 5; i < rf_packet_len_debug; i+=2){
        txSuccess &= m_usb_tx_char('\t');
        m_usb_tx_int((int)((int*)&toSend[i])[0]);
    }
    m_usb_tx_string("\n\r");
    return txSuccess;
}


////////////////////////////////////////////////
// USB_DEBUG_RF_DROP_COUNT
//
// Functionality :
//   - Sends number of dropped packets
//
// Parameters
//   - unsigned int drop_cnt : number of dropped packets as cnted by wireless
//
// Returns
//   - success/failure of transmission
bool usb_debug_rf_drop_count(unsigned int drop_cnt){
    // TODO
    return false;
}



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
//   - int8_t setup_mask  : mask for setup success/fail bits
//
// Returns
//   - success/failure of transmission

bool usb_debug_send_setupMask(void){
    uint8_t wireless_isConnected = (test_connection()?1:0);
    uint8_t imu_init = 1;
    uint8_t loop_timer_running = loop_isSet()?1:0;
    uint8_t stopWatch_running = stopwatch_isSet()?1:0;

    uint8_t setupMask = 0;
    setupMask += (wireless_isConnected ? 1:0);
    setupMask += (imu_init ? 1:0)<<1;
    setupMask += (loop_timer_running ? 1:0)<<2;
    setupMask += (stopWatch_running ? 1:0)<<3;
    m_usb_tx_int(setupMask);
    return setupMask;
}



////////////////////////////////////////////////
// USB_DEBUG_IMU_TX
//
// Functionality :
//   - Sends Imu values over usb instead of wireless
//
// Parameters
//   - int16_t* imu_data
//   - int8_t dataLen : lenght of rf_data
//
// Returns
//   - success/failure of transmission
bool usb_debug_imu_tx(int16_t* imu_data, int8_t dataLen){
    m_usb_tx_string("IMU data:\t");
    bool txSuccess = true;

    int16_t i;
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
//   - int8_t* rf_data : data that would normally be sent over rf
//   - int8_t dataLen : lenght of rf_data
//
// Returns
//   - success/failure of transmission
int16_t rf_packet_len_debug = 18;
bool usb_debug_rf_data(uint8_t* data, int8_t dataLen){
    if(data[0]==0){
        // m_usb_tx_string("rf_data");
        // debug rf_packet

        m_usb_tx_int((int16_t)data[0]);
        m_usb_tx_char('\t');

        uint32_t now_32 = 0;
        now_32 += (uint32_t)data[1] << 24;
        now_32 += (uint32_t)data[2] << 16;
        now_32 += (uint32_t)data[3] << 8;
        now_32 += (uint32_t)data[4];

        m_usb_tx_ulong((unsigned long)now_32);
        m_usb_tx_char('\t');


        int8_t i;
        for(i = 5; i < rf_packet_len_debug - 1; i+=2){
            m_usb_tx_int(*(int16_t*)&data[i]);
            m_usb_tx_char('\t');
        }
        m_usb_tx_int((int16_t)data[17]);
        bool txSuccess = m_usb_tx_char('\t');
        m_usb_tx_string("\n\r");

        return txSuccess != -1;
    }

    return true ;
}


////////////////////////////////////////////////
// USB_DEBUG_RF_DROP_COUNT
//
// Functionality :
//   - Sends number of dropped packets
//
// Parameters
//   - uint16_t drop_cnt : number of dropped packets as cnted by wireless
//
// Returns
//   - success/failure of transmission
bool usb_debug_rf_drop_count(uint16_t drop_cnt){
    // TODO
    return false;
}


///////////////////////////////////////////////
// USB_DEBUG_STOPWATCH
//
// Functionality :
//   - Sends stopWatch value
//
// Parameters
//   - uint16_t now : stopWatch cnt value
//
// Returns
//   - success/failure of transmission
bool usb_debug_stopWatch(uint32_t now){
    m_usb_tx_string("stopwatch_now:\t");
    m_usb_tx_ulong(now);
    m_usb_tx_string("\n\r");
    return true;
}



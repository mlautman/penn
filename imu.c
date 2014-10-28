/*********************************************
 *********************************************
 *
 *  Project: Penn
 *  Script: imu.c
 *  Authors: Mike Lautman
 *
 *********************************************
 *********************************************
*/
 #include "imu.h"
 #include "m_imu.h"


bool imu_isInitialized = false;

////////////////////////////////////////////////
// IMU_ISINIT
//
// Functionality :
//   - check to see if the imu is setup
//
// Parameters
//   -
//
// Returns
//   - success or failure
bool imu_isInit(void){
    return imu_isInitialized;
}


////////////////////////////////////////////////
// IMU_ISINIT
//
// Functionality :
//   - imu setup
//
// Parameters
//   -
//
// Returns
//   - success or failure
bool imu_init(void){
    unsigned char accel_scale=0;
    unsigned char gyro_scale=0;
    imu_isInitialized = m_imu_init(accel_scale,gyro_scale)
    return imu_isInitialized;
}


////////////////////////////////////////////////
// IMU_rawData_get
//
// Functionality :
//   - check to see if the imu is setup
//
// Parameters
//   - char data[9] : pointer to array where data will be stored
//
// Returns
//   - success or failure
bool imu_rawData_get(char* data){
    return m_imu_raw(char* data);
}
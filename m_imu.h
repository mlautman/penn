// -----------------------------------------------------------------------------
// M2 9-DOF IMU interface
// version: 2.0
// date: Jun 16, 2012
// author: J. Fiene
// -----------------------------------------------------------------------------

#ifndef m_imu__
#define m_imu__

#include "m_general.h"
#include "m_bus.h"

// -----------------------------------------------------------------------------
// Public functions:
// -----------------------------------------------------------------------------

unsigned char m_imu_test(unsigned char* raw_data);

unsigned char m_imu_init(unsigned char accel_scale, unsigned char gyro_scale);
// FUNCTIONALITY
// initialize the 9-DOF mIMU board
//
// TAKES:
// accel_scale: 0 = +/-2G, 1 = +/-4G, 2 = +/-8G, 3 = +/-16G
// gyro_scale: 0 = +/-250d/s, 1 = +/-500d/s, 2 = +/-1000d/s, 3 = +/-2000d/s
//
// RETURNS:
// 1 : success
// 0 : communication error

unsigned char m_imu_raw(int* raw_data);
// FUNCTIONALITY
// places the raw sensor data into a pre-allocated 9-element int array as
// [a_x, a_y, a_z, g_x, g_y, g_z, m_x, m_y, m_z]
//
// TAKES:
// raw_data : (pointer to the first element of a nine-element int array)
//
// RETURNS:
// 1 : success
// 0 : communication error

unsigned char m_imu_accel(int* raw_data);
// FUNCTIONALITY
// places the raw accelerometer data into a pre-allocated 3-element int array as
// [a_x, a_y, a_z]
//
// TAKES:
// raw_data : (pointer to the first element of a three-element int array)
//
// RETURNS:
// 1 : success
// 0 : communication error

unsigned char m_imu_gyro(int* raw_data);
// FUNCTIONALITY
// places the raw gyroscope data into a pre-allocated 3-element int array as
// [g_x, g_y, g_z]
//
// TAKES:
// raw_data : (pointer to the first element of a three-element int array)
//
// RETURNS:
// 1 : success
// 0 : communication error

unsigned char m_imu_mag(int* raw_data);
// FUNCTIONALITY
// places the raw magnetometer data into a pre-allocated 3-element int array as
// [m_x, m_y, m_z]
//
// TAKES:
// raw_data : (pointer to the first element of a three-element int array)
//
// RETURNS:
// 1 : success
// 0 : communication error

#endif
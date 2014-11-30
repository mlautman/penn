# Module imports
import numpy as np

#Class imports
from scipy import ndimage

# function imports

from numpy import interp

def interpolate(Y, return_arr_length):
    dummy = np.ones(return_arr_length)
    interp(dummy, )

def create_feature_vect(motion_data, interpolation_len):
    """
    Create a single vector of features
    [
        interpXacc, mean_Xacc, std_Xacc, rms_Xacc, (interp_len+3 long)
        interpYacc, mean_Yacc, std_Yacc, rms_Yacc, (interp_len+3 long)
        interpZacc, mean_Zacc, std_Zacc, rms_Zacc, (interp_len+3 long)

        interpXvel, mean_Xvel, std_Xvel, rms_Xvel, (interp_len+3 long)
        interpYvel, mean_Yvel, std_Yvel, rms_Yvel, (interp_len+3 long)
        interpZvel, mean_Zvel, std_Zvel, rms_Zvel, (interp_len+3 long)

        interpXpos, mean_Xpos, std_Xpos, rms_Xpos, (interp_len+3 long)
        interpYpos, mean_Ypos, std_Ypos, rms_Ypos, (interp_len+3 long)
        interpZpos, mean_Zpos, std_Zpos, rms_Zpos, (interp_len+3 long)

        interpXgyro, mean_Xgyro, std_Xgyro, rms_Xgyro, (interp_len+3 long)
        interpYgyro, mean_Ygyro, std_Ygyro, rms_Ygyro, (interp_len+3 long)
        interpZgyro, mean_Zgyro, std_Zgyro, rms_Zgyro, (interp_len+3 long)

        interpXang, mean_Xang, std_Xang, rms_Xang, (interp_len+3 long)
        interpYang, mean_Yang, std_Yang, rms_Yang, (interp_len+3 long)
        interpZang, mean_Zang, std_Zang, rms_Zang, (interp_len+3 long)
        record_time, (1 long)

    ]

        XYZ
            accel
            vel
            pos
            gyro
            ang
                mean
                std
                rms


    input:
        motion_data : Transformed Acc, vel, pos, rot rate and angle values
            k x 15 array.

        interpolation_len : An integer value representing target resolution in sensor readings.
    output:
        feature : (interpolation_len + 3)* 15 + 1

    """

    # grab the record time
    record_time = motion_data.shape[0]

    # dictionaries are used to hold the agg stat values
    mean = {}
    std = {}
    rms = {}

    # mean, std, rms
    num_agg_stat_vals = 3
    # |[X, Y, Z]| * |[acc, vel, pos, rot, ang]|
    num_dimensions = 5

    block_size = interpolation_len + num_agg_stat_vals

    # preallocate output array
    out = np.zeros(
        num_dimensions * (
            interpolation_len + num_agg_stat_vals
        ) + 1
    )

    # first grab the agg stat values for each of the 15 motion values
    for i in range(15):
        start_block = i * block_size
        end_block = start_block + block_size
        # MEAN
        mean[i] = np.mean(motion_data[:,i])
        # STD
        std[i] = np.std(motion_data[:,i])
        # RMS
        sq_dist = motion_data[:,i] ** 2
        mean_sq_dist = np.mean(sq_dist)
        rms[i] = np.sqrt(mean_sq_dist)

        # resized

    out[-1] = record_time







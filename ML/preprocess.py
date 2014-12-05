from __future__ import division

# Module imports
import numpy as np

#Class imports

# function imports
from scipy.interpolate import InterpolatedUnivariateSpline
# import matplotlib.pyplot as plt

NUM_AGG_STAT_VALS = 3


def interpolate(y, return_arr_length):
    x = np.linspace(0, 1, len(y))
    xi = np.linspace(0, 1, return_arr_length)
    ius = InterpolatedUnivariateSpline(x, y)
    yi = ius(xi)
    return yi

def normalize(x, mean, std):
    return (x-mean)/std

def feature_vect_size(motion_data, interpolation_len):
    num_dimensions = motion_data.shape[1]
    return num_dimensions * (interpolation_len + NUM_AGG_STAT_VALS ) + 1



def create_feature_vect(motion_data, interpolation_len, just_data=True):
    """
    Create a single vector of features
    [
        interpXacc, mean_Xacc, std_Xacc, rms_Xacc, (interp_len+3 long)
        interpYacc, mean_Yacc, std_Yacc, rms_Yacc, (interp_len+3 long)
        interpZacc, mean_Zacc, std_Zacc, rms_Zacc, (interp_len+3 long)

        interpXvel, mean_Xvel, std_Xvel, rms_Xvel, (interp_len+3 long)
        interpYvel, mean_Yvel, std_Yvel, rms_Yvel, (interp_len+3 long)
        interpZvel, mean_Zvel, std_Zvel, rms_Zvel, (interp_len+3 long)

                interpXvel_int (interp_len)
                interpYvel_int (interp_len)
                interpZvel_int (interp_len)

        interpXpos, mean_Xpos, std_Xpos, rms_Xpos, (interp_len+3 long)
        interpYpos, mean_Ypos, std_Ypos, rms_Ypos, (interp_len+3 long)
        interpZpos, mean_Zpos, std_Zpos, rms_Zpos, (interp_len+3 long)

                interpXpos_int (interp_len)
                interpYpos_int (interp_len)
                interpZpos_int (interp_len)

        interpXgyro, mean_Xgyro, std_Xgyro, rms_Xgyro, (interp_len+3 long)
        interpYgyro, mean_Ygyro, std_Ygyro, rms_Ygyro, (interp_len+3 long)
        interpZgyro, mean_Zgyro, std_Zgyro, rms_Zgyro, (interp_len+3 long)

        interpXang, mean_Xang, std_Xang, rms_Xang, (interp_len+3 long)
        interpYang, mean_Yang, std_Yang, rms_Yang, (interp_len+3 long)
        interpZang, mean_Zang, std_Zang, rms_Zang, (interp_len+3 long)
        record_time, (1 long)

    ]


    input:
        motion_data : Transformed Acc, vel, pos, rot rate and angle values
            k x 18 array.

            # x,y,z,
            # v_x,v_y,v_z,
            # a_x,a_y,a_z,
            # roll,pitch,yaw,
            # theta,phi,psi

        interpolation_len : An integer value representing target resolution in sensor readings.
    output:
        feature : (interpolation_len + 3)* 18 + 1

    """

    # mean, std, rms
    # |[X, Y, Z]| * |[acc, vel, pos, rot, ang]|
    num_dimensions = motion_data.shape[1]


    # grab the record time
    record_time = motion_data.shape[0]

    # dictionaries are used to hold the agg stat values
    mean = [None] * num_dimensions
    std = [None] * num_dimensions
    rms = [None] * num_dimensions


    block_size = interpolation_len + NUM_AGG_STAT_VALS

    # preallocate output array
    out = np.zeros(
        num_dimensions * (
            interpolation_len + NUM_AGG_STAT_VALS
        ) + 1
    )

    # first grab the agg stat values for each of the 15 motion values
    for i in range(num_dimensions):
        start = i * block_size
        # resize
        interp_end = start + interpolation_len
        resized = interpolate(
            motion_data[:,i],
            interpolation_len,
        )
        # plt.subplot(1,2,1)
        # plt.plot(motion_data[:,i])

        # plt.subplot(1,2,2)
        # plt.plot(resized)


        # MEAN - remove offset
        mean[i] = np.mean(resized)
        resized -= mean[i]

        # STD - normalize
        std[i] = np.std(resized)
        resized /= std[i]

        # RMS
        sq_dist = resized ** 2
        mean_sq_dist = np.mean(sq_dist)
        rms[i] = np.sqrt(mean_sq_dist)


        out[start:interp_end] = resized

        out[interp_end] = mean[i]
        out[interp_end + 1] = std[i]
        out[interp_end + 2] = rms[i]

    out[-1] = record_time

    return out







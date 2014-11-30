# Module imports
import numpy as np

#Class imports
from scipy import ndimage

# function imports

from numpy import interp

"""
preprocess image functions
    - Smear convolves the image with a 2D gaussian blur,
    -
"""
def smear(image, sigma):
    """
    performs a 2D gaussian convolution with the image
        parameters:
            image: n x d numpy matrix
            sigma: sigma term in the smearing
        returns
            blurryIm : n x d numpy matrix
    """
    return ndimage.filters.gaussian_filter(image, sigma, mode='nearest')

def recenter(image):
    # center_of_mass
    y_c, x_c = ndimage.measurements.center_of_mass(np.array(image))
    x_c = int(round(x_c))
    y_c = int(round(y_c))


    # center of image
    n, d = image.shape

    # skew down (add to y-index's to recenter)
    skew_down = n/2 - y_c

    # skew right (add to x-index's to recenter)
    skew_right = d/2 - x_c

    return ndimage.interpolation.shift(
        image,
        [skew_down, skew_right],
        cval=0.0
    )


def normalize(image):
    return image/np.sqrt(np.power(image, 2).sum())

# ASSUME SQUARE!!
def scale(image, threshold=10):
    X = np.round(image, decimals=threshold)
    n, d = X.shape
    Yz, Xz = np.nonzero(X)

    left = Xz.min()
    right = Xz.max()
    top = Yz.min()
    bottom = Yz.max()

    h_span = right - left
    v_span = bottom - top

    proportionX = float(h_span)/float(d)
    proportionY = float(v_span)/float(n)

    goal = .80

    if proportionX > proportionY:
        span = h_span
        extents = d
        proportion = proportionX

    else:
        span = v_span
        extents = n
        proportion = proportionY

    zoom = goal/proportion

    full = np.round(span/goal)

    h_begin = (d - full)/2.
    h_end   = (d + full)/2.


    v_begin = (n - full)/2.
    v_end   = (n + full)/2.


    i_locations = (
        np.int64(
            np.floor(
                np.linspace(
                    h_begin,
                    h_end,
                    d
                )
            )
        )
    )

    j_locations = (
        np.int64(
            np.floor(
                np.linspace(
                    v_begin,
                    v_end,
                    n
                )
            )
        )
    )

    # create scaled img grid mapping
    mg = np.meshgrid(
        i_locations,
        j_locations,
        indexing='ij',
    )

    # zoomed in img
    return X[mg]


def interpolate(Y, return_arr_length):
    dummy = np.ones(return_arr_length)
    interp(dummy, )

def create_feature_vect(motion_data, interpolation_len):
    """
    Create a single vector of features
    [
        interpXaccel, (interpolation_len long)
        interpYaccel, (interpolation_len long)
        interpZaccel, (interpolation_len long)

        interpXvel, (interpolation_len long)
        interpYvel, (interpolation_len long)
        interpZvel, (interpolation_len long)

        interpXpos, (interpolation_len long)
        interpYpos, (interpolation_len long)
        interpZpos, (interpolation_len long)

        interpXgyro, (interpolation_len long)
        interpYgyro, (interpolation_len long)
        interpZgyro, (interpolation_len long)

        interpXang, (interpolation_len long)
        interpYang, (interpolation_len long)
        interpZang, (interpolation_len long)

        mean_Xaccel,(1 long)
        std_Xaccel, (1 long)
        rms_Xaccel, (1 long)

        mean_Yaccel,(1 long)
        std_Yaccel, (1 long)
        rms_Yaccel, (1 long)

        mean_Zaccel,(1 long)
        std_Zaccel, (1 long)
        rms_Zaccel, (1 long)


        mean_Xvel,(1 long)
        std_Xvel, (1 long)
        rms_Xvel, (1 long)

        mean_Yvel,(1 long)
        std_Yvel, (1 long)
        rms_Yvel, (1 long)

        mean_Zvel,(1 long)
        std_Zvel, (1 long)
        rms_Zvel, (1 long)


        mean_Xpos,(1 long)
        std_Xpos, (1 long)
        rms_Xpos, (1 long)

        mean_Ypos,(1 long)
        std_Ypos, (1 long)
        rms_Ypos, (1 long)

        mean_Zpos,(1 long)
        std_Zpos, (1 long)
        rms_Zpos, (1 long)


        mean_Xgyro,(1 long)
        std_Xgyro, (1 long)
        rms_Xgyro, (1 long)

        mean_Ygyro,(1 long)
        std_Ygyro, (1 long)
        rms_Ygyro, (1 long)

        mean_Zgyro,(1 long)
        std_Zgyro, (1 long)
        rms_Zgyro, (1 long)

        mean_Xang,(1 long)
        std_Xang, (1 long)
        rms_Xang, (1 long)

        mean_Yang,(1 long)
        std_Yang, (1 long)
        rms_Yang, (1 long)

        mean_Zang,(1 long)
        std_Zang, (1 long)
        rms_Zang, (1 long)

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
    num_dimensions = 15

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








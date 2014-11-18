# Module imports
import numpy as np

#Class imports
from scipy import ndimage


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







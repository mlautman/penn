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
    return ndimage.filters.gaussian_filter(X, sigma, mode='nearest')

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

    return ndimage.interpolation.shift(image, [skew_down, skew_right], cval=0.0)


def normalize(image):
    return image/np.sqrt(np.power(image, 2).sum())

def scale(image, threshold=10):
    X = np.round(image, decimals=threshold)
    n,d = X.shape
    Yz, Xz = np.nonzero(X)
    left = Xz.min()
    right = Xz.max()
    top = Yz.min()
    bottom = Yz.max()

    proportionX = float(right - left)/d
    proportionY = float(bottom - top)/n

    goal = .80

    zoomX = goal/proportionX
    zoomY = goal/proportionY

    zoom = min(zoomY, zoomX)

    step = 1./zoom

    x = np.arange(0, d, step)
    y = np.arange(0, n, step)

    mg = np.meshgrid(x,y)
    for i in range(2):
        mg[i] = np.int64(np.floor(mg[i]))

    b = X[mg].T

    n2,d2 = b.shape

    extra_n = n2-n
    n_before= np.floor(float(extra_n)/2.)
    n_after= np.ceil(float(extra_n)/2.)

    extra_d = d2-d
    d_before= int(np.floor(float(extra_d)/2.))
    d_after= int(np.ceil(float(extra_d)/2.))

    X2 = b[n_before:n2-n_after, d_before:d2-d_after]
    return X2
    # return ndimage.interpolation.zoom(X, zoom)






import os
import numpy as np
from scipy.interpolate import InterpolatedUnivariateSpline
from preprocess import create_feature_vect
from preprocess import feature_vect_size

def interpolate(y, return_arr_length):
    x = np.linspace(0, 1, len(y))
    xi = np.linspace(0, 1, return_arr_length)
    ius = InterpolatedUnivariateSpline(x, y)
    yi = ius(xi)
    return yi

def get_labels(fnames, label_index):
    class_labels=[None] * len(fnames)

    class_labels = [f[label_index] for f in fnames]
    labels = np.unique(class_labels)

    label_dict = {}
    label_lookup = {}

    for i,l in enumerate(labels):
        label_dict[l]=i
        label_lookup[i]=l

    y = np.array([ label_dict[l] for l in class_labels])
    return y, label_lookup


# timestamp,x,y,z,v_x,v_y,v_z,a_x,a_y,a_z,roll,pitch,yaw,theta,phi,psi
def load_all(path, label_index, prefix='', suffix=''):
    fnames = [
        f for f in os.listdir(path) if (
            f.startswith(prefix) and f.endswith(suffix)
        )
    ]

    signal_len = 200
    samples = len(fnames)

    ex_vect = np.loadtxt(path + '/' + fnames[0], delimiter=',', skiprows=1)[:,1:]

    features = feature_vect_size(ex_vect, signal_len)

    X = np.zeros((samples, features))

    for i,f in enumerate(fnames):
        raw_data = np.loadtxt(path + '/' + f, delimiter=',', skiprows=1)[:,1:]
        X[i,:] = create_feature_vect(raw_data, signal_len, just_data=True)

    y, label_lookup = get_labels(fnames, label_index)

    return X, y, label_lookup




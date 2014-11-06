"""
The Data class is an object intended to encapsulate a data object.
This class
    loads the data,
    encapsulates the intermediate values
    stores the intermediate data when done.
"""
import os
import matplotlib.pyplot as plt
import numpy as np

def find_class_in_fname(s):
    # TODO
    return

class Data(object):
    def __init__(self, X=None, Y=None, fname=None, dirname=None, labeled=False):
        """
        Data should be given either an X value, a filename or a dirname
        The data for the penn project should be in vector format with each horozontal line
        representing a single image.
        """

        self.X = None
        self.Y = None
        self.labeled = None
        self.image_dimensions = (32,32)
        if X:
            """
            X is a n x d np-matrix.
            """
            self.X = X
            if Y:
                self.Y = Y
                self.labeled = True
            else:
                self.labeled = False

        elif fname:
            """
            > fname: be a relative or absolute path to a csv with the data
             comma sepparated.
            > Each column is a new observation.
            """
            self.X = np.matrix(np.loadtxt(fname, delimiter=',')).T
            # TODO: add self.Y code
            if labeled:
                self.Y = find_class_in_fname(fname);
            else:

        elif dirname:
            # grab all file names ito a single vector
            files = os.listdir(dirname)

            #grab one file for size information
            Xtmp = np.matrix(np.loadtxt(dirname + files[0], delimiter=',')).T
            n, d = Xtmp.shape

            # preallocate matrix of shape n,d and copy the files into the X
            self.X = np.matrix(np.zeros((n,len(files))))
            # preallocate matrix of shape d,1 for the y's
            self.Y = np.matrix(np.zeros((len(files),1)))
            for i, f in enumerate(os.listdir(dirname)):
                self.X[:,i] = np.matrix(np.loadtxt(dirname + f, delimiter=',')).T
                self.Y

    def visualize_img(self, image_index):
        plt.imshow(self.X[:,image_index].reshape(self.image_dimensions))
        plt.show()



"""
The Data class is an object intended to encapsulate a data object.
This class
    loads the data,
    encapsulates the intermediate values
    stores the intermediate data when done.
"""
import os

import numpy as np
class Data(object):
    def __init__(self, X=None, fname=None, dirname=None):
        """
        Data should be given either an X value, a filename or a dirname
        The data for the penn project should be in vector format with each horozontal line
        representing a single image.
        """
        if X:
            """
            X is a n x d np-matrix.
            """
            self.X = X


        elif fname:
            """
            > fname: be a relative or absolute path to a csv with the data
             comma sepparated.
            > Each column is a new observation.
            """
            self.X = np.matrix(np.loadtxt(fname, delimiter=',')).T

        elif dirname:
            self.X = None
            files = os.listdir(dirname)
            Xtmp = np.matrix(np.loadtxt(dirname + files[0], delimiter=',')).T
            n, d = Xtmp.shape
            self.X = np.matrix(np.zeros((n,len(files))))
            for i, f in enumerate(os.listdir(dirname)):
                self.X[:,i] = np.matrix(np.loadtxt(dirname + f, delimiter=',')).T

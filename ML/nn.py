"""
nn.py
Authors: Mike Lautman
        Francisco de Villalobos
"""

import numpy as np
from sklearn import preprocessing
from sklearn.base import ClassifierMixin

class NN(ClassifierMixin):
    def
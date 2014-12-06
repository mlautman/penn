from __future__ import division
import numpy as np
import sys
import pickle

from sklearn.metrics import accuracy_score
from sklearn import svm
from sklearn.externals import joblib

from preprocess import create_feature_vect


if __name__ == '__main__':
    path = '../matlab/python/'

    #model = joblib.load(path + 'letter_clf.pkl')
    #label_lookup = pickle.load(open(path + 'label_lookup.p','r'))
    
    pickled = pickle.load(open(path + 'letter_clf.p','r'))
    model = pickled[0]
    label_lookup = pickled[1]

    signal_len = 40 #this is unclean: should be matched to training data signal_len
    file_loc = path + 'letter.txt'
    #file_loc = '../data/letter_logs1Justin/proc_z14-12-05_13:21:55.txt'
    raw_data = np.loadtxt(file_loc, delimiter=',', skiprows=1)[:,1:]
    Xnew = create_feature_vect(raw_data, signal_len, just_data=True)

    pred = model.predict(Xnew)

    sys.stdout.write(label_lookup[pred[0]])



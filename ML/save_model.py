from __future__ import division
import numpy as np
import pickle

from sklearn.metrics import accuracy_score
from sklearn import svm

from merge_data import load_all


if __name__ == '__main__':
    split = 0.9
    prefix = 'proc_'
    path = '../data/letter_logs1Justin2/'
    X, y, label_lookup = load_all(path, 5, prefix=prefix)

    n, d = X.shape
    # shuffle
    idx = np.arange(n)
    np.random.shuffle(idx)
    X = X[idx]
    y = y[idx].reshape(-1)

    # split the data
    c = np.rint(split * n)

    # train on first 100 instances
    Xtrain = X[:c,:]
    ytrain = y[:c]

    # test on remaining instances
    Xtest = X[c:,:]
    ytest = y[c:]

    C = 1.
    equivalentGamma = 1.0 / (2. ** 2)
    model = svm.SVC(
        C = C,
        gamma=equivalentGamma,
        kernel='linear',
    )

    model.fit(Xtrain, ytrain)
    testPred = model.predict(Xtest)
    accuracy = accuracy_score(ytest, testPred)
    print accuracy

    path = '../matlab/python/'
    pickle.dump( (model, label_lookup), open(path + 'letter_clf.p', 'w'))

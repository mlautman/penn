from sklearn.externals import joblib


def save(fname, obj):
    joblib.dump(obj, fname)


###
#
# load a saved object
#
###
def load(fname):
    return joblib.load(fname)

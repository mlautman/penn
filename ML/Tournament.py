"""
Tournament.py
"""

def store_results(fname, params):
    """
    Store results takes in a dictionary of accuracy
    characteristics and saves them to a file
    :parameter: params - dict like
        {
            learning algorithm: string
            Kernel: string (eg. linear, rbf)
            accuracy: correct/all
            split: float
        }
    """
    pass

def ROC_curve(fname):
    """
    Plots and stores a ROC curve for a learning
    algorithm.
    """
    pass

def main():
    """
    Main runs the machine learning algorithms
    and computes accuracy numbers each to be
    saved to a text file.

    Algorithms to compare:
        SVM
        LogReg
        Boosting
        Decision Trees
        CNN
        DTW with K-NN
        PNN

    Methods for comparison:
        10-fold testing
        ROC curves for each letter.
    """
    pass


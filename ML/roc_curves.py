
import matplotlib.pyplot as plt
from sklearn.metrics import roc_curve, auc

# Calculation and graph of ROC curves
def calc_roc_curves(fname, y_probs, y_binarized):
  n, d = y_probs.shape
  roc_indexes = range(d)

  # Compute ROC curve and ROC area for each class
  fpr = dict()
  tpr = dict()
  roc_auc = dict()

  for i in range(len(roc_indexes)):
    fpr[i], tpr[i], _ = roc_curve(y_binarized[:, roc_indexes[i]], y_probs[:, roc_indexes[i]])
    roc_auc[i] = auc(fpr[i], tpr[i])

  # Plot NB ROC curve
  plt.clf()
  plt.figure(1)
  for i in range(len(roc_indexes)):
    plt.plot(fpr[i], tpr[i])

  plt.plot([0, 1], [0, 1], 'k--')
  plt.xlim([0.0, 1.0])
  plt.ylim([0.0, 1.05])
  plt.xlabel('False Positive Rate')
  plt.ylabel('True Positive Rate')
  plt.title('ROC Curves for SVMs')
  plt.legend(loc="lower right")
  plt.savefig(fname)


# Calculation and graph of ROC curves
def calc_roc_curves(y_probs, y_binarized):
  roc_indexes = range(37)

  # Compute ROC curve and ROC area for each class
  fpr = dict()
  tpr = dict()
  roc_auc = dict()

  for i in range(roc_indexes.size):
    fpr[i], tpr[i], _ = roc_curve(y_binarized[:, roc_indexes[i]], y_probs[:, roc_indexes[i]])
    roc_auc[i] = auc(fpr[i], tpr[i])

  # Plot NB ROC curve
  plt.figure(1)
  for i in range(roc_indexes.size):
    plt.plot(fpr[i], tpr[i], label='ROC curve of class {0} (area = {1:0.4f})'
                                  ''.format(roc_indexes[i], roc_auc[i]))

  plt.plot([0, 1], [0, 1], 'k--')
  plt.xlim([0.0, 1.0])
  plt.ylim([0.0, 1.05])
  plt.xlabel('False Positive Rate')
  plt.ylabel('True Positive Rate')
  plt.title('ROC Curves for SVMs')
  plt.legend(loc="lower right")
  plt.show()


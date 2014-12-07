import csv
import numpy as np
from sklearn.metrics import precision_recall_fscore_support

def perf_csv(name, y_true, y_pred, label_lookup, beta = 1.0):
    prec, rec, fscore, _ = precision_recall_fscore_support(y_true, y_pred, beta);
    ave_prec, ave_rec, ave_fscore, _ = \
            precision_recall_fscore_support(y_true, y_pred, beta, average='micro')

    with open(name+'_scores.csv','w') as csvfile:
        writer = csv.writer(csvfile, delimiter=',')
        
        writer.writerow(['classes'] + ['fscore'] + ['precision'] + ['recall'])
        writer.writerow(['average'] + ['%.6f' % ave_fscore] + ['%.6f' % ave_prec] + ['%.6f' % ave_rec])
        for ii in xrange(len(label_lookup.values())):
            writer.writerow([label_lookup.values()[ii]] + ['%.6f' % fscore[ii]] + \
                    ['%.6f' % prec[ii]] + ['%.6f' % rec[ii]])

        '''
        writer.writerow(['metric'] + ['average'] + label_lookup.values())
        writer.writerow(['fscore'] + ['%.6f' % ave_fscore] + ['%.6f' % x for x in fscore])
        writer.writerow(['precision'] + ['%.6f' % ave_prec] + ['%.6f' % x for x in prec])
        writer.writerow(['recall'] + ['%.6f' % ave_rec] + ['%.6f' % x for x in rec])
        '''


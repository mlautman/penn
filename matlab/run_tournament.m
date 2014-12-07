
dir_input = '../data/together/proc/';
dir_output = '../clf_logs/';
prefix = 'proc_';

call_tourn = 'ipython ../ML/Tournament.py';
call_base = [call_tourn,' -i ',dir_input,' -o ',dir_output,' -p ',prefix];

algorithms = {'svm','logreg','boost','dt','knn'};
folds = 10;
interp_size = round(logspace(1,3,3));


% SVM Specific Parameters
svm_Cs = [0.2,1,5];                         % regularization
kernels = {'poly','linear'};                % kernels
max_iter = 1E5;                             % max iterations
svm_tol = 10.^(-5);                         % stop tolerance 

gamma = 1./(2*([0.5,1,3]).^2);              % kernel width
degree = [2,3,5,8];                         % poly kernel degree

% Logistic Regression Specific Parameters
penalty = {'l1','l2'};                      % distance metrics
lr_Cs = [0.2,1,5];                          % regularization
lr_tol = 10.^(-5);                          % stop tolerance

% Adaboost Specific Parameters
base_est = 'DecisionTreeClassifier';
n_est = [10,50];
learn_rate = [1,2];
ada_max_feat = 'sqrt';
ada_max_depth = [1,2];

% Decision Trees Specific Parameters
max_feat = 'sqrt';
max_depth = [4,5,7,100];

% K Nearest Neighbots Specific Parameters
k_neigh = [3,5,7,11];
max_warp_wind = round(0.1*interp_size);
subsample_step = round(0.01*interp_size + 1);

% Output
fid = fopen('run_tournament.txt','w');

isvm = 1;
ilr = 1;
iab = 1;
idt = 1;
iknn = 1;


for a = algorithms % loop over learning algorithms
    for f = interp_size % loop over increasing iteration size
        params1 = [' -k ',num2str(folds),' -a ',a{1},' -f ',num2str(f)];
        
        switch a{1}
            
            case 'svm'                
                for k = kernels
                    for C = svm_Cs
                        params2 = [params1,...
                            ' --svc_C=',num2str(C),...
                            ' --kernel=',k{1},...
                            ' --max_iter=',num2str(max_iter),...
                            ' --tolerance=',num2str(svm_tol)];

                        switch k{1}
                            
                            case 'linear'
                                fprintf(fid,[call_base,params2,' --name svm',num2str(isvm),'\n']);
                                isvm = isvm+1;
                            case 'poly'
                                for d = degree
                                    for g = gamma
                                        params3 = [params2,...
                                            ' --degree=',num2str(d),...
                                            ' --gamma=',num2str(g)];
                                        fprintf(fid,[call_base, params3,' --name svm',num2str(isvm),'\n']);
                                        isvm = isvm+1;
                                    end
                                end
                            case 'sigmoid'
                                for g = gamma
                                    params3 = [params2,...
                                        ' --gamma=',num2str(g)];
                                    fprintf(fid,[call_base, params3,' --name svm',num2str(isvm),'\n']);
                                    isvm = isvm+1;
                                end
                            case 'rbf'
                                for g = gamma
                                    params3 = [params2,...
                                        ' --gamma=',num2str(g)];
                                    fprintf(fid,[call_base, params3,' --name svm',num2str(isvm),'\n']);
                                    isvm = isvm+1;
                                end    
                        end
                    end
                end
                
            case 'knn'
                for k = k_neigh
                    params2 = [params1, ...
                        ' --n_neighbors',num2str(k),...
                        ' --max_warping_window',num2str(round(0.1*a{1})),...
                        ' --subsample_step',num2str(round(0.01*a{1}+1))];
                    fprintf(fid,[call_base,params2,' --name knn',num2str(iknn),'\n']);
                    iknn = iknn+1;
                end
                
            case 'logreg'
                for p = penalty
                    for C = lr_Cs
                        params2 = [params1,...
                            ' --penalty=',p{1},...
                            ' --C_lr=',num2str(C),...
                            ' --tolerance=',num2str(lr_tol)];
                        fprintf(fid,[call_base, params2,' --name lr',num2str(ilr),'\n']);
                        ilr = ilr+1;
                    end
                end
                
            case 'dt'
                for d = max_depth
                    params2 = [params1, ...
                        ' --max_features=',max_feat,...
                        ' --max_depth=',num2str(d)];
                    fprintf(fid,[call_base, params2,' --name dt',num2str(idt),'\n']);
                    idt = idt+1;
                end
                
            case 'boost'
                for e = n_est
                    for d = ada_max_depth
                        for l = learn_rate
                            params2 = [params1, ...
                                ' --base_estimator=',base_est,...
                                ' --n_estimators=',num2str(e),...
                                ' --learning_rate=',num2str(l),...
                                ' --max_features=',ada_max_feat,...
                                ' --max_depth=',num2str(d)];
                            fprintf(fid,[call_base, params2,' --name ab',num2str(iab),'\n']);
                            iab = iab+1;
                        end
                    end
                end
                
        end
    end
end

fprintf(['svm ',num2str(isvm), '\n'])
fprintf(['knn ',num2str(iknn), '\n'])
fprintf(['logreg ',num2str(ilr), '\n'])
fprintf(['dt ',num2str(idt), '\n'])
fprintf(['adaboost ',num2str(iab), '\n'])

fclose(fid);

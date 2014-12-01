% Documentation ===========================================================
%                       z   y
%     z  y       Q       | /   M Penn frame
%     | /        . _ _ _ .__ x
%     .__ x              P
%    O    G Ground frame
% 
% G: ground frame
% M: IMU frame
% O: ground origin
% P: IMU origin
% Q: pen tip
%
% Note:
% w stands for omega (angular velocity)
% alpha is the angular acceleration
% w and alpha are of frame M relative to frame G
% 
% Notation Examples:
% M_w is w measured in frame M
% G_RM is the rotation of frame M in frame G
% G_Q is the position of point Q in frame G

% GUI
nogui = true;

% File directory
directory = 'letter_logs';

% Simulated run
rerun = false;
if rerun
    rerun_file = 'a14-11-30_20:29:36';
    f_rerun = fopen([directory,'/raw_',rerun_file,'.txt'],'r');
    rerun_cntr = 0;
end

% Initialization ==========================================================
%{a
% Open serial interface to M2
if ~exist('s','var') && ~rerun
s = serial('/dev/ttyACM0');
s.baudrate = 57600;
s.terminator = 'LF';
fopen(s);
end
%}

% Prepare data log
numHistory = 1000;
numVals = 9;
log = zeros(numHistory,numVals+1);
log(end,1) = toc;

% Display Raw Signals
% Display parameters
toPlot = 3:8; % indices of raw logs to plot
plot_reduce = 30; % plot every plot_reduce loops
plt_red_cntr = 0;

if ~nogui
figure(99)
clf
haxis99 = axes;
title('Raw Serial Data')

h_raws = zeros(length(toPlot),1);
color_list = 'rgbcmk';
line_list = {'-','--'};
hold on
for ii=1:length(toPlot)
    h_raws(ii) = plot(log(:,1),log(:,toPlot(ii)+1),...
        [color_list(mod(ii-1,length(color_list))+1),...
        line_list{mod(floor((ii-1)/length(color_list)),length(line_list))+1}]);
end
hold off
grid on
end

% Calculations and plotting
% vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
processedVals = 12;
processed = zeros(numHistory,processedVals);

% Parameters
M_Q = [83.6*10^-3; 0; 0]; %penn tip position in IMU frame

% Attitude estimation
addpath('quaternion_library')
gyr_max_scale = 250; %degrees per second max
acc_max_scale = 2; %gs max

gyr_scale = 250/32768 * pi/180;
acc_scale = 2/32768 * 9.81;

acc_scale = acc_scale + [-2.2e-6; -3.5e-6; -4e-6]; %by-hand calibration adjustments (m/s^2/count)
acc_offs = [-0.27; 0.06; -0.465]; %by-hand offset adjustments (m/s^2)

%AHRS = MahonyAHRS('SamplePeriod', 1/300, 'Kp', 0.5, 'Ki', 0.05); % Kalman filter
AHRS = MadgwickAHRS('SamplePeriod', 1/300, 'Beta', 0.05); % Kalman filter
% Notes:
% MadgwickAHRS converges linearly, then stops at zero while Mahoney follows
% a second order damped curve (less desireable).  Madgwick's Beta controls
% how much the accelerometer is trusted against the gyro, which we want low
% to prevent accelerations coupling to rotations

% Filters and Integration
M_aP_g = [0;0;9.81];
M_w = [0;0;0];
M_w_old = 0;
M_alpha_LP = 0;

alpha1 = 0.995;
alpha2 = 0.995;
G_vQ_old = 0;
G_Q_old = 0;

alpha_yaw = 0.998;
yaw_old = 0;
yaw = 0;

alpha_a = 0.99;
G_aP_LP = zeros(3,1);


% Plotting
% Plot selected processed signals (euler angles, ground frame accelerations, etc.)
if ~nogui
figure(1)
clf
haxis1 = axes;
title('Processed Signals')

h_processed = zeros(3,1);
hold on
for ii=1:3
    h_processed(ii) = plot(log(:,1), processed(:,ii),...
        [color_list(mod(ii-1,length(color_list))+1),...
        line_list{mod(floor((ii-1)/length(color_list)),length(line_list))+1}]);
end
hold off
grid on


% Plot 3D pen visualization
figure(2)
clf
haxis2 = axes;

pen3D = [0, 0, 0.05; % long arm along pen, short arm away from author
         0, 0, 0;
         M_Q';
        -0.15, 0, 0]';
         
h_3D = plot3(pen3D(1,:),pen3D(2,:),pen3D(3,:),'k');
axis equal
set(gca,'xlim',[-0.2,0.2],'ylim',[-0.2,0.2],'zlim',[-0.2,0.2]);
grid on
title('Pen Visualization')
xlabel('x')
ylabel('y')
zlabel('z')


% Plot character integration
figure(3)
clf
haxis3 = axes;
h_char = plot(processed(:,1),processed(:,2),'.-b');
axis equal
grid on
end

% Character Label
if ~rerun
    char_writing = input('Character: ','s');
    if ~rerun && ~nogui
        title(char_writing);
        set(gcf,'windowkeypressfcn',['char_writing = get(gcf,''currentcharacter'');', ...
        'title(char_writing);'])
    end
else
    char_writing = rerun_file(1);
    title(char_writing);
end

% Rerun Initialization
if rerun
    fgets(f_rerun);
    
    % Initialize 
    f_proced = fopen([directory,'/proc_',rerun_file,'.txt'],'r');
    fgets(f_proced)
    data = fgets(f_proced)
    fclose(f_proced)
    numdata = str2num(data)
    euler = numdata(10:12);
    M_RG = euler2rotMat(euler(1),euler(2),euler(3))';
    AHRS.Quaternion = rotMat2quatern(M_RG);
end
% ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

% Run Loop ================================================================
tic
while(1)
    % Read data
    if ~rerun
        % Read data from M2
        data = fgets(s);
        numdata = str2num(data);
        
        if all(size(numdata) == [1,numVals]) % reject malformed packets
            log = [log(2:end,:);[toc,numdata]];
        else
            continue
        end
    else
        % Read data from log file
        data = fgets(f_rerun);
        
        if ~ischar(data)
            fclose(f_rerun)
            
            % Write file and exit at end of raw file
            toappend = rerun_file((end-17):end);
            f_proc = fopen([directory,'/','post_',toappend,'.txt'],'w');
            fprintf(f_proc,'a_x,a_y,a_z,v_x,v_y,v_z,x,y,z,roll,pitch,yaw\n');
            fclose(f_proc);
            dlmwrite([directory,'/','post_',toappend],processed,'-append')
            
            return
        end
        
        numdata = str2num(data);

        log = [log(2:end,:);numdata];
        rerun_cntr = rerun_cntr+1;
    end
    
    
    
    % Calculations
    % vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
    
    % Time, Acceleration and Rotation scaling
    dt = log(end,1)-log(end-1,1);           % time since last loop
    M_aP_g_raw = log(end,4:6)' .* acc_scale + acc_offs;
    M_w_raw = log(end,7:9)' .* gyr_scale;
    
    % Initial low pass filters
    tau_acc = 0.04;                         % accelerometer LP t. const. (s)
    tau_gyr = 0.02;                         % gyroscope LP t. const. (s)
    al_acc = dt/(tau_acc + dt);
    al_gyr = dt/(tau_gyr + dt);
    M_aP_g = al_acc*M_aP_g_raw + (1-al_acc)*M_aP_g;
    M_w = al_gyr*M_w_raw + (1-al_gyr)*M_w;
    
    % Attitude estimation
    AHRS.UpdateIMU(M_w', M_aP_g');          % Kalman filter
    quaternion = AHRS.Quaternion;           % quaternion rotation
    M_RG = quatern2rotMat(quaternion);      % rotation matrix
    % Correct for gyro drift
    euler = rotMat2euler(M_RG');
    yaw = alpha_yaw*(yaw + (euler(3) - yaw_old));
    yaw_old = euler(3);
    euler(3) = yaw;
    M_RG = euler2rotMat(euler(1),euler(2),euler(3))';
    
    % Gravity cancellation and ground frame rotations
    M_aP = M_RG*[0;0;-9.81] + M_aP_g;       % acceleration w/o gravity
    G_aP = M_RG'*M_aP;                      % acc. w/o g in ground frame
    G_w = M_RG'*M_w;                        % ang. vel. in ground frame
    
    % Tip Acceleration in the Ground Frame
    % Angular acceleration
    % low pass, differentiate, low pass, rotate into ground frame
    tau_w = 0.02;                           % w low pass time constant (s)
    tau_alpha = 0.02;                       % alpha LP filt. t. const. (s)
    al_w = dt/(tau_w + dt);
    al_alpha = dt/(tau_alpha + dt);
    
    M_alpha = (M_w - M_w_old)/dt;           % angular acceleration
    M_w_old = M_w;
    M_alpha_LP = al_alpha*M_alpha + (1-al_alpha)*M_alpha_LP; %alpha LP
    G_alpha = M_RG'*M_alpha_LP;             % alpha in ground frame
    
    % Tip acceleration in the ground frame
    %                         tangential       coriolis          centripetal
    % A_aQ = A_aP + B_aQ + A_alphaB x PQ + 2*A_wB x B_vQ + A_wB x (A_wB x PQ)
    % A_aQ = A_aP + 0 + A_alphaB x PQ + 0 + A_wB x (A_wB x PQ)
    G_aP_LP = G_aP;%alpha_a*G_aP + (1-alpha_a)*G_aP_LP; %LP linear acceleration to prevent drift
    G_aQ = G_aP_LP + cross(G_alpha, M_RG'*M_Q) + cross(G_w,cross(G_w, M_RG'*M_Q));
    
    % Zeroing
    if log(end,2) && ~log(end-1,2)
        G_aQ = zeros(3,1);
        G_vQ_old = zeros(3,1);
        G_Q_old = zeros(3,1);
    end
    
    % IMU position integration
    G_vQ = alpha1*(G_vQ_old + G_aQ*1/300);
    G_vQ_old = G_vQ;
    G_Q = alpha2*(G_Q_old + G_vQ*1/300);
    G_Q_old = G_Q;
        
    processed = [processed(2:end,:); [G_Q', G_vQ', G_aQ', euler] ];
    
    
    % File output 
    if log(end-1,2) && ~log(end,2) && ~isempty(char_writing) && ~rerun
        % datetime to append to filenames
        datetime = datestr(now,'yy-mm-dd_HH:MM:SS');
        
        % Raw data
        f_raw = fopen([directory,'/','raw_',char_writing,datetime,'.txt'],'w');
        fprintf(f_raw,'comptime,switch,timestamp,a_x,a_y,a_z,w_x,w_y,w_z,switch\n');
        fclose(f_raw);
        dlmwrite([directory,'/','raw_',char_writing,datetime,'.txt'],log,'-append')
        
        % Integrated data
        f_proc = fopen([directory,'/','proc_',char_writing,datetime,'.txt'],'w');
        fprintf(f_proc,'a_x,a_y,a_z,v_x,v_y,v_z,x,y,z,roll,pitch,yaw\n');
        fclose(f_proc);
        dlmwrite([directory,'/','proc_',char_writing,datetime,'.txt'],processed,'-append')
        
        % Integrated image
        fig_im = getframe(3);
        im = fig_im.cdata;
        imwrite(im,[directory,'/','im_',char_writing,datetime,'.png'])
    end
    % ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
    
    % Display vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
    if ~nogui
    plt_red_cntr = mod(plt_red_cntr+1,plot_reduce);
    if ~plt_red_cntr
        % Raw Signals
        for ii=1:length(toPlot)
            set(h_raws(ii), 'xdata', log(:,1), 'ydata', log(:,toPlot(ii)+1))
        end
        set(haxis99, 'xlim', [log(1,1), log(end,1)])
        
        % Processed Signals
        for ii=1:3
            set(h_processed(ii), 'xdata', log(:,1), 'ydata', processed(:,ii))
        end
        set(haxis1, 'xlim', [log(1,1),log(end,1)])
        
        % 3D Pen Visualization
        newPen = M_RG'*pen3D;
        set(h_3D, 'xdata', newPen(1,:), 'ydata', newPen(2,:), 'zdata', newPen(3,:))
        
        % Character Visualization
        toDraw = processed;
        toDraw(log(:,2)==0,:) = NaN;
        set(h_char, 'xdata', toDraw(:,1), 'ydata', toDraw(:,2))
        drawnow
    end
    else
        disp(norm(G_vQ))
    end
    % ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
        
end

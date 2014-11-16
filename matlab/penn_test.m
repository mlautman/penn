% Documentation ===========================================================
%                       z   y
%     z  y     tip       | /    P Penn frame
%     | /        . _ _ _ .__ x
%     .__ x              IMU
%    O      G Ground frame
% 
% A: Penn tip
% P: IMU frame
% C: IMU center
% G: Ground frame

tipP = [83.6*10^-3; 0; 0]; %penn tip position in IMU frame

% Initialization ==========================================================
%quadSerial
%{a without the a does block comment in Matlab. If you delete a, block comment.

%% Initialize program and USB port
if(exist('M2USB'))
  fclose(M2USB);
 else
%  fclose(instrfindall);
 delete(instrfindall);
end

%% If the above initialization does not work, please run the following commands manually and disconnect and reconnect USB.
%  fclose(serial('COM5','Baudrate', 9600));
%  fclose(instrfindall);
clear all;
close all;

%% SERIAL - Open serial interface to M2

%----> for ***UBUNTU***
%{a
if ~exist('s','var')
    s = serial('/dev/ttyACM0');
%     s = serial('/dev/tty.usbmodem411');
    s.baudrate = 57600;
    s.terminator = 'LF';
    fopen(s);
end
%}

%----> for ***WINDOZE***
%{
M2USB = serial('COM5','Baudrate', 9600);
*** Use the device manager to check where the microcontroller is plugged
into.
%}

%----> for ***MAC***
%{
M2USB = serial('/dev/tty.usbmodem411','Baudrate',9600);
fopen(M2USB);       % Open up the port to the M2 microcontroller.
flushinput(M2USB);  % Remove anything extranneous that may be in the buffer.
%}

%%
% Prepare data log
numHistory = 1000;
numVals = 9;
log = zeros(numHistory,numVals+1);

% Display and additional code setup
figure(62)
clf
haxis62 = axes;
title('Raw Serial Data')

toPlot = 3:8; % indices of raw logs to plot
plot_reduce = 10; % plot every plot_reduce loops
plt_red_cntr = 0;

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

% Additional calculations and plotting
% vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
% Attitude estimation
addpath('quaternion_library')
gyr_max_scale = 250; %degrees per second max
acc_max_scale = 2; %gs max

gyr_scale = 250/32768 * pi/180;
acc_scale = 2/32768 * 9.81;

%AHRS = MahonyAHRS('SamplePeriod', 1/300, 'Kp', 0.5); % Kalman filter
AHRS = MadgwickAHRS('SamplePeriod', 1/300, 'Beta', 0.1); % Kalman filter

% Integration
v_old = [0;0;0];
p_old = [0;0;0];
alpha1 = 0.9;
alpha2 = 0.9;

tp_old = [0;0;0];
alpha3 = 0.99;
NanVect = NaN(numHistory,1);

yaw_old = 0;
raw_old = 0;
alpha4 = 0.95;

% Plot selected processed signals (euler angles, ground frame accelerations, etc.)
figure(1)
clf
haxis1 = axes;
title('Processed Signals')

h_sigs = zeros(3,1);
sigs = zeros(numHistory,3);

hold on
for ii=1:3
    h_sigs(ii) = plot(log(:,1), sigs(:,ii),...
        [color_list(mod(ii-1,length(color_list))+1),...
        line_list{mod(floor((ii-1)/length(color_list)),length(line_list))+1}]);
end
hold off
grid on


% Plot 3D pen visualization
figure(2)
clf
haxis2 = axes;
title('Pen Visualization')

pen3D = [0, 0, 0.05; % long arm along pen, short arm away from author
         0, 0, 0;
         tipP';
        -0.15, 0, 0]';
         
h_3D = plot3(pen3D(1,:),pen3D(2,:),pen3D(3,:),'k');
axis equal
set(gca,'xlim',[-0.2,0.2],'ylim',[-0.2,0.2],'zlim',[-0.2,0.2]);
grid on


% Plot character integration
figure(3)
clf
haxis3 = axes;
title('Drawn Character')
h_char = plot(sigs(:,1),sigs(:,2),'b');
axis equal
grid on

char_writing = input('Character: ','s');
title(char_writing);
save_ind = 0;
dir = 'example_letters';
set(gcf,'windowkeypressfcn','char_writing = get(gcf,''currentcharacter''); title(char_writing); save_ind = 0;')
% ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^


% Run Loop ================================================================
tic
while(1)
    % Read data from M2
    data = fgets(s);
    %fprintf(data); %display read data
    numdata = str2num(data);
    if all(size(numdata) == [1,numVals]) % reject malformed packets
        log = [log(2:end,:);[toc,numdata]];
    end
    
    disp(log(end,7:9)*gyr_scale)
    
    % Additional calculations
    % vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
    % Acceleration and Rotation scaling
    a_meas_P = log(end, 4:6)' * acc_scale;
    w_PG = log(end, 7:9)' * gyr_scale;
    
    % Attitude estimation
    AHRS.UpdateIMU(w_PG, a_meas_P);
    quaternion = AHRS.Quaternion;
    
    % Rotation, projection, and integration
    rotGP = quatern2rotMat(quaternion);
    %{
        euler = rotMat2euler(rotGP');
        yaw = alpha4*(yaw_old + (euler(3) - raw_old));
        yaw_old = yaw;
        raw_old = euler(3);
        euler(3) = yaw;
        rotGP = euler2rotMat(euler(1),euler(2),euler(3));
    %}
    a_P = rotGP*[0;0;-9.81] + a_meas_P;
    a_PG = rotGP'*a_P;
    
    % IMU position integration
    v_PG = alpha1*(v_old + a_PG*1/300);
    v_old = v_PG;
    p_PG = alpha2*(p_old + v_PG*1/300);
    p_old = p_PG;
    
    % Tip position integration
    tv_P = cross(w_PG, rotGP'*tipP);
    tp_P = alpha3*(tp_old + tv_P*1/300);
    tp_old = tp_P;
    
    sigs = [sigs(2:end,:); tp_P' + p_PG'];%p_PG'];%a_PG'];%quatern2euler(quaternConj(quaternion)) * (180/pi)];
    
    % ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
    
    % Display
    plt_red_cntr = mod(plt_red_cntr+1,plot_reduce);
    if ~plt_red_cntr
        for ii=1:length(toPlot)
            set(h_raws(ii), 'xdata', log(:,1), 'ydata', log(:,toPlot(ii)+1))
        end
        set(haxis62, 'xlim', [log(1,1), log(end,1)])
        
        % Additional Plotting
        % vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
        % Other signals
        for ii=1:3
            set(h_sigs(ii), 'xdata', log(:,1), 'ydata', sigs(:,ii))
        end
        set(haxis1, 'xlim', [log(1,1),log(end,1)])
        
        % Penn visualization
        newPen = rotGP'*pen3D;
        set(h_3D, 'xdata', newPen(1,:), 'ydata', newPen(2,:), 'zdata', newPen(3,:))
        
        % Character visualization
        toDraw = sigs;
        toDraw(log(:,2)==0,:) = NaN;
        set(h_char, 'xdata', toDraw(:,1), 'ydata', toDraw(:,2))
        % ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
        drawnow
    end
    
    % Saving 
    if log(end-1,2) && ~log(end,2)
        fig_im = getframe(3);
        im = fig_im.cdata;
        imwrite(im,[dir,'/',char_writing,num2str(save_ind),'.png'])
        save_ind = save_ind + 1;
    end
        
end

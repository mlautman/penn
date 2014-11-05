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
cntr = 0;
figure(62)
clf
haxis62 = axes;

toPlot = 3:8; % indices of raw logs to plot
plot_reduce = 10; % plot every plot_reduce loops

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
addpath('quaternion_library')
gyr_max_scale = 250; %degrees per second max
acc_max_scale = 2; %gs max

gyr_scale = 250/32768 * pi/180;
acc_scale = 2/32768 * 9.81;

eulers = zeros(numHistory, 3);

% Kalman filter
%AHRS = MahonyAHRS('SamplePeriod', 1/300, 'Kp', 0.5);
AHRS = MadgwickAHRS('SamplePeriod', 1/300, 'Beta', 0.1);

%Plot euler angles
figure(1)
clf
haxis1 = axes;

h_filts = zeros(3,1);
hold on
for ii=1:3
    h_filts(ii) = plot(log(:,1), eulers(:,ii),...
        [color_list(mod(ii-1,length(color_list))+1),...
        line_list{mod(floor((ii-1)/length(color_list)),length(line_list))+1}]);
end
hold off
grid on

% 3D pen plot
figure(2)
clf
haxis2 = axes;

pen3D = [0, 0, -5;
         -1, 0, 0;
         0, 0, 0];
         
h_3D = plot3(pen3D(1,:),pen3D(2,:),pen3D(3,:),'k');
axis equal
set(gca,'xlim',[-6,6],'ylim',[-6,6],'zlim',[-6,6]);
grid on
% ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^


%HACK
quaternion = [1, 0, 0, 0];

% Run loop
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
    %R = quatern2rotMat(quaternion);
    %mag = R*[0;1;0];
    %AHRS.Update(log(end,6:8) * gyr_scale, log(end,3:5) * acc_scale/9.81, [1, 0, 0]);%mag');	% gyroscope units must be radians
    AHRS.UpdateIMU(log(end,7:9) * gyr_scale, log(end,4:6) * acc_scale/9.81);
    quaternion = AHRS.Quaternion;
    eulers = [eulers(2:end,:);quatern2euler(quaternConj(quaternion)) * (180/pi)];
    % ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
    
    % Display
    cntr = mod(cntr+1,plot_reduce);
    if ~cntr
        for ii=1:length(toPlot)
            set(h_raws(ii), 'xdata', log(:,1), 'ydata', log(:,toPlot(ii)+1))
        end
        set(haxis62, 'xlim', [log(1,1), log(end,1)])
        
        % Additional Plotting
        % vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
        for ii=1:3
            set(h_filts(ii), 'xdata', log(:,1), 'ydata', eulers(:,ii))
        end
        set(haxis1, 'xlim', [log(1,1),log(end,1)])
        
        rot = quatern2rotMat(quaternion);
        newPen = rot'*pen3D;
        set(h_3D, 'xdata', newPen(1,:), 'ydata', newPen(2,:), 'zdata', newPen(3,:))
        
        % ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
        drawnow
    end
end

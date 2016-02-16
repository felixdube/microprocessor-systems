clear all, close all;
data = importdata('data_bad_parameter.txt');
s = size(data);
number = 1:1:s;
dataformatted = [number', data];
str = sprintf('Kalman filter r = %f, q = %f, x = %f, p = %f, k = %f',dataformatted(1,3),dataformatted(1,4), dataformatted(1,5), dataformatted(1,6),dataformatted(1,7));
figure('units','normalized','outerposition',[0 0 1 1])
subplot(2,1,1),
plot(dataformatted(:,1),400 * dataformatted(:,2)*3/4096 - 279,dataformatted(:,1),400 * dataformatted(:,5)*3/4096 - 279);
%plot(dataformatted(:,1),dataformatted(:,2),dataformatted(:,1),dataformatted(:,5));
title(str,'fontsize',18);
xlabel('measurement #','fontsize',14);
ylabel('Temperature in degree C','fontsize',14);
legend('Input Data','Output Data');
axis([0 500 35 45]);
data = importdata('data_good_parameter.txt');
s = size(data);
number = 1:1:s;
dataformatted = [number', data];
str = sprintf('Kalman filter r = %f, q = %f, x = %f, p = %f, k = %f',dataformatted(1,3),dataformatted(1,4), dataformatted(1,5), dataformatted(1,6),dataformatted(1,7));
subplot(2,1,2),
plot(dataformatted(:,1),400 * dataformatted(:,2)*3/4096 - 279,dataformatted(:,1),400 * dataformatted(:,5)*3/4096 - 279);
%plot(dataformatted(:,1),dataformatted(:,2),dataformatted(:,1),dataformatted(:,5));
title(str,'fontsize',18);
xlabel('measurement #','fontsize',14);
ylabel('Temperature in degree C','fontsize',14);
legend('Input Data','Output Data');
axis([0 500 35 45]);
% 
% data = importdata('data2.txt');
% s = size(data);
% number = 1:1:s;
% dataformatted = [number', data];
% str = sprintf('Kalman filter r = %f, q = %f, x = %f, p = %f, k = %f',dataformatted(1,3),dataformatted(1,4), dataformatted(1,5), dataformatted(1,6),dataformatted(1,7));
% subplot(3,1,3),
% plot(dataformatted(:,1),400 * dataformatted(:,2)*3/4096 - 279,dataformatted(:,1),400 * dataformatted(:,5)*3/4096 - 279);
% %plot(dataformatted(:,1),dataformatted(:,2),dataformatted(:,1),dataformatted(:,5));
% title(str,'fontsize',18);
% xlabel('measurement #','fontsize',14);
% ylabel('Temperature in degree C','fontsize',14);
% legend('Input Data','Output Data');
% axis([0 500 35 45]);
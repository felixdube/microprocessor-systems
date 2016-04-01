clear all;
data = importdata('data1z.txt');
s = size(data);
number = 1:1:s;
dataformatted = [number', data];
str = sprintf('Kalman filter r = %f, q = %f, x = %f, p = %f, k = %f',dataformatted(1,3),dataformatted(1,4), dataformatted(1,5), dataformatted(1,6),dataformatted(1,7));
figure('units','normalized','outerposition',[0 0 1 1])
subplot(2,1,1),
plot(dataformatted(:,1),dataformatted(:,2),dataformatted(:,1),dataformatted(:,5));
title(str,'fontsize',18);
xlabel('measurement # (/25 for seconds)','fontsize',14);
ylabel('Acceleration in millig','fontsize',14);
legend('Input Data','Output Data');
axis([0 600 -1050 1050]);
data = importdata('data2z.txt');
s = size(data);
number = 1:1:s;
dataformatted = [number', data];
str = sprintf('Kalman filter r = %f, q = %f, x = %f, p = %f, k = %f',dataformatted(1,3),dataformatted(1,4), dataformatted(1,5), dataformatted(1,6),dataformatted(1,7));
subplot(2,1,2),
plot(dataformatted(:,1),dataformatted(:,2),dataformatted(:,1),dataformatted(:,5));
title(str,'fontsize',18);
xlabel('measurement # (/25 for seconds)','fontsize',14);
ylabel('Acceleration in millig','fontsize',14);
legend('Input Data','Output Data');
axis([0 600 -1050 1050]);
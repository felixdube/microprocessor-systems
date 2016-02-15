clear all, close all;
data = importdata('data.txt');
s = size(data);
number = 1:1:s;
dataformatted = [number', data];
figure;
%subplot(2,1,1),plot();

plot(dataformatted(:,1),400 * dataformatted(:,2)*3/4096 - 279,dataformatted(:,1),400 * dataformatted(:,5)*3/4096 - 279);
%plot(dataformatted(:,1),dataformatted(:,2),dataformatted(:,1),dataformatted(:,5));
title('Voltage','fontsize',18);
xlabel('measurement','fontsize',14);
ylabel('Temperature in degree C','fontsize',14);
legend('Input Data','Output Data');
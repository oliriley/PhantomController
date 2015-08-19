%clear all
close all
clc

pathString = 'C:\GitHub\PhantomController\cplusplus_codebase\Tests\TestWin32\Debug\'; %\RatData\
fileString = '999_10000000.txt';
addpath(pathString);
data = load(fileString);
rmpath(pathString);
data(:,1) = data(:,1)/1000; % Convert to seconds

figure(1)
plot(data(:,1),data(:,2),'Linewidth',2), box off, hold on
ylabel('Successes')
xlabel('Time (msec)')
%legend('Successes','Total trials')


figure(2)
subplot(3,1,1)
plot(data(:,1),data(:,3)), box off
subplot(3,1,2)
plot(data(:,1),data(:,4)), box off
subplot(3,1,3)
plot(data(:,1),data(:,5)), box off

figure(3)
plot3(data(:,3),data(:,4),data(:,5))
% MATLAB Script to Load and Plot GPS Receiver Position from solution.txt
clc; clear; close all;

% Load the data from solution.txt
data = load('solution.txt');

% Extract columns
epochTime = data(:,1);  % Epoch time
xPos = data(:,2);       % X coordinate
yPos = data(:,3);       % Y coordinate
zPos = data(:,4);       % Z coordinateG
clockBias = data(:,5);  % Receiver clock bias

% 1. Plot X, Y, and Z Position Over Time
figure;
plot(epochTime, xPos, 'r', 'LineWidth', 1.5); hold on;
plot(epochTime, yPos, 'g', 'LineWidth', 1.5);
plot(epochTime, zPos, 'b', 'LineWidth', 1.5);
xlabel('Epoch Time');
ylabel('Position (meters)');
title('Receiver Position Over Time');
legend('X Position', 'Y Position', 'Z Position');
grid on;
hold off;

% 2. 3D Position Trajectory Plot
figure;
plot3(xPos, yPos, zPos, 'b', 'LineWidth', 1.5);
xlabel('X Position (m)');
ylabel('Y Position (m)');
zlabel('Z Position (m)');
title('3D Position Trajectory');
grid on;
axis equal;

% 3. Plot Receiver Clock Bias Over Time
figure;
plot(epochTime, clockBias, 'k', 'LineWidth', 1.5);
xlabel('Epoch Time');
ylabel('Clock Bias (m)');
title('Receiver Clock Bias Over Time');
grid on;

% 4. Interactive 3D Scatter Plot (Color Coded by Epoch)
figure;
scatter3(xPos, yPos, zPos, 20, epochTime, 'filled');
xlabel('X Position (m)');
ylabel('Y Position (m)');
zlabel('Z Position (m)');
title('Receiver Position with Epoch Time Color Coding');
colorbar;
grid on;

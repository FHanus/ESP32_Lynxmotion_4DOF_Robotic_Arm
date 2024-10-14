% 3 DOF Robotic Arm Inverse Kinematics
clear;
clc;

%% Lynx Robot Parameters
% Link lengths
L1 = 0.1; 
L2 = 0.1; 
L3 = 0.05;

% Initial position
X = 0.15; 
Y = 0.1;  
phi = 0;

%% Create Figure and Sliders
fig = figure();
ax = axes('Parent', fig, 'Position', [0.1, 0.1, 0.75, 0.8]);

armPlot = plot(ax, 0, 0);
jointPlots = [];
desiredPosPlot = plot(ax, 0, 0);

grid(ax, 'on');
xlabel(ax, 'X Position (m)');
ylabel(ax, 'Y Position (m)');
xlim(ax, [-0.3, 0.3]); % Set x-axis limits
ylim(ax, [-0.1, 0.3]); % Set y-axis limits
title(ax, '3 DOF Lynx Sim');

sliderX = uicontrol('Parent', fig, 'Style', 'slider', ...
    'Units', 'normalized', 'Position', [0.25, 0.02, 0.5, 0.03], ...
    'Min', -0.15, 'Max', 0.25, 'Value', X);

sliderY = uicontrol('Parent', fig, 'Style', 'slider', ...
    'Units', 'normalized', 'Position', [0.02, 0.25, 0.03, 0.5], ...
    'Min', 0, 'Max', 0.2, 'Value', Y, ...
    'SliderStep', [0.01, 0.1]);

set(sliderX, 'Callback', @(src, event) updatePlot(src, sliderY, L1, L2, L3));
set(sliderY, 'Callback', @(src, event) updatePlot(sliderX, src, L1, L2, L3));

%% Initial Plot
updatePlot(sliderX, sliderY, L1, L2, L3);

%% Callback for sliders
function updatePlot(sliderX, sliderY, L1, L2, L3)
    X = get(sliderX, 'Value');
    Y = get(sliderY, 'Value');
 
    % Clear previous robot position
    cla(gca);
    
    % IK
    Xp = X - L3; 
    Yp = Y; 
   
    D = (Xp^2 + Yp^2 - L1^2 - L2^2) / (2 * L1 * L2);
       
    % Display an error message if unreachable
    if abs(D) > 1
        text(0, 0, 'Target Unreachable', 'Color', 'r', 'FontSize', 14, 'HorizontalAlignment', 'center');
        return;
    end
    
    theta2 = atan2(-sqrt(1 - D^2), D);
    theta1 = atan2(Yp, Xp) - atan2(L2 * sin(theta2), L1 + L2 * cos(theta2));
    theta3 = -(theta1 + theta2);
    
    % FK for visualisation
    x0 = 0;
    y0 = 0;
    
    x1 = x0 + L1 * cos(theta1);
    y1 = y0 + L1 * sin(theta1);
    
    x2 = x1 + L2 * cos(theta1 + theta2);
    y2 = y1 + L2 * sin(theta1 + theta2);
    
    x3 = x2 + L3 * cos(theta1 + theta2 + theta3);
    y3 = y2 + L3 * sin(theta1 + theta2 + theta3);
    
    % Plot
    hold(gca, 'on');
    plot(gca, [x0, x1], [y0, y1], 'r-', 'LineWidth', 2); % Link 1
    plot(gca, [x1, x2], [y1, y2], 'g-', 'LineWidth', 2); % Link 2
    plot(gca, [x2, x3], [y2, y3], 'b-', 'LineWidth', 2); % Link 3
    
    plot(gca, x0, y0, 'ko', 'MarkerSize', 10, 'MarkerFaceColor', 'k'); % Base
    plot(gca, x1, y1, 'ko', 'MarkerSize', 8, 'MarkerFaceColor', 'k'); % Joint 1
    plot(gca, x2, y2, 'ko', 'MarkerSize', 8, 'MarkerFaceColor', 'k'); % Joint 2
    plot(gca, x3, y3, 'ko', 'MarkerSize', 8, 'MarkerFaceColor', 'k'); % End Effector
    
    axis(gca, 'equal');
    grid(gca, 'on');

    xlim(gca, [-0.3, 0.3]);
    ylim(gca, [-0.1, 0.3]);
    
    theta1_deg = rad2deg(theta1);
    theta2_deg = rad2deg(theta2);
    theta3_deg = rad2deg(theta3);
    
    title(gca, sprintf('Theta1: %.2f°, Theta2: %.2f°, Theta3: %.2f°', theta1_deg, theta2_deg, theta3_deg));
end
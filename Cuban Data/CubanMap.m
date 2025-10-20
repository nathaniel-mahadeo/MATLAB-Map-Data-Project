data_raw = readtable('CubanMapData.xlsx');
data = table2cell(data_raw);

stateData = load('borderdata.mat');

[rows, cols] = size(data);

% Open a new figure and plot the continental United States outlined in black:
figure
bordersm
hold on

% Define a blue gradient: from light to dark
blueGradient = [
    0.8, 0.9, 1.0;   % very light blue (< 100)
    0.6, 0.8, 1.0;   % light blue     (< 500)
    0.4, 0.7, 1.0;   % medium-light   (< 1000)
    0.2, 0.5, 1.0;   % medium blue    (< 2000)
    0.1, 0.3, 0.9;   % medium-dark    (< 5000)
    0.05, 0.2, 0.8;  % dark blue      (< 10000)
    0.03, 0.1, 0.6;  % very dark blue (< 20000)
    0.0, 0.0, 0.4    % deepest blue   (> 20000)
];

for i = 1:rows
    sName = lower(data{i,1});
    sValue = data{i,2};

    if sum(strcmp(sName, lower(stateData.places)))
        if sValue < 100
            c = blueGradient(1,:);
        elseif sValue < 500
            c = blueGradient(2,:);
        elseif sValue < 1000
            c = blueGradient(3,:);
        elseif sValue < 2000
            c = blueGradient(4,:);
        elseif sValue < 5000
            c = blueGradient(5,:);
        elseif sValue < 10000
            c = blueGradient(6,:);
        elseif sValue < 20000
            c = blueGradient(7,:);
        else
            c = blueGradient(8,:);
        end

        bordersm(sName, 'facecolor', c)
    end
end

% Create a separate figure for the legend
figure
axis off
title('Refugee Count Legend', 'FontWeight', 'bold')

% Define positions and sizes for legend boxes
n = size(blueGradient, 1);
boxWidth = 1;
boxHeight = 1;
xStart = 1;
yPos = 1;

% Define labels
legendLabels = {
    '< 100', ...
    '100 - 499', ...
    '500 - 999', ...
    '1000 - 1999', ...
    '2000 - 4999', ...
    '5000 - 9999', ...
    '10000 - 19999', ...
    '≥ 20000'};

% Plot each color box with its label
for i = 1:n
    xPos = xStart + (i - 1) * (boxWidth + 0.5);
    rectangle('Position', [xPos, yPos, boxWidth, boxHeight], ...
              'FaceColor', blueGradient(i,:), ...
              'EdgeColor', 'k')
    text(xPos + boxWidth/2, yPos - 0.3, legendLabels{i}, ...
         'HorizontalAlignment', 'center', 'FontSize', 10)
end

% Adjust figure limits
xlim([0, xStart + n * (boxWidth + 0.5)])
ylim([0, 2])

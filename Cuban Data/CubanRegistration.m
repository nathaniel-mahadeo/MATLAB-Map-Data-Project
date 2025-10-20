% ---------- Weekly Data ---------- %
%% 1961
% Read data from the Excel file
projectDataCell = readcell('CubanResettlementfigures.xlsx');

% Extract relevant data
months1961 = projectDataCell(3:12, 1);
weeklyregistered1961 = projectDataCell(3:12, 2);
weeklyresettled1961 = projectDataCell(3:12, 5);

close all;
format long;

x1 = 1:length(months1961);

weeklyregistered1961 = cell2mat(weeklyregistered1961);
weeklyresettled1961 = cell2mat(weeklyresettled1961);

% Plot
figure;
plot(x1, weeklyregistered1961, '-o');
hold on
plot(x1, weeklyresettled1961, '-ro');
xlabel('Month');
ylabel('Registration vs Resettlement');
title('1961 Average Weekly Cuban Registration vs Resettlement');
legend('Registered', 'Resettled')
grid on;

% Use string month names as x-axis labels
xticks(x1);
xticklabels(months1961);

%% 1962
% Extract relevant data
months1962 = projectDataCell(14:25, 1);
weeklyregistered1962 = projectDataCell(14:25, 2);
weeklyresettled1962 = projectDataCell(14:25, 5);

x2 = 1:length(months1962);

weeklyregistered1962 = cell2mat(weeklyregistered1962);
weeklyresettled1962 = cell2mat(weeklyresettled1962);

% Plot
figure;
plot(x2, weeklyregistered1962, '-o');
hold on
plot(x2, weeklyresettled1962, '-ro');
xlabel('Month');
ylabel('Registration vs Resettlement');
title('1962 Average Weekly Cuban Registration vs Resettlement');
legend('Registered', 'Resettled')
grid on;

% Use string month names as x-axis labels
xticks(x2);
xticklabels(months1962);

%% 1963
% Extract relevant data
months1963 = projectDataCell(27:37, 1);
weeklyregistered1963 = projectDataCell(27:37, 2);
weeklyresettled1963 = projectDataCell(27:37, 5);

x3 = 1:length(months1963);

weeklyregistered1963 = cell2mat(weeklyregistered1963);
weeklyresettled1963 = cell2mat(weeklyresettled1963);

% Plot
figure;
plot(x3, weeklyregistered1963, '-o');
hold on
plot(x3, weeklyresettled1963, '-ro');
xlabel('Month');
ylabel('Registration vs Resettlement');
title('1963 Average Weekly Cuban Registration vs Resettlement');
legend('Registered', 'Resettled')
grid on;

% Use string month names as x-axis labels
xticks(x3);
xticklabels(months1963);

% ---------- Monthly Data ---------- %
%% 1961
% Extract relevant data
months1961 = projectDataCell(40:49, 1);
monthlyregistered1961 = projectDataCell(40:49, 2);
monthlyresettled1961 = projectDataCell(40:49, 5);

x1 = 1:length(months1961);

monthlyregistered1961 = cell2mat(monthlyregistered1961);
monthlyresettled1961 = cell2mat(monthlyresettled1961);

ytickformat('%.0f'); % Display Y-axis tick labels as full numbers
% Plot
figure;
plot(x1, monthlyregistered1961, '-o');
hold on
plot(x1, monthlyresettled1961, '-ro');
xlabel('Month');
ylabel('Registration vs Resettlement');
title('1961 Monthly Cuban Registration vs Resettlement');
legend('Registered', 'Resettled')
grid on;

% Use string month names as x-axis labels
xticks(x1);
xticklabels(months1961);
ax = gca;
ax.YAxis.Exponent = 0;   % Suppress exponent/scientific notation

%% 1962
% Extract relevant data
months1962 = projectDataCell(51:62, 1);
monthlyregistered1962 = projectDataCell(51:62, 2);
monthlyresettled1962 = projectDataCell(51:62, 5);

x1 = 1:length(months1962);

monthlyregistered1962 = cell2mat(monthlyregistered1962);
monthlyresettled1962 = cell2mat(monthlyresettled1962);

ytickformat('%.0f'); % Display Y-axis tick labels as full numbers
% Plot
figure;
plot(x1, monthlyregistered1962, '-o');
hold on
plot(x1, monthlyresettled1962, '-ro');
xlabel('Month');
ylabel('Registration vs Resettlement');
title('1962 Monthly Cuban Registration vs Resettlement');
legend('Registered', 'Resettled')
grid on;

% Use string month names as x-axis labels
xticks(x1);
xticklabels(months1962);
ax = gca;
ax.YAxis.Exponent = 0;   % Suppress exponent/scientific notation

%% 1963
% Extract relevant data
months1963 = projectDataCell(64:74, 1);
monthlyregistered1963 = projectDataCell(64:74, 2);
monthlyresettled1963 = projectDataCell(64:74, 5);

x1 = 1:length(months1963);

monthlyregistered1963 = cell2mat(monthlyregistered1963);
monthlyresettled1963 = cell2mat(monthlyresettled1963);

ytickformat('%.0f'); % Display Y-axis tick labels as full numbers
% Plot
figure;
plot(x1, monthlyregistered1963, '-o');
hold on
plot(x1, monthlyresettled1963, '-ro');
xlabel('Month');
ylabel('Registration vs Resettlement');
title('1963 Monthly Cuban Registration vs Resettlement');
legend('Registered', 'Resettled')
grid on;

% Use string month names as x-axis labels
xticks(x1);
xticklabels(months1963);
ax = gca;
ax.YAxis.Exponent = 0;   % Suppress exponent/scientific notation
data = csvread('tracer_data.csv');

% Extract the columns directly
Vce = sweep_data(:, 1);
Ib  = sweep_data(:, 2) * 1e6;
Ic  = sweep_data(:, 3) * 1e3;

figure;
% scatter3(X, Y, Z, dot_size, color_mapping, style)
scatter3(Vce, Ib, Ic, 15, Ic, 'filled');

xlabel('V_{CE} (Volts)');
ylabel('I_B (\muA)');
zlabel('I_C (mA)');
title('2N2222 BJT 3D Raw Data');

grid on;
colormap('jet');
view(45, 30);

clear all
close all
clc

figure (1)

%axis ([0 50 3 7])
ax=gca;
set(ax, 'FontSize',14)
grid on
box on

load mp.out
plot(mp(:,1),mp(:,2))

xlabel('moment[kN*m]')
ylabel('force[kN]')
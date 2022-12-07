close all; clear all; clc

load stress37.out
load strain37.out
box on
grid 
set(0,'defaultaxesfontsize',20);
plot(strain37(:,4), stress37(:,4))
xlabel('strain')
ylabel('stress(kPa)')

figure (2)

load force79.out
load deformation79.out
box on
grid 
set(0,'defaultaxesfontsize',20);
plot(deformation79(:,3), force79(:,3))
xlim([-0.005  0.006])
xlabel('curvature(1/m)')
ylabel('moment(kN*m)')
%=========================================================================%
%  Spd2Acc.m                                         Module of MOVESTAR   %
%                                                                         %
%  Script for calculate the second-by-second acceleration using the       %
%                                              Central Difference Method. %
%																		  %
%  Version of 08-10-2020             Copyright by Guoyuan Wu & Ziran Wang %
%  University of California, Riverside, USA								  %
%  gywu@cert.ucr.edu, ryanwang11@hotmail.com							  %
%=========================================================================%

function [Acc] = Spd2Acc(Speed)   % speed unit is m/s and acceleration is m/s^2

% Use central difference method (N=3) to estimate the acceleration from
% speed data
Acc = zeros(length(Speed), 1);
for i = 2:(length(Speed)-1)
    Acc(i) = (Speed(i+1) - Speed(i-1))/2;
end
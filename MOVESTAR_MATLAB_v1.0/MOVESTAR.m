%=========================================================================%
%  MOVESTAR.m                                 Main Function of MOVESTAR   %
%                                                                         %
%  Script for calculating fuel consumption and pollutant emissions.       %
%																		  %
%  Version of 08-10-2020             Copyright by Guoyuan Wu & Ziran Wang %
%  University of California, Riverside, USA								  %
%  gywu@cert.ucr.edu, ryanwang11@hotmail.com							  %
%=========================================================================%

%Run the example file by entering in MATLAB command window:
% MOVESTAR(1, "test.csv")

function [] = MOVESTAR(vehType, fn)
%MOVESTAR is to calculate the criteria pollutant emission based on MOVES
%model and second-by-second vehicle velocity profile.

% Initialization of parameters and variables
c1 = 1609.34;       % 1 mile = 1609.34 m
Ems_sum = zeros(1, 8);
tt_sum = 0;
td_sum = 0;

% Read the data     % Unit: speed -- m/s
[speed] = textread(fn, '%*s%*s%*s%*s%*s%*s%*s%*s%*s%*s%*s%*s%*s%*s%*s%*s%*s%*s%n%*[^\n]',...
'delimiter', ',', 'headerlines', 1);
tt = length(speed);         % travel time -- sec
td = sum(speed);            % travel distance -- m

% Calculate the acceleration/deceleration
[Acc] = Spd2Acc(speed);                                         % Unit: Acc -- m/s^2

% Calculate the VSP/STP (assuming roadway grade is zero)
if vehType == 1             % Light duty vehicle (passenger car)
    A = 0.156461;
    B = 0.002002;
    C = 0.000493;
    M = 1.4788;
    f = 1.4788;
elseif vehType == 2         % Light duty truck (passenger truck)
    A = 0.22112;
    B = 0.002838;
    C = 0.000698;
    M = 1.86686;
    f = 1.86686;
else
    disp('Please restrict the vehicle type into either 1 or 2');
end

VSP = (A*speed + B*speed.^2 + C*speed.^3 + M*Acc.*speed)/f;      % Scaled Tractive Power: Unit: speed -- m/s; acceleration -- m/s^2 and VSP -- kWatt/tonne (all for the purpose of VSP calculation) No consideration of road grade

% Obtain the OpMode distribution
[size_bin] = OMCal(speed, Acc, VSP);                                   % Unit: speed -- m/s, Acc -- m/s^2 and VSP -- kWatt/tonne

% Estimate the emissions
[Ems_rate] = EmsRateCal(size_bin, vehType);
    
Fuel_rate = Ems_rate(7)*12/44*13.78/12;
Ems_sum = [Ems_rate Fuel_rate];
tt_sum = tt_sum + tt;
td_sum = td_sum + td;

% Output emission rate and travel time
output_fn1 = strcat(fn, '_ER', '.dat');
fid1 = fopen(output_fn1, 'w');
fprintf(fid1, 'CO(g),HC(g),NOx(g),PM2.5_Ele(g),PM2.5_Org(g),Energy(KJ),CO2(g),Fuel(g),TT(s)\n');
for i = 1:8
    fprintf(fid1, '%g,', Ems_sum(i));
end
fprintf(fid1, '%d\n', tt_sum);
fclose(fid1);

% Output emission factor and travel time
output_fn2 = strcat(fn, '_EF', '.dat');
fid2 = fopen(output_fn2, 'w');
fprintf(fid2, 'CO(g/mi),HC(g/mi),NOx(g/mi),PM2.5_Ele(g/mi),PM2.5_Org(g/mi),Energy(KJ/mi),CO2(g/mi),Fuel(g/mi),TD(mi)\n');
for i = 1:8
    fprintf(fid2, '%g,', Ems_sum(i)*c1/td_sum);
end
fprintf(fid2, '%d\n', td_sum/c1);
fclose(fid2);
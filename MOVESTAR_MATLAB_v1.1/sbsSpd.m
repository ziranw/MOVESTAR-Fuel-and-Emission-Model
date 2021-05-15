%=========================================================================%
%  sbsSpd.m                                          Module of MOVESTAR   %
%                                                                         %
%  Script for aggregating sub-second data from Simulink into              %
%                             into second-by-second speed trajectory.     %
%																		  %
%  Version of 08-10-2020             Copyright by Guoyuan Wu & Ziran Wang %
%  University of California, Riverside, USA								  %
%  gywu@cert.ucr.edu, ryanwang11@hotmail.com							  %
%=========================================================================%

% This program is to aggregate sub-second data from Simulink and aggregate
% into sbs speed trajectory
function [] = sbsSpd(fileName)

output_fn = strcat(fileName,'_', 'sbs', '.txt');
fid = fopen(output_fn, 'w');
fprintf(fid, 'Time(sec),Speed_Act(m/s)\n');

load(strcat(fileName, '.mat'))
Data = temp_data;

% Data column:
% 1 - timestamp (sec); 2 actual speed (mps);
Time_fl = floor(Data(:,3));
Time_uniq = unique(Time_fl);
m = length(Time_uniq);

for i = 1:m
    spd_Act = mean(Data(find(Time_fl==Time_uniq(i)),4));
    if i~=m
        fprintf(fid,'%d,%4.2f\n', Time_uniq(i),spd_Act);
    else
        fprintf(fid,'%d,%4.2f', Time_uniq(i),spd_Act);
    end
end
fclose(fid);
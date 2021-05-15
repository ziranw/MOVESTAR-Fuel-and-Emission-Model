%=========================================================================%
%  OMCal.m                                           Module of MOVESTAR   %
%                                                                         %
%  Script for calculating the operating mode of a vehicle.                %
%																		  %
%  Version of 08-10-2020             Copyright by Guoyuan Wu & Ziran Wang %
%  University of California, Riverside, USA								  %
%  gywu@cert.ucr.edu, ryanwang11@hotmail.com							  %
%=========================================================================%

% This program is a sub-function to be called to calculate the Operating
% Mode defined by MOVES. Input data include second-by-second speed vector 
% (mph), second-by-second acceleration vector (mph/s) and second-by-second
% VSP value (kWatt/tonne). Please note that each vector should be a
% "snippet" of individual vehicle only. The output vector is the number of
% samples for each bin with respect to the bin mode definition by MOVES, 
% say, [0 1 11 12 13 14 15 16 21 22 23 24 25 27 28 29 30 33 35 37 38 39 40]

function [size_bin] = OMCal(Spd, Acc, VSP)      % Unit here: Spd -- m/s, Acc -- m/s^2 and VSP -- kWatt/tonne

c1 = 2.23693629;        % conversion factor for speed from m/s to mph and acceleration from m/s^2 to mph/s
size_bin = zeros(23,1); % vector of sample size for each of 23 operating mode 

for i = 1:length(Spd)
    if Acc(i)*c1<=-2
        size_bin(1) = size_bin(1)+1;        % deceleration/braking
    else
        if i>=3 & Acc(i-2)*c1<-1 & Acc(i-1)*c1<-1
            size_bin(1) = size_bin(1)+1;    % deceleration/braking
        else
            if Spd(i)*c1 < 1
                size_bin(2) = size_bin(2)+1;    %Idle
            else
                if Spd(i)*c1 < 25
                    if VSP(i) < 0
                        size_bin(3) = size_bin(3)+1;
                    elseif VSP(i) < 3
                        size_bin(4) = size_bin(4)+1;
                    elseif VSP(i) < 6
                        size_bin(5) = size_bin(5)+1;
                    elseif VSP(i) < 9
                        size_bin(6) = size_bin(6)+1;
                    elseif VSP(i) < 12
                        size_bin(7) = size_bin(7)+1;
                    else
                        size_bin(8) = size_bin(8)+1;
                    end
                else
                    if Spd(i)*c1 < 50
                        if VSP(i) < 0
                            size_bin(9) = size_bin(9)+1;
                        elseif VSP(i) < 3
                            size_bin(10) = size_bin(10)+1;
                        elseif VSP(i) < 6
                            size_bin(11) = size_bin(11)+1;
                        elseif VSP(i) < 9
                            size_bin(12) = size_bin(12)+1;
                        elseif VSP(i) < 12
                            size_bin(13) = size_bin(13)+1;
                        elseif VSP(i) < 18
                            size_bin(14) = size_bin(14)+1;
                        elseif VSP(i) < 24
                            size_bin(15) = size_bin(15)+1;
                        elseif VSP(i) < 30
                            size_bin(16) = size_bin(16)+1;
                        else
                            size_bin(17) = size_bin(17)+1;
                        end
                    else
                        if VSP(i) < 6
                            size_bin(18) = size_bin(18)+1;
                        elseif VSP(i) < 12
                            size_bin(19) = size_bin(19)+1;
                        elseif VSP(i) < 18
                            size_bin(20) = size_bin(20)+1;
                        elseif VSP(i) < 24
                            size_bin(21) = size_bin(21)+1;
                        elseif VSP(i) < 30
                            size_bin(22) = size_bin(22)+1;
                        else
                            size_bin(23) = size_bin(23)+1;
                        end
                    end
                end
            end
        end                   
    end
end
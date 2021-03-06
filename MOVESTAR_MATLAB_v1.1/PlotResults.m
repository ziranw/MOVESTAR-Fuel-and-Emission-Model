%=========================================================================%
%  PlotResults.m                          Plotting Function of MOVESTAR   %
%                                                                         %
%  Script for plotting results of fuel consumption and pollutant emissions%
%																		  %
%  Version of 08-18-2020                  Copyright by Ziran Wang         %
%  University of California, Riverside, USA								  %
%  ryanwang11@hotmail.com                                                 %
%=========================================================================%

% Attention: the file name in the "xlsread" function needs to be specified

close all
clear

FONTSIZE = 30;

% Read files and convert from mile to km
base = xlsread('Output_All_EgoVeh1_baseline.csv_EF.csv') * 1.61;
model = xlsread('Output_All_EgoVeh1_ADAS.csv_EF.csv') * 1.61;

% Improvement by applying the model  
change = (base - model)./base;

% Plot CO
figure(1)
b = bar(categorical({'Baseline', 'Model'}), [base(1) model(1)]);
b.FaceColor = '#0072BD';

title('CO of Ego Vehicle','fontsize',FONTSIZE);
ylabel('CO (g/km)','fontsize', FONTSIZE);
set(gcf,'Position',[1080 360 600 600]);
% set(gca,'Position',[.13 .17 .75 .74]);
set(gca,'FontSize', 30);
set(gca, 'LooseInset', [0,0,0,0]);
saveas(gcf,'CO.png')

% Plot HC
figure(2)
b = bar(categorical({'Baseline', 'Model'}), [base(2) model(2)]);
b.FaceColor = '#D95319';

title('HC of Ego Vehicle','fontsize',FONTSIZE);
ylabel('HC (g/km)','fontsize', FONTSIZE);
set(gcf,'Position',[1080 360 600 600]);
% set(gca,'Position',[.13 .17 .75 .74]);
set(gca,'FontSize', 30);
set(gca, 'LooseInset', [0,0,0,0]);
saveas(gcf,'HC.png')

% Plot NOx
figure(3)
b = bar(categorical({'Baseline', 'Model'}), [base(3) model(3)]);
b.FaceColor = '#EDB120';

title('NOx of Ego Vehicle','fontsize',FONTSIZE);
ylabel('NOx (g/km)','fontsize', FONTSIZE);
set(gcf,'Position',[1080 360 600 600]);
% set(gca,'Position',[.13 .17 .75 .74]);
set(gca,'FontSize', 30);
set(gca, 'LooseInset', [0,0,0,0]);
saveas(gcf,'NOx.png')

% Plot PM2.5 Elementary Carbon
figure(4)
b = bar(categorical({'Baseline', 'Model'}), [base(4) model(4)]);
b.FaceColor = '#7E2F8E';

title('PM2.5 of Ego Vehicle','fontsize',FONTSIZE);
ylabel('PM2.5 (g/km)','fontsize', FONTSIZE);
set(gcf,'Position',[1080 360 600 600]);
% set(gca,'Position',[.13 .17 .75 .74]);
set(gca,'FontSize', 30);
set(gca, 'LooseInset', [0,0,0,0]);
saveas(gcf,'PM2.5_Ele.png')

% Plot PM2.5 Organic Carbon
figure(5)
b = bar(categorical({'Baseline', 'Model'}), [base(5) model(5)]);
b.FaceColor = '#7E2F8E';

title('PM2.5 of Ego Vehicle','fontsize',FONTSIZE);
ylabel('PM2.5 (g/km)','fontsize', FONTSIZE);
set(gcf,'Position',[1080 360 600 600]);
% set(gca,'Position',[.13 .17 .75 .74]);
set(gca,'FontSize', 30);
set(gca, 'LooseInset', [0,0,0,0]);
saveas(gcf,'PM2.5_Org.png')

% Plot Energy
figure(6)
b = bar(categorical({'Baseline', 'Model'}), [base(6) model(6)]);
b.FaceColor = '#77AC30';

title('Energy of Ego Vehicle','fontsize',FONTSIZE);
ylabel('Energy (KJ/km)','fontsize', FONTSIZE);
set(gcf,'Position',[1080 360 600 600]);
% set(gca,'Position',[.13 .17 .75 .74]);
set(gca,'FontSize', 30);
set(gca, 'LooseInset', [0,0,0,0]);
saveas(gcf,'Energy.png')

% Plot CO2
figure(7)
b = bar(categorical({'Baseline', 'Model'}), [base(7) model(7)]);
b.FaceColor = '#4DBEEE';

title('CO2 of Ego Vehicle','fontsize',FONTSIZE);
ylabel('CO2 (g/km)','fontsize', FONTSIZE);
set(gcf,'Position',[1080 360 600 600]);
% set(gca,'Position',[.13 .17 .75 .74]);
set(gca,'FontSize', 30);
set(gca, 'LooseInset', [0,0,0,0]);
saveas(gcf,'CO2.png')

% Plot Fuel
figure(8)
b = bar(categorical({'Baseline', 'Model'}), [base(8) model(8)]);
b.FaceColor = '#A2142F';

title('Fuel of Ego Vehicle','fontsize',FONTSIZE);
ylabel('Fuel (g/km)','fontsize', FONTSIZE);
set(gcf,'Position',[1080 360 600 600]);
% set(gca,'Position',[.13 .17 .75 .74]);
set(gca,'FontSize', 30);
set(gca, 'LooseInset', [0,0,0,0]);
saveas(gcf,'Fuel.png')
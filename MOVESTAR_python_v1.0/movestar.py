# -*- coding: utf-8 -*-
"""
Created on Aug 28 2020
Updated on May 14 2021

@Authors: Kinjal Dand & Ziran Wang
@Email: ryanwang11@hotmail.com
@Website: http://ziranw.github.io
@Repo: https://github.com/ziranw/MOVESTAR-Fuel-and-Emission-Model

"""

import pandas as pd
import numpy as np
import os
import math
BASE_DIR = os.path.dirname(os.path.abspath(__file__))

def sbs_spd(data):
    Time_fl = math.floor(Data[:,2])
    Time_uniq = np.unique(Time_fl)
    m = len(Time_uniq)
    res=[]
    for i in range(m):
        spd_Act = mean(Data[Data[Time_fl==Time_uniq[i]]][3]);
        res.append((Time_uniq(i),spd_Act))
    resdf=pd.Dataframe(res,columns=['Time(sec)','Speed_Act(m/s)'])
    resdf.to_csv('resdf'+'_'+ 'sbs'+'.csv')
    

    

def ems_rate_cal(Bin_data, veh_type):
    #print(Bin_data)
    ems_file = os.path.join(BASE_DIR,'EmsRate_'+str(veh_type)+'.csv')
    if os.path.exists(ems_file) ==False:
        return 'Invalid File Type',False
    
    df = pd.read_csv(ems_file,header=None)
    df = df/3600
    #print("EMS Table Columns: ",len(df.columns))
    #print(df)
    #Ems_rate = Bin_data*df[:, 0:7]
    Bin_data = Bin_data.reshape(-1,1)
    Ems_rate = np.dot(Bin_data.T,df.loc[:, 1:])
    
    #print("Shape for Bin_data : " ,Bin_data.shape)
    #print("Shape for Ems_rate : " ,Ems_rate.shape)
    Ems_rate = Ems_rate[0][:]
    #print("Ems_rate : ",Ems_rate)
    return Ems_rate,True
    


def OMCal(Spd, Acc, VSP):      # Unit here: Spd -- m/s, Acc -- m/s^2 and VSP -- kWatt/tonne

    c1 = 2.23693629        # conversion factor for speed from m/s to mph and acceleration from m/s^2 to mph/s
    size_bin = np.zeros(23) # vector of sample size for each of 23 operating mode
    maxVSP = np.max(VSP)+1  # To satisfy default cond
    
    for i in range(len(Spd)):
        sc1 = Spd[i]*c1
        binselector = 0
        if Acc[i]*c1<=-2:
            binselector = 1        # deceleration/braking
        elif i>=3 and Acc[i-2]*c1<-1 and Acc[i-1]*c1<-1:
                binselector = 1    # deceleration/braking
        elif sc1 < 1:
                binselector = 2    #Idle
        else:
            if sc1 < 25:
                map = [0,3,6,9,12,maxVSP]
                binoffset = 3
            elif sc1 < 50:
                map = [0,3,6,9,12,18,24,30,maxVSP]
                binoffset = 9
            else:
                map = [6,12,18,24,30,maxVSP]
                binoffset = 18
            for index,vsp_cur in enumerate(map):
                if VSP[i] < vsp_cur:
                    binselector = binoffset +index
                    break
        binselector = binselector - 1 # to convert 1-based indexing to 0-based indexing
        size_bin[binselector] = size_bin[binselector]+1
    
    return size_bin
                        


def get_derivative(input_array):
    output_array = np.zeros(len(input_array))
    for i in range(1,len(input_array)):
        output_array[i] = input_array[i] - input_array[i-1]
    return output_array
    

def Spd2Acc(Speed):   # speed unit is m/s and acceleration is m/s^2

    # Use central difference method (N=3) to estimate the acceleration from speed data
    acc = np.zeros(len(Speed))
    for i in range(1,(len(Speed)-1)):
        acc[i] = (Speed[i+1] - Speed[i-1])/2
    return acc
    
def Acc2Jerk(Acc):   # acceleration is m/s^2 and jerk is m/s^3

    # Use central difference method (N=3) to estimate the jerk from acceleration data
    return get_derivative(Acc)    

def movestar(veh_type,speed):
    if veh_type not in [1,2]:
        return {'error':'Please enter vehicle type as 1 or 2'}
    speed = np.array(speed)
    outputres={}
    #movestar is to calculate the criteria pollutant emission based on MOVES
    #model and second-by-second vehicle velocity profile.

    # Initialization of parameters and variables
    c1 = 1609.34# 1 mile = 1609.34 m
    Ems_sum = np.zeros(8)
    tt_sum = 0
    td_sum = 0

    # Read the data     % Unit: speed -- m/s
    

    # Following segment is used to transfer speed into m/s
    speed = speed / 3.6

    # Following segment is used to process sub-second data
    
    #time_int = np.mod(time, 1) < 0.001   # Pick out the row when time is integer
    #print(time_int)
    #speed = speed[time_int]# Change speed into second-by-second
    #outputres["speed"] = speed.tolist()

    tt = len(speed)# travel time -- sec
    td = np.sum(speed)# travel distance -- m

    # Calculate the acceleration/deceleration
    Acc = Spd2Acc(speed)# Unit: Acc -- m/s^2
    df_vehicle_src_coeff = pd.read_csv(os.path.join(BASE_DIR,'VehicleSrcCoeff.csv'))
    
    row = df_vehicle_src_coeff[df_vehicle_src_coeff['VehicleType'] == veh_type]
# Calculate the VSP/STP (assuming roadway grade is zero)

    A = row['A'].values
    B = row['B'].values
    C = row['C'].values
    M = row['M'].values
    f = row['f'].values
    #outputres["Coeff"] = [A,B,C,M,f]

    VSP = A * speed + B * speed**2 + C * speed**3 + M * np.multiply(Acc , speed)
    VSP = VSP/ f  # Scaled Tractive Power: Unit: speed -- m/s; acceleration -- m/s^2 and VSP -- kWatt/tonne (all for the purpose of VSP calculation) No consideration of road grade
    #outputres["VSP"] = VSP.tolist()
    # Obtain the OpMode distribution
    size_bin = OMCal(speed, Acc, VSP)# Unit: speed -- m/s, Acc -- m/s^2 and VSP -- kWatt/tonne

    #outputres["size_bin"] = size_bin.tolist()
    # Estimate the emissions
    Ems_rate,status = ems_rate_cal(size_bin, veh_type)
    #outputres["Ems_rate"] = Ems_rate.tolist()
    if status == False:
        #print(Ems_rate)
        return status

    Fuel_rate = Ems_rate[6] * 12 / 44 * 13.78 / 12
    Ems_sum = np.append(Ems_rate,Fuel_rate)
    #outputres["Ems_sum"] = Ems_sum.tolist()
    tt_sum = tt_sum + tt
    td_sum = td_sum + td

    # Output emission rate and travel time
    outputcol='CO(g),HC(g),NOx(g),PM2.5_Ele(g),PM2.5_Org(g),Energy(KJ),CO2(g),Fuel(g),TT(s)'
    outputcol = outputcol.split(',')
    output_val = Ems_sum
    output_val = np.append(output_val,td_sum)
    
    dfER = pd.DataFrame(output_val).T
    dfER.columns=outputcol
    #outputres["Res ER"]={}
    #outputres["Res ER"]['Columns'] = list(dfER.columns)
    outputres["Emission Rate"] = dfER.values.tolist()
    
    outputcol='CO(g/mi),HC(g/mi),NOx(g/mi),PM2.5_Ele(g/mi),PM2.5_Org(g/mi),Energy(KJ/mi),CO2(g/mi),Fuel(g/mi),TD(mi)'
    outputcol = outputcol.split(',')
    output_val = Ems_sum*c1/td_sum
    output_val = np.append(output_val,(td_sum/c1))
    dfEF = pd.DataFrame(output_val).T
    dfEF.columns=outputcol
    #outputres["Res EF"]={}
    #outputres["Res EF"]['Columns'] = list(dfEF.columns)
    outputres["Emission Factor"] = dfEF.values.tolist()
    print(outputres)
    return outputres

if __name__ =="__main__":
    df = pd.read_csv('test.csv')
    speed = df['Speed'].values
    time = df['Time'].values
    veh_type = 1
    #print({'speed':speed,'vehicle_type':veh_type})
    movestar(veh_type,speed)

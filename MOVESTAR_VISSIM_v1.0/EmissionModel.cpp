/*========================================================================= */
/* EmissionModel.cpp                     VISSIM C++ API version of MOVESTAR */
/*																			*/
/* Script for calculating fuel consumptionand pollutant emissions.	        */
/*																			*/
/* Version of 08-10-2020               Copyright by Guoyuan Wu & Ziran Wang */
/* University of California, Riverside, USA									*/
/* gywu@cert.ucr.edu, ryanwang11@hotmail.com								*/
/*========================================================================= */

/// The detailed steps regarding how to implement the EmissionModel.dll into
/// PTV VISSIM should be referred to VISSIM manual.

#include "EmissionModel.h"
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <map>
#include <cmath>
using namespace std;


//calculate VSP function
double calculateVSP(long type, double velocity, double acceleration)
{
	vector<double> vehicleAttributes;

	if (type == 100) // Vehicle type is light-duty passenger car (source type 21)
	{
		vehicleAttributes = { 0.156461,  0.002002, 0.000493, 1.4788, 1.4788 };
	}

	else if (type == 200) // Vehicle type is transit bus (source type 42)
	{
		vehicleAttributes = { 1.0944, 0, 0.003587, 16.556, 17.1 };
	}

	else if (type == 300) // Vehicle type is combination short-haul truck (source type 61)
	{
		vehicleAttributes = { 1.96354, 0, 0.004031, 29.3275, 17.1 };
	}

	double A = vehicleAttributes.at(0);  //Rolling (kW-s/m)
	double B = vehicleAttributes.at(1);  //Rotating (Kw-s**2/m**2)
	double C = vehicleAttributes.at(2);  //Drag (kW-s**3/m**3)
	double M = vehicleAttributes.at(3);  //Source Mass (metric tons)
	double F = vehicleAttributes.at(4);  //Fixed Mass Factor (metric tons)

	double VSP = ((A * velocity) + (B * pow(velocity, 2)) + (C * pow(velocity, 3)) +
		M * (acceleration)* velocity) / F;
	return VSP;
}

//determines and returns the opmode
int getOpmode(vector<double> accelerations, double velocity, double VSP)
{
	int OpMode = -99999;

	if (accelerations.size() > 0)
	{
		if (accelerations.at(accelerations.size() - 1) <= -2.0)
		{
			OpMode = 0;
			return OpMode;
		}
	}

	if (accelerations.size() == 3)
	{
		if (accelerations.at(2) < -1.0 && accelerations.at(1) < -1.0 && accelerations.at(0) < -1.0)
		{
			OpMode = 0;
			return OpMode;
		}
	}

	if (-1.0 <= velocity && velocity < 1.0)
	{
		OpMode = 1;
	}
	else if (velocity >= 50)
	{
		if (VSP >= 30)
		{
			OpMode = 40;
		}

		else if (VSP >= 24)
		{
			OpMode = 39;
		}

		else if (VSP >= 18)
		{
			OpMode = 38;
		}

		else if (VSP >= 12)
		{
			OpMode = 37;
		}

		else if (VSP >= 6)
		{
			OpMode = 35;
		}

		else
		{
			OpMode = 33;
		}
	}

	else if (velocity >= 25)
	{
		if (VSP >= 30)
		{
			OpMode = 30;
		}

		else if (VSP >= 24)
		{
			OpMode = 29;
		}

		else if (VSP >= 18)
		{
			OpMode = 28;
		}

		else if (VSP >= 12)
		{
			OpMode = 27;
		}

		else if (VSP >= 9)
		{
			OpMode = 25;
		}

		else if (VSP >= 6)
		{
			OpMode = 24;
		}

		else if (VSP >= 3)
		{
			OpMode = 23;
		}

		else if (VSP >= 0)
		{
			OpMode = 22;
		}

		else
		{
			OpMode = 21;
		}
	}

	else if (velocity >= 0)
	{
		if (VSP >= 12)
		{
			OpMode = 16;
		}

		else if (VSP >= 9)
		{
			OpMode = 15;
		}

		else if (VSP >= 6)
		{
			OpMode = 14;
		}

		else if (VSP >= 3)
		{
			OpMode = 13;
		}

		else if (VSP >= 0)
		{
			OpMode = 12;
		}

		else
		{
			OpMode = 11;
		}
	}
	return OpMode;
};

//dictionary for source type 100
map<int, vector<double> > sourceType100Dictionary
{
	{ 0 ,{ 5.65127,57.1901,3.12064,3689.52,51336.2,0.162388 } },
	{ 1 ,{ 4.16103,65.203,3.72146,3303.21,45961.5,0.14099 } },
	{ 11,{ 4.49873,72.2148,5.05498,5065.18,70477.3,0.140804 } },
	{ 12,{ 5.49957,114.721,10.434,6792.71,94514.1,0.181153 } },
	{ 13,{ 6.9986,166.291,21.2052,9898.89,137734,0.304719 } },
	{ 14,{ 8.61404,236.629,36.8724,12754,177459,0.368023 } },
	{ 15,{ 10.289,290.701,54.5185,15525.4,216021,0.511385 } },
	{ 16,{ 12.292,353.082,77.7222,19353,269278,1.37684 } },
	{ 21,{ 6.39121,121.101,12.4625,6552.97,91178.8,0.269425 } },
	{ 22,{ 6.06055,134.493,14.3454,7795.24,108464,0.277939 } },
	{ 23,{ 7.08482,175.239,22.3094,9860,137193,0.328926 } },
	{ 24,{ 9.14126,250.723,38.98,12792.3,177992,0.403531 } },
	{ 25,{ 10.3794,301.015,55.9756,16526.8,229954,0.43782 } },
	{ 27,{ 14.5963,505.205,89.8475,21600.8,300555,0.833802 } },
	{ 28,{ 20.9733,860.458,123.158,29132.2,405348,3.81746 } },
	{ 29,{ 30.9063,1510.07,155.893,39794.4,553702,12.1245 } },
	{ 30,{ 55.4631,3383.72,217.391,49616,690361,29.7988 } },
	{ 33,{ 6.40282,112.342,17.4164,9969.44,138715,0.928642 } },
	{ 35,{ 8.5886,214.588,52.3698,15832.7,220297,0.922485 } },
	{ 37,{ 10.2377,287.955,75.6242,20520.9,285528,1.01152 } },
	{ 38,{ 14.6403,560.062,104.028,26654.3,370869,1.75908 } },
	{ 39,{ 24.3958,906.369,143.246,35381.3,492297,4.48958 } },
	{ 40,{ 39.5436,2442.32,198.037,44859.7,624180,5.2443 } }
};

//dictionary for source type 200
map<int, vector<double> > sourceType200Dictionary
{
	{ 0 ,{ 19.236,92.4235,121.303,15326.9,210592,4.41943 } },
	{ 1 ,{ 17.9677,49.2354,281.303,7709.02,106380,4.78183 } },
	{ 11,{ 25.2863,155.676,169.805,10345.9,142771,10.4614 } },
	{ 12,{ 28.9061,201.415,560.734,29799.4,410370,14.5674 } },
	{ 13,{ 42.3189,580.049,828.318,56032.8,775733,38.1777 } },
	{ 14,{ 52.2232,865.575,1097.32,82582.2,1.14554e+006,54.6949 } },
	{ 15,{ 58.8956,1176.28,1281.25,105324,1.46341e+006,77.8943 } },
	{ 16,{ 66.5306,1636.93,1424.7,145359,2.02099e+006,78.1251 } },
	{ 21,{ 28.4361,171.305,162.769,8642.64,120102,15.7278 } },
	{ 22,{ 32.9324,246.345,706.252,38161.6,525130,31.8557 } },
	{ 23,{ 38.1365,576.677,1069.67,64843.7,896698,38.5261 } },
	{ 24,{ 44.0335,820.389,1428.83,92746.5,1.2795e+006,62.3961 } },
	{ 25,{ 48.7164,1090.84,1600.59,119827,1.65468e+006,87.9118 } },
	{ 27,{ 57.3304,1551.6,2456.08,165851,2.29116e+006,124.826 } },
	{ 28,{ 67.1679,2050.35,3438.51,232192,3.20761e+006,200.882 } },
	{ 29,{ 86.3587,2636.15,4420.93,298532,4.12407e+006,326.447 } },
	{ 30,{ 105.55,3221.97,5403.37,364872,5.04053e+006,418.537 } },
	{ 33,{ 49.7724,330.765,590.11,35112.3,486466,27.7863 } },
	{ 35,{ 53.9466,802.819,1504.05,104780,1.44459e+006,46.0302 } },
	{ 37,{ 66.2907,1381.65,2662.09,164540,2.26999e+006,71.9717 } },
	{ 38,{ 99.7145,1615.82,3726.92,230356,3.17799e+006,113.335 } },
	{ 39,{ 128.205,2077.49,4791.76,296171,4.08598e+006,179.999 } },
	{ 40,{ 156.694,2539.16,5856.59,361986,4.99397e+006,227.656 } }
};

//dictionary for source type 300
map<int, vector<double> > sourceType300Dictionary
{
	{ 0 ,{ 17.9342,61.6024,153.021,15867.4,216403,4.76104 } },
	{ 1 ,{ 14.6674,33.4247,143.922,7845.78,107011,5.14933 } },
	{ 11,{ 22.6629,104.206,135.196,10528.4,143599,11.4434 } },
	{ 12,{ 23.8828,115.977,469.469,30577.7,417042,15.607 } },
	{ 13,{ 35.6299,271.354,751.115,56285.8,767742,39.953 } },
	{ 14,{ 41.5951,368.76,982.122,82291.5,1.12251e+006,55.7104 } },
	{ 15,{ 45.9337,486.889,1182,104238,1.42192e+006,80.7488 } },
	{ 16,{ 46.9285,651.346,1681.07,143472,1.95714e+006,80.9949 } },
	{ 21,{ 25.4088,114.716,100.502,8548.55,116612,15.4937 } },
	{ 22,{ 29.2006,157.582,595.675,39275.8,535663,30.5977 } },
	{ 23,{ 32.3917,238.981,959.51,65435.8,892532,38.5478 } },
	{ 24,{ 34.7385,408.602,1394.02,94496.8,1.28886e+006,61.320 } },
	{ 25,{ 37.4941,526.539,1692.91,121617,1.65879e+006,84.896 } },
	{ 27,{ 42.1349,730.506,2372.96,168058,2.29223e+006,117.161 } },
	{ 28,{ 43.3231,920.788,3197.98,235281,3.20912e+006,182.518 } },
	{ 29,{ 55.701,1183.87,4030.82,302504,4.126e+006,288.461 } },
	{ 30,{ 68.079,1446.95,4926.56,369726,5.04289e+006,365.166 } },
	{ 33,{ 39.8045,196.179,350.483,35163.7,479643,28.3334 } },
	{ 35,{ 41.7728,345.923,1795.03,107031,1.4598e+006,45.9186 } },
	{ 37,{ 48.1088,584.837,2543.71,167626,2.28629e+006,68.996 } },
	{ 38,{ 65.0605,601.128,3422.06,234676,3.2008e+006,105.038 } },
	{ 39,{ 83.6495,772.881,4327.96,301726,4.1153e+006,162.169 } },
	{ 40,{ 102.238,944.633,5289.73,368777,5.02981e+006,202.638 } }
};


//variables for vehicle data
long   vehicleNumber = 0;
long   vehicleType = 0;
double vehicleAcceleration = 0.0;
double vehicleVelocity = 0.0;
double vehicleWeight = 0.0;
double vehicleVSP = 0.0;
int    vehicleOpmode = 0;

//variables for emission data
double HC = 0.0;
double CO = 0.0;
double NOx = 0.0;
double CO2 = 0.0;
double Energy = 0.0;
double PMtwoPointFive = 0.0;

//variables for time data
int 	frequencyToPushAcceleration = 0;
int		timeStepsPassed = 0;
double 	timeStepValue = 0.0;
double  currentSimulationTime = 0.0;

//dictionary of historical accelerations
map<long, vector<double> > accelerationsDictionary;

//variables for testing
double oldestAcceleration = -999999;
double middleAcceleration = -999999;
double newestAcceleration = -999999;

//variable to hold list of emissions corresponding to vehicle state
vector<double> listOfEmissions;

//VISSIM
BOOL APIENTRY DllMain(HANDLE  hModule, DWORD   ul_reason_for_call, LPVOID  lpReserved)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

//update values of vehicle variables
EMISSIONMODEL_API  int  EmissionModelSetValue(long type, long index1, long index2, long long_value, double double_value, char *string_value)
{
	switch (type)
	{
	case EMISSION_DATA_TIMESTEP:
		timeStepValue = double_value;
		return true;
	case EMISSION_DATA_TIME:
		currentSimulationTime = double_value;
		return true;
	case EMISSION_DATA_VEH_ID:
		vehicleNumber = long_value;
		return true;
	case EMISSION_DATA_VEH_TYPE:
		vehicleType = long_value;
		return true;
	case EMISSION_DATA_VEH_VELOCITY:
		vehicleVelocity = double_value * 3.6;				//FIXME: Change back to m/s
		return true;
	case EMISSION_DATA_VEH_ACCELERATION:
		vehicleAcceleration = double_value;
		return true;
	case EMISSION_DATA_VEH_WEIGHT:
		vehicleWeight = double_value;
		return true;
	case EMISSION_DATA_SLOPE:
	default:
		return false;
	}
}

//update values of VISSIM variables
EMISSIONMODEL_API  int  EmissionModelGetValue(long type, long index1, long index2, long *long_value, double *double_value, char **string_value)
{

	switch (type)
	{

	case EMISSION_DATA_BENZ:
		*double_value = oldestAcceleration;							//NOTE: Using to output oldestAcceleration
		return true;
	case EMISSION_DATA_CO:
		*double_value = CO;
		return true;
	case EMISSION_DATA_CO2:
		*double_value = CO2;
		return true;
	case EMISSION_DATA_HC:
		*double_value = HC;
		return true;
	case EMISSION_DATA_FUEL:									//NOTE: using as Energy Variable
		*double_value = Energy;
		return true;
	case EMISSION_DATA_NMOG:
		*double_value = middleAcceleration;					  //NOTE: Using to output middle Acceleration
		return true;
	case EMISSION_DATA_NMHC:
		*double_value = newestAcceleration;					 //NOTE: Using to output newest Acceleration
		return true;
	case EMISSION_DATA_NOX:
		*double_value = NOx;
		return true;
	case EMISSION_DATA_PART:
		*double_value = PMtwoPointFive; 					   	 //NOTE: using as PM2.5 variable
		return true;
	case EMISSION_DATA_SOOT:
		*double_value = vehicleVSP;								 //NOTE: Using to output VSP
		return true;
	case EMISSION_DATA_SO2:
		*double_value = vehicleOpmode;							//NOTE: Using to output OPMODE
		return true;
	case EMISSION_DATA_EVAP:
		*double_value = -1.0;
		return true;
	default:
		return false;
	}
}

EMISSIONMODEL_API  int  EmissionModelExecuteCommand(long number)
{
	switch (number)
	{
	case EMISSION_COMMAND_INIT:
		return true;
	case EMISSION_COMMAND_CREATE_VEHICLE:
		return true;
	case EMISSION_COMMAND_KILL_VEHICLE:
		return true;
	case EMISSION_COMMAND_CALCULATE_VEHICLE:

		//calculate how often to push to historical accelerations and the number of time steps passed
		frequencyToPushAcceleration = (int)round(1.0 / timeStepValue);
		timeStepsPassed = (int)(currentSimulationTime / timeStepValue);

		//push to historical accelerations if closest possible value to one simulation second has passed
		if (timeStepsPassed % frequencyToPushAcceleration == 0 )
		{
			accelerationsDictionary[vehicleNumber].push_back(vehicleAcceleration);
		}

		//if the dictionary key has a list greater than 3 pop the oldest element
		if (accelerationsDictionary[vehicleNumber].size() > 3)
		{
			accelerationsDictionary[vehicleNumber].erase(accelerationsDictionary[vehicleNumber].begin());
		}

		//assign historical accelerations to variables 	for testing
		if (accelerationsDictionary[vehicleNumber].size() >= 1)
		{
			oldestAcceleration = accelerationsDictionary[vehicleNumber][0];
		}
		if (accelerationsDictionary[vehicleNumber].size() >= 2)
		{
			middleAcceleration = accelerationsDictionary[vehicleNumber][1];
		}
		if (accelerationsDictionary[vehicleNumber].size() >= 3)
		{
			newestAcceleration = accelerationsDictionary[vehicleNumber][2];
		}


		//calculate VSP and get OpMode
		vehicleVSP = calculateVSP(vehicleType, vehicleVelocity, vehicleAcceleration);
		vehicleOpmode = getOpmode(accelerationsDictionary[vehicleNumber], vehicleVelocity, vehicleVSP);

		//retrieve list of emissions
		if (vehicleType == 100)
		{
			listOfEmissions = sourceType100Dictionary[vehicleOpmode];
		}
		else if (vehicleType == 200)
		{
			listOfEmissions = sourceType200Dictionary[vehicleOpmode];
		}
		else if (vehicleType == 300)
		{
			listOfEmissions = sourceType300Dictionary[vehicleOpmode];
		}

		//scale emissions data to give output for one second
		HC = listOfEmissions.at(0) / 3600;
		CO = listOfEmissions.at(1) / 3600;
		NOx = listOfEmissions.at(2) / 3600;
		CO2 = listOfEmissions.at(3) / 3600;
		Energy = listOfEmissions.at(4) / 3600;
		PMtwoPointFive = listOfEmissions.at(5) / 3600;


		return true;
	default:
		return false;
	}
}

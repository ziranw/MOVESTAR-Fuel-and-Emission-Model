MOVESTAR: An Open-Source Vehicle Fuel and Emission Model based on USEPA MOVES
============

Abstract
============
  In this paper, we introduce an open-source model "MOVESTAR" to
  calculate the fuel consumption and pollutant emissions of motor
  vehicles. This model is developed based on U.S. Environmental
  Protection Agency's (EPA) Motor Vehicle Emission Simulator (MOVES),
  which provides an accurate estimate of vehicle fuel and emission under
  a wide range of user-defined conditions. Originally, MOVES requires
  users to specify many parameters through its software graphical user
  interface (GUI), including vehicle types, time periods, geographical
  areas, pollutants, vehicle operating characteristics, and road types.
  In this paper, MOVESTAR is developed as a lite version of MOVES, which
  only takes the second-by-second vehicle speed data and vehicle type as
  inputs. To enable easy integration of this model, its source code is
  provided in different languages, including MATLAB and C++. A case
  study is introduced in this paper to illustrate the effectiveness of
  the model in the development of advanced vehicle technology.

Authors
============

  Ziran Wang, Guoyuan Wu, George Scora\
  Center for Environmental Research and Technology\
  University of California, Riverside\
  zwang050@ucr.edu, gywu@cert.ucr.edu, gscora@cert.ucr.edu
  
Introduction
============

Environmental sustainability has been a crucial factor in the
development of transportation systems nowadays. Transportation sources
emit greenhouse gases (GHGs) that contribute to climate change. As the
total number of motor vehicles around the globe increases over time,
transportation sector becomes one of the largest sources of GHG
emissions. Specifically in the United States, transportation
sector was in charge of 28.2% of 2018 GHGs, where over 90% of the fuel
used for transportation is petroleum based, including primarily gasoline
and diesel. Therefore, the research studies on reducing fuel consumption
and pollutant emissions of our transportation systems (especially motor
vehicles) becomes substantially important.

During the past two decades, a significant amount of "eco-driving"
studies has been proposed, focusing on the policy, methodology, as well
as field implementation of this emerging
concept. By adopting advanced vehicular technologies, motor vehicles can
be driven in an economic or ecologic style, so that their impacts on the
environment can be reduced, making our transportation systems more
sustainable.

In order to conduct environment-related research for motor vehicles, a
proper model is needed to calculate their fuel consumption and pollutant
emissions. To this purpose, the United States Environmental Protection
Agency (USEPA) developed MOtor Vehicle Emission Simulator (MOVES). MOVES
is a state-of-the-science emission modeling system that estimates
emissions for mobile sources at the national, county, and project level
for criteria air pollutants, greenhouse gases, and air toxics [@moves].
In the modeling process, the user specifies vehicle types, time periods,
geographical areas, pollutants, vehicle operating characteristics, and
road types to be modeled. The model then performs a series of
calculations, which have been carefully developed to accurately reflect
vehicle operating processes, such as running, starts, or hoteling, and
provide estimates of total emissions or emission rates per vehicle or
unit of activity.

Although USEPA encourages using MOVES to estimate on-road GHG emissions
or energy consumption regardless of the user's experience level, it is a
sophisticated model that requires the user to read through extensive
documentations to get familiar with its graphical user interface (GUI)
and data inputs. MOVES is certainly a well-developed and
well-maintained model for experts in the field of fuel or emission.
However, for users that are not in this field of study, who just want to
get an estimate of the fuel consumption and pollutant emissions of their
vehicles (in simulations or field implementations), MOVES might be
relatively difficult to implement.

In this project, we develop an open-source model based on MOVES, which
allows users of all levels to easily satisfy their research needs. This
"MOVESTAR" model is developed in multiple platforms and programming
languages, which enables both real-time calculation and post-processing
evaluation.

MOVESTAR Model
==============

<img src="docs/images/Flowchart.PNG" align="middle" width="500"/>

In this section, MOVESTAR model is introduced step by step, which is
summarized by the flowchart shown in figure above. The very first step
is to define the vehicle type of the target vehicle that needs to be
considered. Originally, the user of MOVES has to select from 13 "source
use types", which is the terminology defind by MOVES to describe
vehicles. Additionally, the user also needs to select from six different
fuel types, including gasoline, diesel, ethanol E-85, compressed natural
gas (CNG), electricity, and liquefied petroleum gas. Both steps are
supposed to be conducted in the GUI of MOVES. However, in order to
simplify this first process of MOVESTAR, we only provide limited options
of vehicle types.

Actually, before this vehicle type selection, MOVES originally has four
additional steps in its GUI, including specifying "description", "scale"
(including model type, analysis scale, and calculation type selections),
"time spans" (including time aggregation level, years, months, days, and
hours selections), and "geographic bounds" (including region, states,
counties, and project selections). For the sake of simplicity, these
steps are all removed in our proposed MOVESTAR model.

Once the vehicle type is selected, the related parameters of each
vehicle type can be loaded for further calculation of the vehicle
specific power (VSP). The variables A, B, C, M and f needed in the
VSP calculation are derived from the following table based on
different vehicle types.

<img src="docs/images/Table.png" align="middle" width="700"/>

In order to calculate the VSP, we need to also obtain the vehicle state.
VSP is calculated by normalizing the continuous power output for each
vehicle to its own weight. Vehicles are tested on full chassis
dynamometers, and emission standards are in units of grams per mile.
Therefore, the emission standards are largely independent of the weight
(as well as other physical characteristics) of the vehicle and depend on
the distance.

As mentioned earlier, the proposed MOVESTAR model only requires
second-by-second vehicle speed data as model input, so the user needs to
pre-process the data if its frequency is not 1Hz. Additionally, MOVESTAR
model has an additional function that can convert the second-by-second
speed data into the second-by-second acceleration data.

Once the VSP is calculated, MOVESTAR model will be proceeded to the
operating mode calculation. Operating mode is directly used to calculate
fuel consumption and pollutant emissions in MOVESTAR model. It is
calculated on a second-by-second basis, based on the VSP and speed class
of the vehicle. The mapping relationship from VSP and speed class to the
operating mode is illustrated as the following table.

<img src="docs/images/OpMode.png" align="middle" width="500"/>

As can be seen from the table, the values in the center of the table
stand for the operating modes of a vehicle, which are mapped from VSP
and speed class. Specifically, 21 operating modes (all expect for "11"
and "21") represent the "cruise/acceleration" scenario, where VSP is
larger than zero. Two operating modes ("11" and "21") represent the
"coasting" scenario, where VSP is smaller than zero. Additionally, two
scenarios are not shown in this table since they do not require VSP,
where the "deceleration/braking" scenario has a mode of "0", and the
"idle" scenario has a mode of "1".

Once the operating mode calculation is completed, MOVESTAR model can be
proceeded to the last step. The emission rate associated with the
operating modes is defined in USEPA MOVES, where we only extract the
values of specific vehicle types as defined earlier. Therefore, based on
the second-by-second operating modes of a vehicle, second-by-second fuel
and emission outputs of our MOVESTAR model can be generated. A list of
all outputs are shown as the following table.

<img src="docs/images/Output.PNG" align="middle" width="500"/>


Open-Source Implementation in Various Platforms {#sec:implement}
===============================================

In this section, we introduce the implementation of MOVESTAR in various
platforms as open-source codes. The detailed integration frameworks and
guidelines are introduced with respect to Mathworks MATLAB and PTV
Vissim. Specifically, MOVESTAR is implemented in MATLAB as
post-processing code, and in Vissim as real-time processing code.

Mathworks MATLAB
----------------

MOVESTAR model is implemented in Mathworks MATLAB [@MATLAB] as ".m"
files that can process the speed data generated by vehicles in
advance[^1]. There are five files in the MATLAB version of MOVESTAR
model, where their features, inputs and outputs are listed in table
[\[matlab\]](#matlab){reference-type="ref" reference="matlab"}.

![image](Matlab.PNG){width="1.0\\columnwidth"} [\[matlab\]]{#matlab
label="matlab"}

In the current MATLAB version of MOVESTAR model, there are only two
different vehicle types that the user needs to choose from. If the user
specify "1" in the main function "MOVESTAR.m", then the tested vehicle
is assumed as a light-duty vehicle; Otherwise, if "2" is entered, then
the tested vehicle is assumed as a light-duty truck.

In terms of the other input of MOVESTAR model besides the vehicle type,
the vehicle speed data needs to be processed by the user in the
second-by-second format. If the user generates the data by MATLAB
Simulink, MOVESTAR model provides a "spdSpd.m" file that can convert
sub-second speed data (in ".mat" format) into second-by-second speed
data.

The MATLAB version of MOVESTAR model provides two output files in ".dat"
format, where the one with an "ER" suffix means the "emission rate"
results, as shown in table [\[result\]](#result){reference-type="ref"
reference="result"}. Additionally, MOVESTAR also provides the normalized
fuel and emission results based on the travel distance, which are
included in the file with a "EF" suffix (i.e., "emission factor").

Microscopic Traffic Simulator Vissim with C++ API
-------------------------------------------------

As a widely used microscopic traffic simulator, PTV Vissim [@VISSIM]
provides C++ API for users to load external emission models. MOVESTAR
model also provides a C++ project that can be integrated in Vissim to
calculate fuel and emission in real simulation time[^2].

This C++ project includes an "EmissionModel.cpp" file and an
"EmissionModel.h" file. The ".cpp" file is the main script that includes
the calculations of VSP, operating mode, and emission rate. The ".h"
file is defined by Vissim to allow the user to interact with its API.
This "EmissionModel.vcxproj" project can be compiled to a ".dll" file,
which can be further loaded in Vissim GUI. The high-level integration
framework of MOVESTAR model in Vissim is illustrated as figure
[2](#transform){reference-type="ref" reference="transform"}, while the
detailed integration steps should be referred to the Vissim manual.

![Integration framework of MOVESTAR model in PTV
Vissim](VISSIM.png){#transform width="1.0\\columnwidth"}

Case Study and Results Evaluation {#sec:case}
=================================

In this section, we perform a case study of MOVESTAR model based on a
cooperative eco-driving (CED) system with connected and automated
vehicles (CAVs) [@wang2020cooperative]. This case study develops a
system to improve the energy efficiency along a corridor with signalized
intersections. To study the effect of penetration rate of CAVs, two
different types of vehicles are defined in the system as conventional
vehicles and CED vehicles. The high-level concept of this CED system can
be illustrated as figure [3](#CED){reference-type="ref"
reference="CED"}.

![Illustration of the CED system in MOVESTAR case study](Fig1.png){#CED
width="0.6\\columnwidth"}

A microscopic traffic simulation network based on the real-world map in
Riverside, CA, USA is modeled in Vissim as figure
[4](#network){reference-type="ref" reference="network"}, with real-world
signal phase and timing data and traffic count data. Different vehicle
longitudinal control models and their relevant logic (e.g., role
transition) are integrated into the simulation network to simulate
vehicles' behavior. MOVESTAR fuel and emission model is implemented to
analyze the environmental impacts of the proposed CED system. This
integration architecture is illustrated in figure
[5](#arc){reference-type="ref" reference="arc"}.

![Vissim simulation traffic network in MOVESTAR case
study](Fig5.PNG){#network width="1.0\\columnwidth"}

![Vissim simulation system architecture in MOVESTAR case
study](VISSIM2.png){#arc width="1.0\\columnwidth"}

The simulation is conducted under different penetration rates of CED
vehicles and conventional vehicles. The simulation results are compared
with two baseline scenarios, where the first scenario contains 100%
conventional vehicles, and the second scenario contains 100%
eco-approach and departure (EAD) vehicles. EAD vehicles are CAVs that
conduct eco-driving maneuvers solely with the traffic signals, which do
not cooperate with other CAVs.

As can be seen from table
[\[simresult\]](#simresult){reference-type="ref" reference="simresult"},
the results of energy consumption, NOx emission, HC emission, CO
emission, and CO$_2$ emission can all be calculated by MOVESTAR model in
different scenarios, which are eventually shown as relative values with
respect to two baseline scenarios. This case study validates the
effectiveness of the CED system, because the fuel and emission results
improve as the penetration rate of CED vehicles increases. All results
shown in table [\[simresult\]](#simresult){reference-type="ref"
reference="simresult"} are generated by the proposed MOVESTAR model.

![image](Results.PNG){width="1.0\\columnwidth"}
[\[simresult\]]{#simresult label="simresult"}

Conclusion {#sec:conclusion}
==========

In this paper, we introduced an open-source model "MOVESTAR" for users
to easily generate the fuel and emission results in their research
studies. USEPA MOVES model originally requires users to specify many
parameters through its software GUI. MOVESTAR model, developed as a lite
version of MOVES, only takes vehicle speed and vehicle type as inputs of
the model. It provides a much easier access for users of all levels to
calculate fuel and emission results. The open-source implementations of
MOVESTAR model were demonstrated in MATLAB and Vissim C++ API, and a
case study of the CAV technology was conducted to evaluate the
effectiveness of the model.

[^1]: MOVESTAR for MATLAB source code:
    <https://github.com/ziranw/MOVESTAR-Fuel-and-Emission-Model/tree/master/MOVESTAR_MATLAB_v1.0>

[^2]: MOVESTAR for Vissim source code:
    <https://github.com/ziranw/MOVESTAR-Fuel-and-Emission-Model/tree/master/MOVESTAR_VISSIM_v1.0>

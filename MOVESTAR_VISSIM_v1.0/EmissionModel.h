/*==========================================================================*/
/*  EmissionModel.h                                  DLL Module for VISSIM  */
/*                                                                          */
/*  Interface module for external emission models.                          */
/*                                                                          */
/*  Version of 2001-08-29                                   Lukas Kautzsch  */
/*==========================================================================*/

#ifndef __EMISSIONMODEL_H
#define __EMISSIONMODEL_H


#ifndef _CONSOLE
#include <windows.h>
#endif

/* In the creation of EmissionModel.DLL all files must be compiled */
/* with the preprocessor definition EMISSIONMODEL_EXPORTS.         */
/* Programs that use EmissionModel.DLL must not be compiled        */
/* with that preprocessor definition.                              */

#ifdef EMISSIONMODEL_EXPORTS
#define EMISSIONMODEL_API extern "C" __declspec(dllexport)
#else
#define EMISSIONMODEL_API extern "C" __declspec(dllimport)
#endif

/*==========================================================================*/

/* general data: */
#define  EMISSION_DATA_TIMESTEP                101
           /* double: simulation time step length [s] */
#define  EMISSION_DATA_TIME                    102
           /* double: current simulation time [s] */

/* current vehicle data: */
#define  EMISSION_DATA_VEH_ID                  201
           /* long:   vehicle number */
#define  EMISSION_DATA_VEH_TYPE                202
           /* long:   vehicle type number (user defined) */
#define  EMISSION_DATA_VEH_VELOCITY            203
           /* double: current speed [m/s] */
#define  EMISSION_DATA_VEH_ACCELERATION        204
           /* double: current acceleration [m/s�] */
#define  EMISSION_DATA_VEH_WEIGHT              205
           /* double: vehicle weight [kg] */

/* link data: */
#define  EMISSION_DATA_SLOPE                   301

/* emission data: */
/* must be provided by the emission model after EMISSION_COMMAND_CALCULATE */
/* for GetValue()                                                          */
#define  EMISSION_DATA_BENZ                    801
           /* double: benzene emissions [g/s] in the current time step */
#define  EMISSION_DATA_CO                      802
           /* double: carbon monoxide emissions [g/s] in the current time step */
#define  EMISSION_DATA_CO2                     803
           /* double: carbon dioxide emissions [g/s] in the current time step */
#define  EMISSION_DATA_HC                      804
           /* double: hydrocarbon emissions [g/s] in the current time step */
#define  EMISSION_DATA_FUEL                    805
           /* double: fuel consumption [g/s] in the current time step */
#define  EMISSION_DATA_NMOG                    806
           /* double: nonmethane organic gas emissions [g/s] in the current time step */
#define  EMISSION_DATA_NMHC                    807
           /* double: nonmethane hydrocarbon emissions [g/s] in the current time step */
#define  EMISSION_DATA_NOX                     808
           /* double: nitrogen oxide emissions [g/s] in the current time step */
#define  EMISSION_DATA_PART                    809
           /* double: particulate emissions [g/s] in the current time step */
#define  EMISSION_DATA_SOOT                    810
           /* double: soot emissions [g/s] in the current time step */
#define  EMISSION_DATA_SO2                     811
           /* double: sulfur dioxide emissions [g/s] in the current time step */
#define  EMISSION_DATA_EVAP                    812
           /* double: evaporation HC emissions [g/s] in the current time step */

/*--------------------------------------------------------------------------*/

EMISSIONMODEL_API  int  EmissionModelSetValue (long   type,
                                               long   index1,
                                               long   index2,
                                               long   long_value,
                                               double double_value,
                                               char   *string_value);

/* Sets the value of a data object of type <type>, selected by <index1> */
/* and possibly <index2>, to <long_value>, <double_value> or            */
/* <*string_value> (object and value selection depending on <type>).    */
/* Return value is 1 on success, otherwise 0.                           */

/*--------------------------------------------------------------------------*/

EMISSIONMODEL_API  int  EmissionModelGetValue (long   type,
                                               long   index1,
                                               long   index2,
                                               long   *long_value,
                                               double *double_value,
                                               char   **string_value);

/* Gets the value of a data object of type <type>, selected by <index1> */
/* and possibly <index2>, and writes that value to <*double_value>,     */
/* <*float_value> or <**string_value> (object and value selection       */
/* depending on <type>).                                                */
/* Return value is 1 on success, otherwise 0.                           */

/*==========================================================================*/

#define  EMISSION_COMMAND_INIT              0
           /* called from VISSIM once at the start of a simulation run */
           /* values set before: EMISSION_DATA_TIMESTEP */
           /*                    EMISSION_DATA_TIME     */

#define  EMISSION_COMMAND_CREATE_VEHICLE    1
           /* called from VISSIM once per vehicle entering the network */
           /* values set before: EMISSION_DATA_VEH_ID               */
           /*                    EMISSION_DATA_VEH_TYPE             */

#define  EMISSION_COMMAND_KILL_VEHICLE      2
           /* called from VISSIM once per vehicle leaving the network */
           /* value set before: EMISSION_DATA_VEH_ID */

#define  EMISSION_COMMAND_CALCULATE_VEHICLE 3
           /* called from VISSIM once per time step during a simulation run */
           /* values set before: all values                                 */

/*--------------------------------------------------------------------------*/

EMISSIONMODEL_API  int  EmissionModelExecuteCommand (long number);

/* Executes the command <number> if that is available in the emission */
/* module. Return value is 1 on success, otherwise 0.                 */

/*==========================================================================*/

#endif /* __EMISSIONMODEL_H */

/*==========================================================================*/
/*  Ende of EmissionModel.h                                                 */
/*==========================================================================*/

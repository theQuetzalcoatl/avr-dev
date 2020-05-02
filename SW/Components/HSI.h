/***************************************************************************\
** This file containt predefined values about everything related to the HW **
\***************************************************************************/


/*=======[ General ]=======*/
#define CLK_FRQ (16000000UL) /* Hz */

/*=======[ Temperature sensing ]=======*/
#define TEMP_SENSOR_VALUE 	(1000U) /* ohms */
#define TEMP_COEFF					() 			/* ohms/C° */ /*3,90802*10^3*/ /* TODO: convert to fixed point value */
#define SELF_HEATING_COEFF	(0,4f)	/* K/mW  -- I^2*R = 1mW at 0 C° */ /* TODO: convert to fixed point value */

#define TEMP_SENSOR_CURRENT	(1U) 		/* mA */


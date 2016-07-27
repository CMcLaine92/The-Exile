#ifndef CFLYWEIGHT_H
#define CFLYWEIGHT_H

/***********************************************
* Filename:  		Flyweight.h
* Date:      		08/25/2015
* Mod. Date: 		----------
* Mod. Initials:	--
* Author:    		Matthew Zanini
* Purpose:   		Defines the Flyweights
*					for use in particles
************************************************/
struct TFlyweight
{
	//Min Life, Max Life
	float f2Life[2];
	//Start Size, End Size
	float f2Size[2];
	//Start SizeMax, End Size Max
	float f2SizeMax[2];
	//Start Color
	// R G B A
	float f4StartColor[4];
	//End Color
	// R G B A
	float f4EndColor[4];
	//XYZ for each
	float f3MinStartVelocity[3];
	float f3MaxStartVelocity[3];
	float f3MinEndVelocity[3];
	float f3MaxEndVelocity[3];

	bool bConstantVelocity;
};
#endif
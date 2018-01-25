/*
 * CControlComp.cpp
 *
 *  Created on: Dec 13, 2017
 *      Author: EU4M
 */

#include "CControlComp.h"
#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include "CContainer.h"
#include "SMPU6050Data.h"
#include "SStateVectorData.h"
#include "CBBBHardware.h"
#include "math.h"

using namespace std;


extern CContainer myContainer;

UInt16 adcValue;
SMPU6050Data mySensor1;
SMPU6050Data mySensor2;

SStateVectorData myStateData;
CBBBHardware myCBBB;

float xSensor1;
float xSensor2;
float ySensor1;
float ySensor2;
float angleAux;

float myRs1 = 0.14;
float myRs2 = 0.061;
float myAlpha = myRs1/myRs2;

float cX1 = 0.000604;
float oX1 = 0.243;

float c2y1 = -0.0000000308;
float c1y1 = 0.000279;
float oY1 = -6.09;

float cX2 = 0.000599;
float oX2 = 0.0308;

float c2y2 = -0.0000000144;
float c1y2 = -0.000191;
float oY2 = -2.78;

float cGyros =  0.00106526;
float offGyro1 = -11.36;
float offGyro2 = -30.7;

float cAdc = -0.5081;
float oAdc = 1033.99;

bool flagFirst = true;
float phiGInt;
float cFilter = 0.98;
float cTime = 0.02;

float torque = 0;
float k[3] = {1.4701, 0.1075, 0.0005}; //{0.8821, 0.1386, 0.00002}; //{ -1.4701, -0.1075, -0.0005} ,  { 0.5033, 0.052, 0.0001} , { 1.0717, 0.0791, 0.0001}

float cTorque1=0.001;
float cTorque2=-0.001;

CControlComp::CControlComp()
{

}

CControlComp::~CControlComp()
{

}

void CControlComp::run(){

	while(true){

	fetch();
	calibrate();
	calculate();
	filter();
	regler();
	motor();

	myContainer.writeADCValue(adcValue);
	myContainer.writeSensor1Data(mySensor1);
	myContainer.writeSensor2Data(mySensor2);
	myContainer.writeStateData(myStateData);
	myContainer.signalReader();

	usleep(20000);

	}

}

void CControlComp::init(){

	myCBBB.enableMotor();

}

void CControlComp::fetch(){

	myCBBB.fetchValues(adcValue, mySensor1, mySensor2);

}

void CControlComp::calculate(){

}

void CControlComp::calibrate(){

	xSensor1 = cX1 * mySensor1.mX__dd + oX1;
	ySensor1 = c2y1*mySensor1.mY__dd * mySensor1.mY__dd + c1y1 * mySensor1.mY__dd + oY1;

	xSensor2 = cX2 * mySensor2.mX__dd + oX2;
	ySensor2 = c2y2*mySensor2.mY__dd * mySensor2.mY__dd + c1y2 * mySensor2.mY__dd + oY2;

	angleAux = (xSensor1 - myAlpha * xSensor2)/( ySensor1 - myAlpha * ySensor2);

	myStateData.mPhi_A = -atan(angleAux) ;

	myStateData.mPhi_G = ((mySensor1.mPhi__d - offGyro1) + (mySensor2.mPhi__d - offGyro2)) * (cGyros/2);

	myStateData.mPsi__d =  cAdc * adcValue + oAdc;

}

void CControlComp::filter(){

	if(flagFirst){

		myStateData.mPhi_C = 0;
		phiGInt = 0;
		flagFirst = false;

	}

	else{

		myStateData.mPhi_C = (1 - cFilter) * myStateData.mPhi_A + cFilter * (phiGInt + cTime * myStateData.mPhi_G);
		phiGInt = myStateData.mPhi_C;

	}

}

void CControlComp::regler(){

	if(myStateData.mPhi_C < 0.1745 && myStateData.mPhi_C > -0.1745)
	{

		torque = k[0] * (myStateData.mPhi_C) + k[1] * myStateData.mPhi_G + k[2] * myStateData.mPsi__d;
		myCBBB.setTorque(torque);

	}
	else{

		if(myStateData.mPhi_C > 0.1745)
		{

			myCBBB.setTorque(cTorque1);
		}

		if(myStateData.mPhi_C < -0.1745 )
		{

			myCBBB.setTorque(cTorque2);

		}

	}

}

void CControlComp::motor(){

//	myCBBB.setTorque(0.00);

}

/*
 * CCommComp.cpp
 *
 *  Created on: Dec 13, 2017
 *      Author: EU4M
 */

#include "CCommComp.h"
#include "CContainer.h"
#include <iostream>
#include <cstdlib>
#include "SContent.h"
#include "CServer.h"

using namespace std;

extern CContainer myContainer;
SContent myContent;
CServer myServer;

CCommComp::CCommComp()
{

}

CCommComp::~CCommComp()
{

}

void CCommComp::run(){

	while(true){

		myContainer.getContent(true,myContent);
		myServer.transmitMessage(myContent);

	}

}

void CCommComp::init(){

	myServer.init();
	myServer.waitForClient();

}

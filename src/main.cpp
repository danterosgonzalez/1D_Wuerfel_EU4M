/*
 * main.cpp
 *
 *  Created on: Dec 13, 2017
 *      Author: EU4M
 */

#include <iostream>
#include <unistd.h>
#include <cstdlib>
#include "CThread.h"
#include "CContainer.h"
#include "CCommComp.h"
#include "CControlComp.h"
#include "CBinarySemaphore.h"

using namespace std;

CContainer myContainer;

int main ()
{
	CCommComp myCommComp;
	CControlComp myControlComp;

	CThread threadControl (&myControlComp, CThread::PRIORITY_NORM);
	CThread threadComm (&myCommComp, CThread::PRIORITY_NORM);

	threadControl.start();
	threadComm.start();

	threadControl.join();
	threadComm.join();

}



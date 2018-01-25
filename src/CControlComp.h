/*
 * CControlComp.h
 *
 *  Created on: Dec 13, 2017
 *      Author: vmuser
 */

#ifndef CCONTROLCOMP_H_
#define CCONTROLCOMP_H_

#include "Global.h"
#include <pthread.h>
#include "IRunnable.h"

class CControlComp : public IRunnable
{

public:

	void run();
	void init();
	void fetch();
	void calibrate();
	void calculate();
	void regler();
	void motor();
	void filter();
	~CControlComp();
	CControlComp();

};

#endif /* CCONTROLCOMP_H_ */

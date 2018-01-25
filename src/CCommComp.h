/*
 * CCommComp.h
 *
 *  Created on: Dec 13, 2017
 *      Author: vmuser
 */

#ifndef CCOMMCOMP_H_
#define CCOMMCOMP_H_

#include "Global.h"
#include <pthread.h>
#include "IRunnable.h"

class CCommComp : public IRunnable
{

public:
	void run();
	void init();
	~CCommComp();
	CCommComp();

};

#endif /* CCOMMCOMP_H_ */

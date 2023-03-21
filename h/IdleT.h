#ifndef _idlet_h_
#define _idlet_h_

#include "Thread.h"

class IdleT: public Thread {
public:
	void start();
	IdleT();
	//static int a;
	virtual void run();
};

#endif

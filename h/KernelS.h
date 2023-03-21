#ifndef _kernels_h_
#define _kernels_h_

#include "Semaphor.h"

class Queue;

class KernelSem {
public:
	KernelSem(int v);
	~KernelSem();

	static int id;
	int myID;

	int wait (Time maxTimeToWait);
	void signal();

	int value;
	Queue* waitingOnTimeOrSignal;
	Queue* waitingOnSignal;
};

#endif

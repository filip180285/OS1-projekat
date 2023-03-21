#ifndef _kernele_h_
#define _kernele_h_

#include "PCB.h"

typedef unsigned char IVTNo;
class IVTEntry;
class KernelSem;

class KernelEv {
public:
	KernelEv(IVTNo ivtNo);
	~KernelEv();

	void wait();
	void signal();

	PCB* myCreator;
	IVTEntry* myEntry;
	KernelSem* mySem;
};

#endif

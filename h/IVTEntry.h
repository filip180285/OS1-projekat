#ifndef _ivtentry_h_
#define _ivtentry_h_

#include "KernelE.h"

typedef unsigned char IVTNo;

class KernelEv;

typedef void interrupt (*ptrInterrupt)(...);
#define NUMBER_OF_ENTRIES 256

class IVTEntry {
public:
	IVTEntry(IVTNo numEntry,ptrInterrupt myIR);
	~IVTEntry();

	static IVTEntry* entries[NUMBER_OF_ENTRIES];

	void signal();

	IVTNo numberOfEntry;
	ptrInterrupt oldIR;
	KernelEv* myEvent;
};

#endif

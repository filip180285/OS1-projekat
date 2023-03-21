#include "KernelE.h"
#include "Init.h"
#include "IVTEntry.h"
#include "KernelS.h"
#include <iostream.h>

KernelEv::KernelEv(IVTNo ivtNo) {
#ifndef BCC_BLOCK_IGNORE
	lock
#endif
	myCreator = (PCB*)Init::running;
	myEntry = IVTEntry::entries[ivtNo];
	myEntry->myEvent = this;
	mySem = new KernelSem(0);
	//cout << "kreirao kernelev\n"; asm cli;
#ifndef BCC_BLOCK_IGNORE
	unlock
#endif
}

KernelEv::~KernelEv() {
#ifndef BCC_BLOCK_IGNORE
	lock
#endif
	delete mySem;
	myEntry->myEvent = 0;
#ifndef BCC_BLOCK_IGNORE
	unlock
#endif
}

void KernelEv::wait() {
#ifndef BCC_BLOCK_IGNORE
	lock
#endif
	if(Init::running == myCreator) {
		mySem->wait(0);
	}
#ifndef BCC_BLOCK_IGNORE
	unlock
#endif
}

void KernelEv::signal() {
#ifndef BCC_BLOCK_IGNORE
	lock
#endif
	if(mySem->value == 1) {
#ifndef BCC_BLOCK_IGNORE
	unlock
#endif
	return;
	}
	//cout << "dao signal\n"; asm cli;
	mySem->signal();
	//cout << "\n" << mySem->value << "\n"; asm cli;
#ifndef BCC_BLOCK_IGNORE
	unlock
#endif
}

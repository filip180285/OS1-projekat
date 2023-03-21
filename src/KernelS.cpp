#include "KernelS.h"
#include "Init.h"
#include "PCB.h"
#include "SCHEDULE.H"
#include "Thread.H"

int KernelSem::id = 0;

KernelSem::KernelSem(int v) {
#ifndef BCC_BLOCK_IGNORE
	lock
#endif
	myID = id++;
	value = v;
	waitingOnTimeOrSignal = new Queue();
	waitingOnSignal = new Queue();
#ifndef BCC_BLOCK_IGNORE
	unlock
#endif
}

KernelSem::~KernelSem() {
#ifndef BCC_BLOCK_IGNORE
	lock
#endif
	delete waitingOnTimeOrSignal;
	delete waitingOnSignal;
#ifndef BCC_BLOCK_IGNORE
	unlock
#endif
}

int KernelSem::wait(Time maxTimeToWait) {
#ifndef BCC_BLOCK_IGNORE
	lock
#endif
	//if(maxTimeToWait < 0) maxTimeToWait = 0;
	//Init::running->semTime = new TimeOnSemaphore(maxTimeToWait);
	Init::running->semTime->returnValue = 1;
	Init::running->semTime->maxTimeToWait = maxTimeToWait;
	value--;
	if(value < 0) {
		if(maxTimeToWait == 0) {waitingOnSignal->put((PCB*)Init::running);}
		else if (maxTimeToWait > 0) {waitingOnTimeOrSignal->put((PCB*)Init::running);}
		Init::running->semTime->maxTimeToWait = maxTimeToWait;
		Init::running->state = PCB::BLOCKED;
		dispatch();
	}
#ifndef BCC_BLOCK_IGNORE
	unlock
#endif
	return Init::running->semTime->returnValue;
}

void KernelSem::signal() {
#ifndef BCC_BLOCK_IGNORE
	lock
#endif
	value++;
	if(value <= 0) {
		PCB* tmp = waitingOnTimeOrSignal->get();
		if(tmp != 0) {
		tmp->semTime->returnValue = 1;
		tmp->state = PCB::READY;
		Scheduler::put(tmp);
	    }
		else if(tmp == 0) {
			tmp = waitingOnSignal->get();
			tmp->semTime->returnValue = 1;
			tmp->state = PCB::READY;
			Scheduler::put(tmp);
		}
	}
#ifndef BCC_BLOCK_IGNORE
	unlock
#endif
}

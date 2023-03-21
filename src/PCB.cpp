#include "SCHEDULE.h"
#include "PCB.h"
#include <dos.h>
#include "Init.h"
#include "Queue.h"
//#include <iostream.h>

ID PCB::Id = 0;

TimeOnSemaphore::TimeOnSemaphore() {
#ifndef BCC_BLOCK_IGNORE
	lock
#endif
	maxTimeToWait = 0;
	returnValue = 0;
#ifndef BCC_BLOCK_IGNORE
	unlock
#endif
}

PCB::PCB(StackSize stackSz,Time timeSl,Thread* myT) {
#ifndef BCC_BLOCK_IGNORE
	lock
#endif
	stackSize = stackSz; if(stackSize > 65535) stackSize = 65535;
	timeSlice = timeSl;
	myThread = myT;
	myID = Id++;
	state = NEW;
	timePassed = 0;
	waitingQueue = new Queue();
	semTime = new TimeOnSemaphore();
	//stack=0; //ss=sp=bp=0;
#ifndef BCC_BLOCK_IGNORE
	unlock
#endif
}

PCB::~PCB() {
#ifndef BCC_BLOCK_IGNORE
	lock
#endif
	//cout << "usao\n";
	delete[] stack;
	delete waitingQueue;
	delete semTime;
#ifndef BCC_BLOCK_IGNORE
	unlock
#endif
}

void PCB::wrapper() {
	Init::running->myThread->run();
#ifndef BCC_BLOCK_IGNORE
	lock
#endif
	Init::running->state = DONE;
	PCB* tmp = 0;
	while(Init::running->waitingQueue->front != 0) {
		tmp = Init::running->waitingQueue->get();
		tmp->state = READY;
		Scheduler::put(tmp);
	}
	Init::dispatch();
}

void PCB::makeStack() {
#ifndef BCC_BLOCK_IGNORE
	lock
#endif
	StackSize realSize = stackSize/2;
	stack = new unsigned[realSize];
	stack[realSize-1] = 0x200;
	#ifndef BCC_BLOCK_IGNORE
	stack[realSize-2] = FP_SEG(PCB::wrapper);
	stack[realSize-3] = FP_OFF(PCB::wrapper);
	ss = FP_SEG(stack+realSize-12);
	sp = FP_OFF(stack+realSize-12);
	bp = sp;
	#endif
#ifndef BCC_BLOCK_IGNORE
	unlock
#endif
}

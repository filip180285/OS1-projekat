#ifndef _pcb_h_
#define _pcb_h_

#include "Thread.h"
#include "Queue.h"

struct TimeOnSemaphore {
	Time maxTimeToWait;
	int returnValue;
	TimeOnSemaphore();
};

class PCB {
public:
	PCB(StackSize stackSz,Time timeSl,Thread* myT);
	void makeStack();
	~PCB();

	static ID Id;
	static void wrapper();

	enum ThreadState {NEW, READY, BLOCKED, DONE};

	StackSize stackSize;
	Time timeSlice;
	Thread* myThread;
	ID myID;
	ThreadState state;
	unsigned* stack;
	unsigned ss,sp,bp;
	Queue* waitingQueue;
	Time timePassed;
	TimeOnSemaphore* semTime;
};

#endif

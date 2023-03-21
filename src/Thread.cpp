#include "Thread.h"
#include "SCHEDULE.h"
#include "PCB.h"
#include "Init.h"
#include "Queue.h"
//#include "iostream.h"

void Thread::start() {
#ifndef BCC_BLOCK_IGNORE
	lock
#endif
	myPCB->makeStack();
	myPCB->state = PCB::READY;
	Scheduler::put(myPCB);
#ifndef BCC_BLOCK_IGNORE
	unlock
#endif
}

void Thread::waitToComplete() {
#ifndef BCC_BLOCK_IGNORE
	lock
#endif
	if(this == Init::starting) {
#ifndef BCC_BLOCK_IGNORE
	unlock
#endif
	return;
	}
	if(this == Init::idle) {
#ifndef BCC_BLOCK_IGNORE
	unlock
#endif
	return;
	}
	if(this == Init::running->myThread) {
	#ifndef BCC_BLOCK_IGNORE
		unlock
	#endif
	return;
	}
	if(myPCB->state == PCB::NEW) {
#ifndef BCC_BLOCK_IGNORE
	unlock
#endif
	return;
	}
	if(myPCB->state == PCB::DONE) {
#ifndef BCC_BLOCK_IGNORE
	unlock
#endif
	return;
	}
	Init::running->state = PCB::BLOCKED;
	myPCB->waitingQueue->put((PCB*)Init::running);
	dispatch();
#ifndef BCC_BLOCK_IGNORE
	unlock
#endif
}

Thread::~Thread() {
#ifndef BCC_BLOCK_IGNORE
	lock
#endif
	waitToComplete();
	 /*Elem* old = 0; Elem* prev = 0;
	for (Elem* tmp = Init::PCB_List->front; tmp != 0;
			) {
			if(tmp->pcb == myPCB){
				if (tmp == Init::PCB_List->front) {
					Init::PCB_List->front = Init::PCB_List->front->next;
			    }
				old = tmp;
				tmp = tmp->next;
				if(prev != 0)
				prev->next = tmp;
				if (Init::PCB_List->rear == old) {
					Init::PCB_List->rear = prev;
				}
				delete old;
				break;
			}
			prev = tmp;
			tmp = tmp->next;
	}*/
	delete myPCB;
	//myPCB = 0;
#ifndef BCC_BLOCK_IGNORE
	unlock
#endif
}

Thread::Thread(StackSize stackSize,Time timeSlice) {
#ifndef BCC_BLOCK_IGNORE
	lock
#endif
	myPCB = new PCB(stackSize,timeSlice,this);
	Init::PCB_List->put(myPCB);
#ifndef BCC_BLOCK_IGNORE
	unlock
#endif
}

void Thread::run() { }

ID Thread::getId() {
	return myPCB->myID;
}

ID Thread::getRunningId() {
	return Init::running->myID;
}

Thread* Thread::getThreadByID(ID id) {
	return Init::PCB_List->get(id);
}

void dispatch() {
#ifndef BCC_BLOCK_IGNORE
	lock
#endif
	Init::dispatch();
#ifndef BCC_BLOCK_IGNORE
	unlock
#endif
}


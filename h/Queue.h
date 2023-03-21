#ifndef _queue_h_
#define _queue_h_

#include "Thread.h"

class PCB;

class Elem {
public:
	PCB* pcb;
	Elem* next;
	Elem(PCB* p);
};

class Queue {
public:
	Queue();
	void put(PCB* pcb);
	PCB* get();
	Thread* get(ID id);
	~Queue();
	Elem* front,*rear;
};

#endif

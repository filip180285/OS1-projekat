#include "Queue.h"
#include "PCB.h"
#include "Init.h"
//#include <iostream.h>

Elem::Elem(PCB* p) {
	pcb = p; next=0;
}

Queue::Queue() {
#ifndef BCC_BLOCK_IGNORE
	lock
#endif
	front = rear = 0;
#ifndef BCC_BLOCK_IGNORE
	unlock
#endif
}

void Queue::put(PCB* pcb) {
#ifndef BCC_BLOCK_IGNORE
	lock
#endif
	Elem* tmp = new Elem(pcb);
	if (rear == 0) {
	front = rear = tmp;
#ifndef BCC_BLOCK_IGNORE
	unlock
#endif
	return;
	}
	rear->next = tmp;
	rear = tmp;
#ifndef BCC_BLOCK_IGNORE
	unlock
#endif
}

PCB* Queue::get() {
#ifndef BCC_BLOCK_IGNORE
	lock
#endif
	if(front == 0) {
#ifndef BCC_BLOCK_IGNORE
	unlock
#endif
	return 0;
}
	PCB* pcb = front->pcb;
	Elem *tmp = front;
	front = front->next;
	if(front == 0) rear = 0;
	delete tmp;
#ifndef BCC_BLOCK_IGNORE
	unlock
#endif
	return pcb;
}

Thread* Queue::get(ID id) {
#ifndef BCC_BLOCK_IGNORE
	lock
#endif
	Thread* t = 0; Elem* tmp = 0;
	for(tmp = Init::PCB_List->front;tmp != 0; tmp = tmp->next) {
		if(tmp->pcb->myID == id) {t = tmp->pcb->myThread; break;}
	}
#ifndef BCC_BLOCK_IGNORE
	unlock
#endif
	return t;
}

Queue::~Queue() {
#ifndef BCC_BLOCK_IGNORE
	lock
#endif
	//cout << "aaa\n";
	Elem* tmp;
	while(front != 0) {
	tmp = front;
	front = front->next;
	delete tmp;
	}
	rear = 0;
#ifndef BCC_BLOCK_IGNORE
	unlock
#endif
}

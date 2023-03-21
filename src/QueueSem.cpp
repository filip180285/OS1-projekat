#include "QueueSem.h"
#include "KernelS.h"
#include "Init.h"

ElemSem::ElemSem(KernelSem* k) {
	ks = k; next=0;
}

QueueSem::QueueSem() {
#ifndef BCC_BLOCK_IGNORE
	lock
#endif
	front = rear = 0;
#ifndef BCC_BLOCK_IGNORE
	unlock
#endif
}

void QueueSem::put(KernelSem* ks) {
#ifndef BCC_BLOCK_IGNORE
	lock
#endif
	ElemSem* tmp = new ElemSem(ks);
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

/*
KernelSem* QueueSem::get() {
	if(front == 0) return 0;
#ifndef BCC_BLOCK_IGNORE
	lock
#endif
	KernelSem* ks = front->ks;
	ElemSem *tmp = front;
	front = front->next;
	if(front == 0) rear = 0;
	delete tmp;
#ifndef BCC_BLOCK_IGNORE
	unlock
#endif
	return ks;
}*/

QueueSem::~QueueSem() {
#ifndef BCC_BLOCK_IGNORE
	lock
#endif
	ElemSem* tmp;
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

#include "Semaphor.h"
#include "Init.h"
#include "KernelS.h"
#include "QueueSem.h"

Semaphore::Semaphore(int init) {
#ifndef BCC_BLOCK_IGNORE
	lock
#endif
	myImpl = new KernelSem(init);
	Init::Semaphore_List->put(myImpl);
#ifndef BCC_BLOCK_IGNORE
	unlock
#endif
}

Semaphore::~Semaphore() {
#ifndef BCC_BLOCK_IGNORE
	lock
#endif
	 ElemSem* old = 0; ElemSem* prev = 0;
		for (ElemSem* tmp = Init::Semaphore_List->front; tmp != 0;
				) {
				if(tmp->ks == myImpl){
					if (tmp == Init::Semaphore_List->front) {
						Init::Semaphore_List->front = Init::Semaphore_List->front->next;
				    }
					old = tmp;
					tmp = tmp->next;
					if(prev != 0)
					prev->next = tmp;
					if (Init::Semaphore_List->rear == old) {
						Init::Semaphore_List->rear = prev;
					}
					delete old;
					break;
				}
				prev = tmp;
				tmp = tmp->next;
		}
	delete myImpl;
	//myImpl = 0;
#ifndef BCC_BLOCK_IGNORE
	unlock
#endif
}

int Semaphore::wait(Time maxTimeToWait) {
#ifndef BCC_BLOCK_IGNORE
	lock
#endif
	int valueWait = myImpl->wait(maxTimeToWait);
#ifndef BCC_BLOCK_IGNORE
	unlock
#endif
	return valueWait;
}

void Semaphore::signal() {
#ifndef BCC_BLOCK_IGNORE
	lock
#endif
	myImpl->signal();
#ifndef BCC_BLOCK_IGNORE
	unlock
#endif
}

int Semaphore::val() const {
#ifndef BCC_BLOCK_IGNORE
	lock
#endif
	int value = myImpl->value;
#ifndef BCC_BLOCK_IGNORE
	unlock
#endif
	return value;
}

#ifndef _queuesem_h_
#define _queuesem_h_

class KernelSem;

class ElemSem {
public:
	KernelSem* ks;
	ElemSem* next;
	ElemSem(KernelSem* k);
};

class QueueSem {
public:
	QueueSem();
	void put(KernelSem* ks);
	//KernelSem* get();
	~QueueSem();
	ElemSem* front,*rear;
};

#endif

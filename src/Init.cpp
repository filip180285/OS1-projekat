#include "init.h"
#include <dos.h>
#include "PCB.h"
#include "Thread.h"
#include "Queue.h"
#include "QueueSem.h"
#include "KernelS.h"
#include "IdleT.h"
#include "SCHEDULE.h"
#include <stdlib.h>
#include <iostream.h>

volatile PCB* Init::running = 0;
void interrupt (*Init::old)(...) = 0;
Thread* Init::starting = 0;
IdleT* Init::idle = 0;
Queue* Init::PCB_List = new Queue();
unsigned Init::context_switch_on_demand = 0;
QueueSem* Init::Semaphore_List = new QueueSem();
//unsigned Init::tss = 0;
//unsigned Init::tsp = 0;
//unsigned Init::tbp = 0;
char Init::status = 0;

extern void tick();

void Init::initialize() {
#ifndef BCC_BLOCK_IGNORE
	lock
	old = getvect(0x08);
	setvect(0x08,timer);
#endif
	starting = new Thread(defaultStackSize,1);
	starting->myPCB->state = PCB::READY;
	running = (volatile PCB*)starting->myPCB;
	idle = new IdleT();
	idle->start();
	/*status = inportb(0x64);
	cout << (int)status << "\n" << "zavrsio init\n"; asm cli;
	outportb(0x64, 0b00000001);
    cout << "zavrsio init\n";
    asm cli;*/
#ifndef BCC_BLOCK_IGNORE
	unlock
#endif
}

void Init::restore() {
#ifndef BCC_BLOCK_IGNORE
	lock
	setvect(0x08,old);
	delete idle; delete starting; delete PCB_List; delete Semaphore_List;
	context_switch_on_demand = 0;
	asm cli;
	unlock
#endif
}

void Init::dispatch() {
#ifndef BCC_BLOCK_IGNORE
	lock
#endif
	context_switch_on_demand = 1;
	timer();
#ifndef BCC_BLOCK_IGNORE
	unlock
#endif
}

void interrupt Init::timer(...) {
	if(context_switch_on_demand == 0) {
		tick();
		old();
		updateTimeOnSemaphores();
	}

	if(context_switch_on_demand == 0) {
		if(running->timeSlice != 0) running->timePassed++;
		if(running->timeSlice == 0 || running->timePassed != running->timeSlice) return;
	}

	if(running != (volatile PCB*)idle->myPCB && running->state == PCB::READY) Scheduler::put((PCB*)running);

#ifndef BCC_BLOCK_IGNORE
	running->sp = _SP;
	running->ss = _SS;
	running->bp = _BP;
#endif

	running = (volatile PCB*)Scheduler::get();
	if(running == 0) {//cout << "idle\n"; asm cli;
	running = (volatile PCB*)idle->myPCB;
	}

#ifndef BCC_BLOCK_IGNORE
	_SP = running->sp;
	_SS = running->ss;
	_BP = running->bp;
#endif
	//cout << "\n" << (int)status << "\n"; asm cli;
	context_switch_on_demand = 0;
	running->timePassed = 0;
}

void Init::updateTimeOnSemaphores() {
	Elem* old = 0; Elem* prev = 0;
	for (ElemSem* tmpSem = Semaphore_List->front; tmpSem != 0;
			tmpSem = tmpSem->next) {
		old = 0;prev = 0;
		for(Elem* tmpElem = tmpSem->ks->waitingOnTimeOrSignal->front; tmpElem != 0;
				) {
			PCB* tmpPCB = tmpElem->pcb;
			tmpPCB->semTime->maxTimeToWait--;
			if(tmpPCB->semTime->maxTimeToWait == 0) {
				tmpPCB->semTime->returnValue = 0;
				tmpPCB->state = PCB::READY;
				Scheduler::put(tmpPCB);
				//tmpSem->ks->value++;
				if (tmpElem == tmpSem->ks->waitingOnTimeOrSignal->front) {
					tmpSem->ks->waitingOnTimeOrSignal->front = tmpSem->ks->waitingOnTimeOrSignal->front->next;
			    }
				old = tmpElem;
				tmpElem = tmpElem->next;
				if(prev != 0)
				prev->next = tmpElem;
				if (tmpSem->ks->waitingOnTimeOrSignal->rear == old) {
					tmpSem->ks->waitingOnTimeOrSignal->rear = prev;
				}
				delete old;
				continue;
			}
			prev = tmpElem;
			tmpElem = tmpElem->next;
		}
	}
}

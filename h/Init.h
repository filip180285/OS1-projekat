#ifndef _init_h
#define _init_h

class PCB;
class Thread;
class Queue;
class IdleT;
class QueueSem;

#define lock {asm{pushf; cli;}}
#define unlock {asm{popf}}

class Init {
public:
	static void initialize();
	static void restore();
	static void dispatch();

	static void interrupt timer(...);
	static void interrupt (*old)(...);
	static void updateTimeOnSemaphores();

	static volatile PCB* running;
	static Thread* starting;
	static IdleT* idle;
	static Queue* PCB_List;
	static QueueSem* Semaphore_List;
	static unsigned context_switch_on_demand;
	//static unsigned tss,tsp,tbp;
	static char status;
};

#endif

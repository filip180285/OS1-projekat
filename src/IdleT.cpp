#include "IdleT.h"
#include "PCB.h"
#include "Init.h"
#include "iostream.h"

//int IdleT::a = 0;

void IdleT::start() {
#ifndef BCC_BLOCK_IGNORE
	lock
#endif
	myPCB->makeStack();
	myPCB->state = PCB::READY;
#ifndef BCC_BLOCK_IGNORE
	unlock
#endif
}

IdleT::IdleT():Thread(defaultStackSize,1) { }

void IdleT::run() {
	while(1);
}

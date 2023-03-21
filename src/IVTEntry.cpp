#include "IVTEntry.h"
#include <dos.h>
#include "Init.h"
#include <iostream.h>

IVTEntry* IVTEntry::entries[NUMBER_OF_ENTRIES] = {0};

IVTEntry::IVTEntry(IVTNo numEntry,ptrInterrupt myIR) {
#ifndef BCC_BLOCK_IGNORE
	lock
#endif
	//cout << "kreirao ivtentry\n"; asm cli;
	numberOfEntry = numEntry;
	myEvent = 0;
#ifndef BCC_BLOCK_IGNORE
	oldIR = getvect(numEntry);
	setvect(numEntry,myIR);
#endif
	entries[numEntry] = this;
#ifndef BCC_BLOCK_IGNORE
	unlock
#endif
}

IVTEntry::~IVTEntry() {
#ifndef BCC_BLOCK_IGNORE
	lock
#endif
	oldIR();
#ifndef BCC_BLOCK_IGNORE
	setvect(numberOfEntry,oldIR);
	unlock
#endif
}

void IVTEntry::signal() {
#ifndef BCC_BLOCK_IGNORE
	lock
#endif
	if(myEvent != 0) myEvent->signal();
#ifndef BCC_BLOCK_IGNORE
	unlock
#endif
}

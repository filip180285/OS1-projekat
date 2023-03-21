#ifndef _event_h_
#define _event_h_

#include "Init.h"
#include "IVTEntry.h"

typedef unsigned char IVTNo;
class KernelEv;

#define PREPAREENTRY(numEntry, callOld) \
void interrupt inter##numEntry(...); \
IVTEntry newEntry##numEntry(numEntry, inter##numEntry); \
void interrupt inter##numEntry(...) { \
newEntry##numEntry.signal();\
if(callOld == 1) newEntry##numEntry.oldIR(); \
dispatch(); \
}

class KernelEv;

class Event {
public:
	Event (IVTNo ivtNo);
	~Event ();

	void wait ();

protected:
	friend class KernelEv;
	void signal();

private:
	KernelEv* myImpl;
};

#endif

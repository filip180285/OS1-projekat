#include <iostream.h>
#include "Init.h"
#include "IdleT.h"
#include "Thread.h"
#include "Queue.h"
//#include "PCB.h"

extern int userMain(int argc,char* argv[]);

int main(int argc,char* argv[]) {
	Init::initialize();
	int value = userMain(argc,argv);
	Init::restore();
	cout << value;
	return value;

	/*
	cout << 2;
	return 0;*/
}



#include <iostream>
#include "conio.h"
#include "ReliabilityDomain.h"
#include <string>
extern "C" {
#include "tcl.h"
}

#pragma comment(lib,"tcl84.lib")

using namespace std;


int main(int argc, char *argv[]) {
	Tcl_Interp *interp;
	interp = Tcl_CreateInterp();
	Tcl_Main(argc,argv,Tcl_AppInit);
	system("pause");
	return 0;
}

int OpenSeesExit(ClientData clientData, Tcl_Interp *interp, int argc, const char **argv)
{
	Tcl_Exit(0);
	return 0;
}


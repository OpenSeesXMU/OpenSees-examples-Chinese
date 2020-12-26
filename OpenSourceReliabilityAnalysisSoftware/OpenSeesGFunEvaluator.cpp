/* ****************************************************************** **
**    OpenSees - Open System for Earthquake Engineering Simulation    **
**          Pacific Earthquake Engineering Research Center            **
**                                                                    **
**                                                                    **
** (C) Copyright 2001, The Regents of the University of California    **
** All Rights Reserved.                                               **
**                                                                    **
** Commercial use of this program without express permission of the   **
** University of California, Berkeley, is strictly prohibited.  See   **
** file 'COPYRIGHT'  in main directory for information on usage and   **
** redistribution,  and for a DISCLAIMER OF ALL WARRANTIES.           **
**                                                                    **
** Developed by:                                                      **
**   Frank McKenna (fmckenna@ce.berkeley.edu)                         **
**   Gregory L. Fenves (fenves@ce.berkeley.edu)                       **
**   Filip C. Filippou (filippou@ce.berkeley.edu)                     **
**                                                                    **
** Reliability module developed by:                                   **
**   Terje Haukaas (haukaas@ce.berkeley.edu)                          **
**   Armen Der Kiureghian (adk@ce.berkeley.edu)                       **
**                                                                    **
** ****************************************************************** */
                                                                        
// $Revision: 1.9 $
// $Date: 2003/10/27 23:45:43 $
// $Source: /usr/local/cvs/OpenSees/SRC/reliability/analysis/gFunction/OpenSeesGFunEvaluator.cpp,v $


//
// Written by Terje Haukaas (haukaas@ce.berkeley.edu)
//

#include "OpenSeesGFunEvaluator.h"
#include "Vector.h"
#include "GFunEvaluator.h"
#include "ReliabilityDomain.h"
#include "LimitStateFunction.h"
#include "RandomVariablePositioner.h"

#include <tcl.h>
#include <string.h>
#include <stdio.h>
#ifdef _WIN32
#include <dos.h>
#include <windows.h>
#include<ShellAPI.h>    // ShellExecuteEx
#else
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#endif
#include <tchar.h>

#include <fstream>
#include <iomanip>
#include <iostream>
using std::cout;
using std::ifstream;
using std::ios;
using std::setw;
using std::setprecision;
using namespace std;

#pragma warning(disable:4996)

OpenSeesGFunEvaluator::OpenSeesGFunEvaluator(Tcl_Interp *passedTclInterp,
					ReliabilityDomain *passedReliabilityDomain,
					TCL_Char *passedFileName, double p_dt)
:GFunEvaluator(passedTclInterp, passedReliabilityDomain)
{
	fileName = new char[256];
	strcpy(fileName,passedFileName);
	nsteps = 0;
	dt = p_dt;
	// (here the user has provided a file with the analysis commmands)

	createRecorders();

}

OpenSeesGFunEvaluator::OpenSeesGFunEvaluator(Tcl_Interp *passedTclInterp,
	ReliabilityDomain *passedReliabilityDomain,
	int p_nsteps, double p_dt, TCL_Char *passedFileName)
	:GFunEvaluator(passedTclInterp, passedReliabilityDomain)
{
	nsteps = p_nsteps;
	dt = p_dt;
	fileName = new char[256];
	strcpy(fileName, passedFileName);
	
	// (here the user has provided a file with the analysis commmands)

	createRecorders();
}

OpenSeesGFunEvaluator::OpenSeesGFunEvaluator(Tcl_Interp *passedTclInterp,
					ReliabilityDomain *passedReliabilityDomain,
					int p_nsteps, double p_dt)
:GFunEvaluator(passedTclInterp, passedReliabilityDomain)
{
	fileName = 0;
	nsteps = p_nsteps;
	dt = p_dt;
	// (here the user has specified number of steps and possibly dt)

	createRecorders();
}

OpenSeesGFunEvaluator::~OpenSeesGFunEvaluator()
{
	if (fileName != 0)
		delete [] fileName;
}


int
OpenSeesGFunEvaluator::runGFunAnalysis(Vector x)
{
	//////////////指定改哪一行的数，并将其替换到RV.txt文档中
	//int numberRV = sizeof(x) / sizeof(int);
	ofstream outfile;
	outfile.precision(16);
	outfile.open("RV.txt");
	for (int i = 0; i < x.Size(); i++) {
		outfile << x[i] << endl;
	}
	outfile.close();
	//////////////指定改哪一行的数，并将其替换到RV.txt文档中
	//run OpenSees analysis

#ifdef _WIN32

	char *args[] = { fileName };
	system("OpenSees.exe test13.tcl");
	/*
	SHELLEXECUTEINFO  ShExecInfo = { 0 };
	ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
	ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
	ShExecInfo.hwnd = NULL;
	ShExecInfo.lpVerb = NULL;
	ShExecInfo.lpFile = "OpenSees.exe";
	ShExecInfo.lpParameters = fileName;
	ShExecInfo.lpDirectory = NULL;
	ShExecInfo.nShow = SW_HIDE;
	ShExecInfo.hInstApp = NULL;
	ShellExecuteEx(&ShExecInfo);

	WaitForSingleObject(ShExecInfo.hProcess, INFINITE);
*/
	
#else

	// vfork and execv

	pid_t pid = vfork();
	if (pid == -1)
	{
		perror("fork() failed");
		return -1;
	}
	else if (pid == 0)
	{
		char *args[] = { fileName };
		execv("/Users/fmckenna/bin/OpenSees", args);
	}

	int child_status;
	int child_pid = wait(&child_status);


	// or try popen
#endif

	/*

	SHELLEXECUTEINFO  ShExecInfo = { 0 };
	ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
	ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
	ShExecInfo.hwnd = NULL;
	ShExecInfo.lpVerb = NULL;
	ShExecInfo.lpFile = "OpenSees.exe";
	ShExecInfo.lpParameters = fileName;
	ShExecInfo.lpDirectory = NULL;
	ShExecInfo.nShow = SW_HIDE;
	ShExecInfo.hInstApp = NULL;
	ShellExecuteEx(&ShExecInfo);
	//等到程序跑完了再干其他事情
	WaitForSingleObject(ShExecInfo.hProcess, INFINITE);

	/*if (ShExecInfo.hProcess != NULL)
	{
		TerminateProcess(ShExecInfo.hProcess, 0);
		ShExecInfo.hProcess = NULL;
	}
	*/
	
	//HINSTANCE hNewExe = ShellExecuteA(NULL, "open", "OpenSees.exe", fileName, NULL, SW_SHOW);

   return 0;
}




int
OpenSeesGFunEvaluator::tokenizeSpecials(TCL_Char *theExpression)
{
	// Set value of OpenSees finite element response quantities 
	// appearing in the limit-state function in the Tcl domain
	char buf[500]="";
	char tempchar[100]="";
	int i;
	double fileValue = 0.0;
	char tclAssignment[500]="";
	char temp[120];


	char separators[5] = "}{";
	char lsf_forTokenizing[1000];
	strcpy(lsf_forTokenizing,theExpression);
	char lsf_expression[1000] = "";
	char *dollarSign = (char *)"$";
	char *underscore = (char *)"_";
	char *tokenPtr = strtok( lsf_forTokenizing, separators);
	while ( tokenPtr != NULL ) {

		strcpy(tempchar,tokenPtr);

		// If a nodal velocity is detected
		if ( strncmp(tokenPtr, "ud", 2) == 0) {

			// Get node number and dof number
			int nodeNumber, direction;
			sscanf(tempchar,"ud_%i_%i", &nodeNumber, &direction);

			// Assign value to the displacement quantity
			char tclAssignment[100];

			///////////////////////读取数据
			double performance_b;
			ifstream infile;
			infile.open("PerformanceV.txt");      //注意文件的路径
			infile >> performance_b;                   //两行数据可以连续读出到变量里
			infile.close();
			sprintf(tclAssignment, "set ud_%d_%d %.14f", nodeNumber, direction, performance_b);
			//sprintf(tclAssignment,"set ud_%d_%d [nodeVel %d %d ]",nodeNumber,direction,nodeNumber,direction);
			Tcl_Eval( theTclInterp, tclAssignment);
		}
		// If a nodal displacement is detected
		else if ( strncmp(tokenPtr, "u", 1) == 0) {

			// Get node number and dof number
			int nodeNumber, direction;
			sscanf(tempchar,"u_%i_%i", &nodeNumber, &direction);

			// Assign value to the displacement quantity
			char tclAssignment[100];

			///////////////////////读取数据
			double performance_a;
			ifstream infile;
			infile.open("PerformanceU.txt");      //注意文件的路径
			infile >> performance_a;                   //两行数据可以连续读出到变量里
			infile.close();
			sprintf(tclAssignment,"set u_%d_%d %.14f", nodeNumber, direction, performance_a);
			Tcl_Eval( theTclInterp, tclAssignment);

// Quan for debug ----

//			sprintf(tclAssignment,"puts \"[expr $u_%d_%d]\" ",nodeNumber,direction);
//			Tcl_Eval( theTclInterp, tclAssignment);


		}
		else if ( strncmp(tokenPtr, "rec",3) == 0) {

			// Determine file name 
			int lineNum = 0;
			int colNum = 0;
			char *variableName=0;
			if ( strncmp(tokenPtr, "rec_node",8) == 0) {
				variableName = rec_node_occurrence(tempchar, false, lineNum, colNum);
			}
			else if ( strncmp(tokenPtr, "rec_element",11) == 0) {
				variableName = rec_element_occurrence(tempchar, false, lineNum, colNum);
			}
			char fileName[100];
			sprintf(fileName,"%s.out",variableName);

			// Open the file
			ifstream inputFile(fileName,ios::in);
			if (!inputFile) {
				cout << "Could not open file with quantities for limit-state function." << endl;
			}

			// Count number of lines
			int numberOfLines = 0;
			while (inputFile.getline(buf,120)) {
				numberOfLines++;
			}
			
			// Close and re-open file
			inputFile.close();
			ifstream inputFile2(fileName,ios::in);
			
			// Now pick last line and the right column
			for (i=1; i<numberOfLines; i++) {
				inputFile2.getline(buf,120);
			}
			for (i=1; i<=colNum; i++) {
				inputFile2 >> temp;
			}
			fileValue = (double)atof(temp);
			if (fileValue == 0.0) {
				cout << "ERROR: Could not find quantity in recorder file." << endl;
				return -1;
			}
			inputFile2.close();


			// Make the variable assignment
			sprintf(tclAssignment, "set %s  %25.20e", variableName, fileValue);
			Tcl_Eval( theTclInterp, tclAssignment);

			delete variableName;

		}

		tokenPtr = strtok( NULL, separators);
	}

//	delete [] lsf_forTokenizing;

	// Re-create possible recorders for subsequent analyses
	createRecorders();

	return 0;
}


void
OpenSeesGFunEvaluator::setNsteps(int p_nsteps)
{
	nsteps = p_nsteps;
}

double
OpenSeesGFunEvaluator::getDt()
{
	return dt;
}





int
OpenSeesGFunEvaluator::createRecorders()
{


//	removeRecorders();



	// Download active limit-state function
	int lsf = theReliabilityDomain->getTagOfActiveLimitStateFunction();
	LimitStateFunction *theLimitStateFunction = theReliabilityDomain->getLimitStateFunctionPtr(lsf);
	char *theExpression = theLimitStateFunction->getExpression();

	// Initial declarations
	char tempchar[100]="";
	char separators[5] = "}{";
	char lsf_forTokenizing[500];
	strcpy(lsf_forTokenizing,theExpression);

	// Go through the limit-state function
	char *tokenPtr = strtok( lsf_forTokenizing, separators);
	while ( tokenPtr != NULL ) {

		strcpy(tempchar,tokenPtr);

		if ( strncmp(tokenPtr, "rec_node",8) == 0) {
			int dummy1, dummy2;
			rec_node_occurrence(tempchar, true, dummy1, dummy2);
		}
		else if ( strncmp(tokenPtr, "rec_element",11) == 0) {
			int dummy1, dummy2;
			rec_element_occurrence(tempchar, true, dummy1, dummy2);
		}

		tokenPtr = strtok( NULL, separators);
	}

//	delete [] lsf_forTokenizing;

	return 0;
}




int
OpenSeesGFunEvaluator::removeRecorders()
{
	char tclAssignment[50] = "";
	sprintf(tclAssignment , "remove recorders");
	Tcl_Eval( theTclInterp, tclAssignment);

	return 0;
}

char *
OpenSeesGFunEvaluator::rec_node_occurrence(char tempchar[100], bool createRecorder, int &line, int &col)
{
	// Initial declarations
	char tclAssignment[500]="";


	// Get node and dof number etc.
	int nodeNumber, direction;
	char dispOrWhat[10];
	char restString[100];
	sscanf(tempchar,"rec_node_%s",restString);
	if ( strncmp(restString, "disp",4) == 0) {
		// recorder Node fileName.out disp  -time -node 7 -dof 1  {rec_node_disp_7_1}
		strcpy(dispOrWhat,"disp");
		sscanf(restString,"disp_%i_%i", &nodeNumber, &direction);
	}
	else if ( strncmp(restString, "vel",3) == 0) {
		// recorder Node fileName.out vel   -time -node 7 -dof 1  {rec_node_vel_7_1}
		strcpy(dispOrWhat,"vel");
		sscanf(restString,"vel_%i_%i", &nodeNumber, &direction);
	}
	else if ( strncmp(restString, "accel",5) == 0) {
		// recorder Node fileName.out accel -time -node 7 -dof 1  {rec_node_accel_7_1}
		strcpy(dispOrWhat,"accel");
		sscanf(restString,"accel_%i_%i", &nodeNumber, &direction);
	}
	else {
		cout << "ERROR in syntax of limit-state function with recorder." << endl;
	}

	// Determine variable name 
	char *variableName;
	char tempString[100];
	sprintf(tempString, "rec_node_%s_%d_%d", dispOrWhat, nodeNumber, direction);
	variableName = new char[100];
	strcpy(variableName,tempString);

	if (createRecorder) {

		// Create the recorder
		sprintf(tclAssignment , "recorder Node %s.out %s -time -node %d -dof %d",variableName,dispOrWhat,nodeNumber,direction);
		Tcl_Eval( theTclInterp, tclAssignment);

		// Possibly create DDM gradient recorders				
		sprintf(tclAssignment , "recorder NodeGrad %s_ddm.out %s -time -node %d -dof %d",variableName,dispOrWhat,nodeNumber,direction);
//		Tcl_Eval( theTclInterp, tclAssignment);

		delete variableName;
		return 0;
	}

	line = 0;
	col = 2;
	return variableName;
}

char *
OpenSeesGFunEvaluator::rec_element_occurrence(char tempchar[100], bool createRecorder, int &line, int &col)
{

	// Initial declarations
	char tclAssignment[500]="";
	char restString[100];
	char tempString[100];

	// Start obtaining information about element number etc. 
	int eleNumber;
	sscanf(tempchar,"rec_element_%i_%s", &eleNumber, restString);

	if ( strncmp(restString, "globalForce",11) == 0) {
		// recorder Element 5 fileName.out -time globalForce {rec_element_5_globalForce_2}
		int globalForceComponent;
		sscanf(restString,"globalForce_%i", &globalForceComponent);
		sprintf(tempString, "rec_element_%d_globalForce_%d", eleNumber, globalForceComponent);
		line = 0;
		col = globalForceComponent+1;
		if (createRecorder) {
			sprintf(tclAssignment , "recorder Element %d -file %s.out -time globalForce",eleNumber, tempString);
			Tcl_Eval( theTclInterp, tclAssignment);
		}
	}
	else if ( strncmp(restString, "localForce",10) == 0) {
		// recorder Element 5 fileName.out -time localForce {rec_element_5_localForce_2}
		int localForceComponent;
		sscanf(restString,"localForce_%i", &localForceComponent);
		sprintf(tempString, "rec_element_%d_localForce_%d", eleNumber, localForceComponent);
		line = 0;
		col = localForceComponent+1;
		if (createRecorder) {
			sprintf(tclAssignment , "recorder Element %d -file %s.out -time localForce",eleNumber, tempString);
			Tcl_Eval( theTclInterp, tclAssignment);
		}
	}
	else if ( strncmp(restString, "section",7) == 0) {
		int sectionNumber;
		sscanf(restString,"section_%i_%s", &sectionNumber, restString);
		if ( strncmp(restString, "force",5) == 0) {
			// recorder Element 5 fileName.out -time section 2 force {rec_element_5_section_2_force_1}
			int sectionForceComponent;
			sscanf(restString,"force_%i", &sectionForceComponent);
			sprintf(tempString, "rec_element_%d_section_%d_force_%d", eleNumber, sectionNumber, sectionForceComponent);
			line = 0;
			col = sectionForceComponent+1;
			if (createRecorder) {
				sprintf(tclAssignment , "recorder Element %d -file %s.out -time section %d force",eleNumber, tempString, sectionNumber);
				Tcl_Eval( theTclInterp, tclAssignment);
			}
		}
		else if ( strncmp(restString, "deformation",11) == 0) {
			// recorder Element 5 fileName.out -time section 2 deformation {rec_element_5_section_2_deformation_1}
			int sectionDeformationComponent;
			sscanf(restString,"deformation_%i", &sectionDeformationComponent);
			sprintf(tempString, "rec_element_%d_section_%d_deformation_%d", eleNumber, sectionNumber, sectionDeformationComponent);
			line = 0;
			col = sectionDeformationComponent+1;
			if (createRecorder) {
				sprintf(tclAssignment , "recorder Element %d -file %s.out -time section %d deformation",eleNumber, tempString, sectionNumber);
				Tcl_Eval( theTclInterp, tclAssignment);
			}
		}
		else if ( strncmp(restString, "stiffness",9) == 0) {
			// recorder Element 5 fileName.out -time section 2 stiffness {rec_element_5_section_2_stiffness_1}
			int sectionStiffnessComponent;
			sscanf(restString,"stiffness_%i", &sectionStiffnessComponent);
			sprintf(tempString, "rec_element_%d_section_%d_stiffness_%d", eleNumber, sectionNumber, sectionStiffnessComponent);
			line = 0;
			col = sectionStiffnessComponent+1;
			if (createRecorder) {
				sprintf(tclAssignment , "recorder Element %d -file %s.out -time section %d stiffness",eleNumber, tempString, sectionNumber);
				Tcl_Eval( theTclInterp, tclAssignment);
			}
		}
		else if ( strncmp(restString, "fiber",5) == 0) {
			// recorder Element 5 fileName.out -time section 2 fiber $y $z stressStrain {rec_element_5_section_2_fiber_2_36_6_54_stress}
			int ya, yb, za, zb;
			sscanf(restString,"fiber_%i_%i_%i_%i_%s", &ya, &yb, &za, &zb, restString);
			sprintf(tempString, "rec_element_%d_section_%d_fiber_%d_%d_%d_%d_stressStrain", eleNumber, sectionNumber, ya, yb, za, zb);
			line = 0;
			if (createRecorder) {
				sprintf(tclAssignment , "recorder Element %d -file %s.out -time section %d fiber %d.%d %d.%d stressStrain",eleNumber, tempString, sectionNumber, ya, yb, za, zb);
				Tcl_Eval( theTclInterp, tclAssignment);
			}
			if (strcmp(restString,"stress")==0) {
				col = 2;
			}
			else if (strcmp(restString,"strain")==0) {
				col = 3;
			}
			else {
				cout << "ERROR in syntax of limit-state function with recorder." << endl;
			}
		}
	}
	else {
		cout << "ERROR in syntax of limit-state function with recorder." << endl;
	}


	char *variableName;
	variableName = new char[100];
	strcpy(variableName,tempString);
	return variableName;
}

double OpenSeesGFunEvaluator::getG2(double g, double littleDeltaT)
{
	 
 
	// Initial declaractions
	double perturbationFactor = 0.001; // (is multiplied by stdv and added to others...)
	char tclAssignment[500];
	char *dollarSign = (char *)"$";
	char *underscore = (char *)"_";
	char lsf_expression[500] = "";
	char separators[5] = "}{";
	int nrv = theReliabilityDomain->getNumberOfRandomVariables();
//	RandomVariable *theRandomVariable;
	char tempchar[100]="";
	char newSeparators[5] = "_";
	double g_perturbed;
	Vector dudx(nrv);
	


 

	// "Download" limit-state function from reliability domain
	int lsf = theReliabilityDomain->getTagOfActiveLimitStateFunction();
	LimitStateFunction *theLimitStateFunction = 
	theReliabilityDomain->getLimitStateFunctionPtr(lsf);
	char *theExpression = theLimitStateFunction->getExpression();
	char lsf_copy[500]="";
	strcpy(lsf_copy,theExpression);

	double g2FunctionValue = g;

	// Tokenize the limit-state function and COMPUTE GRADIENTS
	char *tokenPtr = strtok( lsf_copy, separators); 
	while ( tokenPtr != NULL ) {

		strcpy(tempchar,tokenPtr);

		if ( strncmp(tokenPtr, "x",1) == 0) {

			cout<<"Fatal: can not deal with x in limit state function"<<endl; exit(-1);
		}
		// If a nodal velocity is detected
		else if ( strncmp(tokenPtr, "ud", 2) == 0) {

			cout<<"Fatal: can not deal with ud in limit state function"<<endl; exit(-1);
		}
		// If a nodal displacement is detected
		else if ( strncmp(tokenPtr, "u", 1) == 0) {

			// Get node number and dof number
			int nodeNumber, direction;
			sscanf(tempchar,"u_%i_%i", &nodeNumber, &direction);

			// Keep the original value


			double originalValue;
			sprintf(tclAssignment,"$u_%d_%d", nodeNumber, direction);
			Tcl_ExprDouble( theTclInterp, tclAssignment, &originalValue);

			// Set perturbed value in the Tcl workspace

			// ---------------------- Quan and michele   

			double newValue;
			//if (originalValue ==0 ) newValue = 0.0001;
			if (fabs(originalValue)<1.0e-14 ) newValue = 0.0001;  //---  Quan Jan 2006 ---
			else 	newValue = originalValue*(1.0+perturbationFactor);

			sprintf(tclAssignment,"set u_%d_%d %35.20f", nodeNumber, direction, newValue);
			Tcl_Eval( theTclInterp, tclAssignment);

			// Evaluate the limit-state function again
			char *theTokenizedExpression = theLimitStateFunction->getTokenizedExpression();
			Tcl_ExprDouble( theTclInterp, theTokenizedExpression, &g_perturbed );

			// Compute gradient


			double onedgdu = (g_perturbed-g)/(newValue - originalValue);

			// Make assignment back to its original value
			sprintf(tclAssignment,"set u_%d_%d %35.20f", nodeNumber, direction, originalValue);
			Tcl_Eval( theTclInterp, tclAssignment);

	
			double ud_=0.0;
					
			//sprintf(expression,"$ud_%d_%d",nodeNumberAndDOFNumber[0], nodeNumberAndDOFNumber[1]);		
			sprintf(tclAssignment,"set udot [nodeVel  %d  %d]",nodeNumber, direction);		
			if (Tcl_Eval( theTclInterp, tclAssignment) !=TCL_OK ){cout<<"wrong.."<<endl; exit(-1);};
					
			const char * myStr;
			myStr = Tcl_GetVar(theTclInterp, "udot",TCL_GLOBAL_ONLY );


			ud_ = atof(myStr);			
			if (fabs(ud_) <1.e-14) {
				cout<<"warning:OpenSeesGFunEvaluator::getSecondG() velocity is 0! probabily because you are running static case!"<<endl;
				
			}

	
			g2FunctionValue += onedgdu*ud_*littleDeltaT;


		}

		tokenPtr = strtok( NULL, separators);  // read next token and go up and check the while condition again
	} 

	
	return g2FunctionValue;

}




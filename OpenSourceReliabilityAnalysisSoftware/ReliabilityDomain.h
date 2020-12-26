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
                                                                        
// $Revision: 1.5 $
// $Date: 2003/10/27 23:04:38 $
// $Source: /usr/local/cvs/OpenSees/SRC/reliability/domain/components/ReliabilityDomain.h,v $


//
// Written by Terje Haukaas (haukaas@ce.berkeley.edu)
//

#ifndef ReliabilityDomain_h     
#define ReliabilityDomain_h

#include "RandomVariable.h"
#include "CorrelationCoefficient.h"
#include "LimitStateFunction.h"
#include "RandomVariablePositioner.h"
#include "ModulatingFunction.h"
#include "Filter.h"
#include "Spectrum.h"


//#include <ParameterPositioner.h>
//Quan
/*
#include <DesignVariable.h>
#include <DesignVariablePositioner.h>
#include <ConstraintFunction.h>
#include <ObjectiveFunction.h>*/


class TaggedObjectStorage;
class ConstraintFunction;
class ObjectiveFunction;
class DesignVariable;
class DesignVariablePositioner;

class ReliabilityDomain
{

public:
	ReliabilityDomain();
	virtual ~ReliabilityDomain();

	// Member functions to add components to the domain
	virtual bool addRandomVariable(RandomVariable *theRandomVariable);
	virtual bool addCorrelationCoefficient(CorrelationCoefficient *theCorrelationCoefficient);
	virtual bool addLimitStateFunction(LimitStateFunction *theLimitStateFunction);
	virtual bool addRandomVariablePositioner(RandomVariablePositioner *theRandomVariablePositioner);
	virtual bool addFilter(Filter *theFilter);
	virtual bool addModulatingFunction(ModulatingFunction *theModulatingFunction);
	virtual bool addSpectrum(Spectrum *theSpectrum);
	//virtual bool addParameterPositioner(ParameterPositioner *theParameterPositioner);
	
//Quan

	//virtual bool addDesignVariable(DesignVariable *theDesignVariable);
	/*virtual bool addDesignVariablePositioner(DesignVariablePositioner *theDesignVariablePositioner);
	virtual bool addObjectiveFunction(ObjectiveFunction *theObjectiveFunction);
	virtual bool addConstraintFunction(ConstraintFunction *theConstraintFunction);*/

	// Member functions to get components from the domain
	RandomVariable *getRandomVariablePtr(int tag);
	CorrelationCoefficient *getCorrelationCoefficientPtr(int tag);
	LimitStateFunction *getLimitStateFunctionPtr(int tag);
	RandomVariablePositioner *getRandomVariablePositionerPtr(int tag);
	Filter *getFilter(int tag);
	ModulatingFunction *getModulatingFunction(int tag);
	Spectrum *getSpectrum(int tag);
	//ParameterPositioner *getParameterPositionerPtr(int tag);

//Quan
	DesignVariable *getDesignVariablePtr(int tag);
	DesignVariablePositioner *getDesignVariablePositionerPtr(int tag);
	ObjectiveFunction *getObjectiveFunctionPtr(int tag);
	ConstraintFunction *getConstraintFunctionPtr(int tag);
	int setNumberOfRandomVariables( int num);

	// Member functions giving information about the domain
	int getNumberOfRandomVariables(void);
	int getNumberOfCorrelationCoefficients(void);
	int getNumberOfLimitStateFunctions(void);
	int getNumberOfRandomVariablePositioners(void);
	int getNumberOfParameterPositioners(void);
	int getNumberOfModulatingFunctions(void);
	int getNumberOfFilters(void);
	int getNumberOfSpectra(void);

//Quan
	int getNumberOfDesignVariables(void);
	int getNumberOfDesignVariablePositioners(void);
	int getNumberOfObjectiveFunctions(void);
	int getNumberOfConstraintFunctions(void);


	// Member functions to set/get active limit-state function
	int getTagOfActiveLimitStateFunction(void);
	void setTagOfActiveLimitStateFunction(int tag);

	// Member functions to remove single components from the domain
	int removeRandomVariablePositioner(int tag);
	int removeRandomVariable(int tag);
	int removeCorrelationCoefficient(int tag);
	int removePerformanceFunction(int tag);

//Quan
	int removeDesignVariable(int tag);
	int removeDesignVariablePositioner(int tag);
	int removeObjectiveFunction(int tag);
	int removeConstraintFunction(int tag);




protected:

private:
	TaggedObjectStorage *theRandomVariablesPtr;
	TaggedObjectStorage *theCorrelationCoefficientsPtr;
	TaggedObjectStorage *theLimitStateFunctionsPtr;
	TaggedObjectStorage *theRandomVariablePositionersPtr;
	TaggedObjectStorage *theParameterPositionersPtr;
	TaggedObjectStorage *theModulatingFunctionsPtr;
	TaggedObjectStorage *theFiltersPtr;
	TaggedObjectStorage *theSpectraPtr;
	int tagOfActiveLimitStateFunction;


//Quan
	TaggedObjectStorage *theDesignVariablesPtr;
	TaggedObjectStorage *theObjectiveFunctionsPtr;
	TaggedObjectStorage *theConstraintFunctionsPtr;
	TaggedObjectStorage *theDesignVariablePositionersPtr;
//	int tagOfActiveObjectiveFunction;  // is this needed?

	ReliabilityDomain *theReliabilityDomain;
};

#endif



/* 
   A C-program for MT19937, with initialization improved 2002/1/26.
   Coded by Takuji Nishimura and Makoto Matsumoto.

   Before using, initialize the state by using init_genrand(seed)  
   or init_by_array(init_key, key_length).

   Copyright (C) 1997 - 2002, Makoto Matsumoto and Takuji Nishimura,
   All rights reserved.                          

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions
   are met:

     1. Redistributions of source code must retain the above copyright
        notice, this list of conditions and the following disclaimer.

     2. Redistributions in binary form must reproduce the above copyright
        notice, this list of conditions and the following disclaimer in the
        documentation and/or other materials provided with the distribution.

     3. The names of its contributors may not be used to endorse or promote 
        products derived from this software without specific prior written 
        permission.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
   "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
   A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR
   CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
   EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
   PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
   PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
   LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
   NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
   SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.


   Any feedback is very welcome.
   http://www.math.sci.hiroshima-u.ac.jp/~m-mat/MT/emt.html
   email: m-mat @ math.sci.hiroshima-u.ac.jp (remove space)
*/

// --- wrapped by Quan Gu qgu@ucsd.edu June 10 2009


#include "MersenneTwister.h"

//#include <fstream.h>

#include "RandomNumberGenerator.h"
#include "NormalRV.h"
#include "Vector.h"
#include <iostream>
using namespace std;

MersenneTwister::MersenneTwister( unsigned long int pSeed)
:RandomNumberGenerator()
{

	
	
	// --- define constant ---

	N = 624;
    M = 397;
    MATRIX_A = 0x9908b0dfUL;   /* constant vector a */
    UPPER_MASK = 0x80000000UL; /* most significant w-r bits */
    LOWER_MASK = 0x7fffffffUL; /* least significant r bits */

    mt = new unsigned long [N]; /* the array for the state vector  */
    mti=N+1; /* mti==N+1 means mt[N] is not initialized */
	
	restartArray =0;

	isRestart = false;
	
	setSeed (pSeed);  // default value

// ----
	generatedNumbers = 0;

}

MersenneTwister::~MersenneTwister()
{
	if (mt !=0) delete mt;
	if (restartArray !=0) delete restartArray;
	if ( generatedNumbers ==0) delete generatedNumbers;

}


void
MersenneTwister::setSeed (int pSeed) {

	seed = pSeed;
	unsigned long init[4]={0x123, 0x234, 0x345, 0x456}, length=4;
	init_by_array(init, length);
	
}

void MersenneTwister::init_genrand(unsigned long s)
{
    mt[0]= s & 0xffffffffUL;
    for (mti=1; mti<N; mti++) {
        mt[mti] = 
	    (1812433253UL * (mt[mti-1] ^ (mt[mti-1] >> 30)) + mti); 
        /* See Knuth TAOCP Vol2. 3rd Ed. P.106 for multiplier. */
        /* In the previous versions, MSBs of the seed affect   */
        /* only MSBs of the array mt[].                        */
        /* 2002/01/09 modified by Makoto Matsumoto             */
        mt[mti] &= 0xffffffffUL;
        /* for >32 bit machines */
    }
};

/* initialize by an array with array-length */
/* init_key is the array for initializing keys */
/* key_length is its length */
/* slight change for C++, 2004/2/26 */
void MersenneTwister::init_by_array(unsigned long init_key[], int key_length)
{
    int i, j, k;
    init_genrand(this->seed);
    i=1; j=0;
    k = (N>key_length ? N : key_length);
    for (; k; k--) {
        mt[i] = (mt[i] ^ ((mt[i-1] ^ (mt[i-1] >> 30)) * 1664525UL))
          + init_key[j] + j; /* non linear */
        mt[i] &= 0xffffffffUL; /* for WORDSIZE > 32 machines */
        i++; j++;
        if (i>=N) { mt[0] = mt[N-1]; i=1; }
        if (j>=key_length) j=0;
    }
    for (k=N-1; k; k--) {
        mt[i] = (mt[i] ^ ((mt[i-1] ^ (mt[i-1] >> 30)) * 1566083941UL))
          - i; /* non linear */
        mt[i] &= 0xffffffffUL; /* for WORDSIZE > 32 machines */
        i++;
        if (i>=N) { mt[0] = mt[N-1]; i=1; }
    }

    mt[0] = 0x80000000UL; /* MSB is 1; assuring non-zero initial array */ 
}

/* generates a random number on [0,0xffffffff]-interval */
unsigned long MersenneTwister::genrand_int32(void)
{
    unsigned long y;
    static unsigned long mag01[2]={0x0UL, MATRIX_A};
    /* mag01[x] = x * MATRIX_A  for x=0,1 */

    if (mti >= N) { /* generate N words at one time */
        int kk;

        if (mti == N+1)   /* if init_genrand() has not been called, */
            init_genrand(this->seed); /* a default initial seed is used */

        for (kk=0;kk<N-M;kk++) {
            y = (mt[kk]&UPPER_MASK)|(mt[kk+1]&LOWER_MASK);
            mt[kk] = mt[kk+M] ^ (y >> 1) ^ mag01[y & 0x1UL];
        }
        for (;kk<N-1;kk++) {
            y = (mt[kk]&UPPER_MASK)|(mt[kk+1]&LOWER_MASK);
            mt[kk] = mt[kk+(M-N)] ^ (y >> 1) ^ mag01[y & 0x1UL];
        }
        y = (mt[N-1]&UPPER_MASK)|(mt[0]&LOWER_MASK);
        mt[N-1] = mt[M-1] ^ (y >> 1) ^ mag01[y & 0x1UL];

        mti = 0;
    }
  
    y = mt[mti++];

    /* Tempering */
    y ^= (y >> 11);
    y ^= (y << 7) & 0x9d2c5680UL;
    y ^= (y << 15) & 0xefc60000UL;
    y ^= (y >> 18);

    return y;
}

/* generates a random number on [0,0x7fffffff]-interval */
long MersenneTwister::genrand_int31(void)
{
    return (long)(genrand_int32()>>1);
}

/* generates a random number on [0,1]-real-interval */
double MersenneTwister::genrand_real1(void)
{
    return genrand_int32()*(1.0/4294967295.0); 
    /* divided by 2^32-1 */ 
}

/* generates a random number on [0,1)-real-interval */
double MersenneTwister::genrand_real2(void)
{
    return genrand_int32()*(1.0/4294967296.0); 
    /* divided by 2^32 */
}

/* generates a random number on (0,1)-real-interval */
double MersenneTwister::genrand_real3(void)
{
    return (((double)genrand_int32()) + 0.5)*(1.0/4294967296.0); 
    /* divided by 2^32 */
}

/* generates a random number on [0,1) with 53-bit resolution*/
double MersenneTwister::genrand_res53(void) 
{ 
    unsigned long a=genrand_int32()>>5, b=genrand_int32()>>6; 
    return(a*67108864.0+b)*(1.0/9007199254740992.0); 
} 

 





void MersenneTwister::restart(unsigned long * pRestartArray)
{
	seed = pRestartArray[0];
	mti = (int) pRestartArray[1];
	
	for (int i =0; i<N; i++) mt[i] = pRestartArray[i+2];
	
	isRestart =true;


};

unsigned long *  MersenneTwister::savedRestart(){


	if (restartArray ==0){
		restartArray = new unsigned long [N+2];  // 1. seed 2. mti 3. mt[624]
	}

	restartArray[0]= seed;
	restartArray[1]= (unsigned long) mti;
	
	for (int i =0; i<N; i++) 	restartArray[i+2]= mt[i];
	
	return restartArray;

};


// ============================================================================


int MersenneTwister::generate_nIndependentStdNormalNumbers(int n, int seedIn){

	// Initial declarations
	int j;
	double randomNumberBetween0And1;
	Vector randomArray(n);
	
	NormalRV *aStdNormRV = 0;
	aStdNormRV = new NormalRV(1,0.0,1.0,0.0);


	// Check if computer ran out of memory
	if (aStdNormRV==0) {
		cout << "MersenneTwister::generate_nIndependentStdNormalNumbers() - " << endl
			<< " out of memory while instantiating internal objects." << endl;
		return -1;
	}


	// Create array of standard normal random numbers
	if ((seedIn != 0)&&(!isRestart)) { // do not do in restart case.
		
		setSeed(seedIn);
	
	}

	for ( j=0; j<n; j++)
	{

		// Modify it so that the value lies between 0 and 1
		randomNumberBetween0And1 = (double) this->genrand_res53();;

		// cout<< randomNumberBetween0And1 <<endl; 

		// Treat two special cases
		if (randomNumberBetween0And1 == 0.0) {
			randomNumberBetween0And1 = 0.0000001;
		}
		if (randomNumberBetween0And1 == 1.0) {
			randomNumberBetween0And1 = 0.9999999;
		}


		// Transform that number into a standard normal variable
		//    Phi(z) = F(x)
		//    z = invPhi( F(x) )
		//       where F(x) for the uniform distribution 
		//       from 0 to 1 in fact is equal to x itself.
		randomArray(j) = aStdNormRV->getInverseCDFvalue(randomNumberBetween0And1); 
	}
	

	if (generatedNumbers == 0) {
		generatedNumbers = new Vector(n);
	}
	else if (generatedNumbers->Size() != n) {
		delete generatedNumbers;
		generatedNumbers = new Vector(n);
	}
	(*generatedNumbers) = randomArray;

	delete aStdNormRV;

	return 0;


};

int MersenneTwister::generate_nIndependentUniformNumbers(int n, double lower, double upper, int seedIn){

		// Initial declarations
	int j;
	
	double randomNumberBetween0And1;
	Vector randomArray(n);


	// Create array of standard normal random numbers
	if ((seedIn != 0) &&(!isRestart))	{
		this->setSeed(seedIn);  // run only if at very beginning
		
	}
	

	for ( j=0; j<n; j++)
	{
		// Generate a number between 0 and RAND_MAX
		randomNumberBetween0And1 = this->genrand_res53();

		// Transform according to uniform distribution
		randomArray(j) = (upper-lower)*randomNumberBetween0And1 + lower;
	}

	if (generatedNumbers == 0) {
		generatedNumbers = new Vector(n);
	}
	else if (generatedNumbers->Size() != n) {
		delete generatedNumbers;
		generatedNumbers = new Vector(n);
	}
	(*generatedNumbers) = randomArray;


	return 0;

	
};

Vector	MersenneTwister::getGeneratedNumbers(){

	return (*generatedNumbers); 
};



/* These real versions are due to Isaku Wada, 2002/01/09 added */
/*
int main(void)
{

	ofstream resultsOutputFile( "randomNum.txt", ios::out );
	ofstream resultsOutputFile1( "randomNum3000.txt", ios::out );
    int i;
    


	MersenneTwister * theGenerator = new MersenneTwister(1023);
    // theGenerator->init_by_array(init, length);
    //printf("1000000 outputs of genrand_int32()\n");
    for (i=0; i<3000; i++) {
	  resultsOutputFile1<< theGenerator->genrand_res53() <<"  \n  ";

    }

	delete theGenerator; 


    // --- to verify 'restart': step 1. generate 1000 RNs---

	theGenerator = new MersenneTwister(1023 );
    // theGenerator->init_by_array(init, length);
    //printf("1000000 outputs of genrand_int32()\n");
    for (i=0; i<1000; i++) {
	  resultsOutputFile<< theGenerator->genrand_res53() <<"  \n  ";

    }

	unsigned long savedArray[626];
	
	unsigned long * tmp = theGenerator->savedRestart();
	for ( i =0; i<626; i++){
		savedArray[i] = tmp[i];
	}

	
	delete theGenerator;

	// ---2. generate 1000 RNs---
	
	theGenerator = new MersenneTwister();
    // theGenerator->init_by_array(init, length);
    //printf("1000000 outputs of genrand_int32()\n");
    
	
	theGenerator->restart(savedArray);

	for (i=0; i<1000; i++) {
	  resultsOutputFile<< theGenerator->genrand_res53() <<"  \n  ";

    }

    tmp = theGenerator->savedRestart();
	for ( i =0; i<626; i++){
		savedArray[i] = tmp[i];
	}
	

	delete theGenerator;


	// ---3. generate 1000 RNs---


	theGenerator = new MersenneTwister();
    
    
	theGenerator->restart(savedArray);

    for (i=0; i<1000; i++) {
	  resultsOutputFile<< theGenerator->genrand_res53() <<"  \n  ";

    }

	delete theGenerator;

    resultsOutputFile.close();
	resultsOutputFile1.close();

    return 0;
}
*/

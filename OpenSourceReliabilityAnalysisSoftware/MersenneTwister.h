


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



#if !defined(MersenneTwister_H__)
#define MersenneTwister_H__


#include "RandomNumberGenerator.h"

class MersenneTwister  : public RandomNumberGenerator 
{
public:

	MersenneTwister(unsigned long seed = 19650218UL);
	
	virtual ~MersenneTwister();

private:
	int N;
    int M;
	unsigned long MATRIX_A ; //0x9908b0dfUL   /* constant vector a */
	unsigned long UPPER_MASK ;//  0x80000000UL /* most significant w-r bits */
	unsigned long LOWER_MASK ; //0x7fffffffUL /* least significant r bits */
	unsigned long seed; 
	unsigned long * mt; /* the array for the state vector  */
	int mti; /* mti==N+1 means mt[N] is not initialized */

	unsigned long * restartArray; 

    void init_genrand(unsigned long s);

	Vector *generatedNumbers;
	bool isRestart;

public:
	
	void restart(unsigned long * mt);
	unsigned long * savedRestart();  // save three values in a array:  1, seed . 2: mti( transfered to unsigned long). 3. mt[624]

private:	
	void init_by_array(unsigned long init_key[], int key_length);
	long genrand_int31(void);
	unsigned long genrand_int32(void);
	double genrand_real1(void);	
	double genrand_real2(void);
	double genrand_real3(void);
	double genrand_res53(void);


	// ----	
public:
	int     getSeed() {return (int)seed;};
	void    setSeed(int seed);



// ---
	int		generate_nIndependentStdNormalNumbers(int n, int seed=0);
	int     generate_nIndependentUniformNumbers(int n, double lower, double upper, int seed=0);
	Vector	getGeneratedNumbers();
	

};

#endif // !defined MersenneTwister_H__

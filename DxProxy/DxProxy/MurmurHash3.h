//-----------------------------------------------------------------------------
// MurmurHash3 was written by Austin Appleby, and is placed in the public
// domain. The author hereby disclaims copyright to this source code.

#ifndef _MURMURHASH3_H_
#define _MURMURHASH3_H_

// do not change
#define VIREIO_SEED 12345

#include <sstream>
#include <iomanip>
#include <iostream>

//-----------------------------------------------------------------------------
// Platform-specific functions and macros

// Microsoft Visual Studio

#if defined(_MSC_VER) && (_MSC_VER<1700)

typedef unsigned char uint8_t;
typedef unsigned long uint32_t;
typedef unsigned __int64 uint64_t;

/*typedef struct _Hash128Bit 
{ 
	uint32_t value[4]; 

	std::string ToString()
	{

	}

} Hash128Bit;*/

// Other compilers

#else	// defined(_MSC_VER) && (_MSC_VER<1700)

#include <stdint.h>

#endif // !defined(_MSC_VER) && (_MSC_VER<1700)

//-----------------------------------------------------------------------------

void MurmurHash3_x86_32  ( const void * key, int len, uint32_t seed, void * out );

//void MurmurHash3_x86_128 ( const void * key, int len, uint32_t seed, void * out );

//void MurmurHash3_x64_128 ( const void * key, int len, uint32_t seed, void * out );

//-----------------------------------------------------------------------------

#endif // _MURMURHASH3_H_

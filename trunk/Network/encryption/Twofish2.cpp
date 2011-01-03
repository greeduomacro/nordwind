/***************************************************************************
	TWOFISH2.C	-- Optimized C API calls for TWOFISH AES submission

	Submitters:
		Bruce Schneier, Counterpane Systems
		Doug Whiting,	Hi/fn
		John Kelsey,	Counterpane Systems
		Chris Hall,		Counterpane Systems
		David Wagner,	UC Berkeley

	Code Author:		Doug Whiting,	Hi/fn

	Version  1.00		April 1998

	Copyright 1998, Hi/fn and Counterpane Systems.  All rights reserved.

	Notes:
		*	Optimized version
		*	Tab size is set to 4 characters in this file

***************************************************************************/

#include "Twofish2.hpp"
#include <QtEndian>
#include <qcryptographichash.h>

namespace Twofish2 {

	
/* fixed 8x8 permutation S-boxes */

/***********************************************************************
*  07:07:14  05/30/98  [4x4]  TestCnt=256. keySize=128. CRC=4BD14D9E.
* maxKeyed:  dpMax = 18. lpMax =100. fixPt =  8. skXor =  0. skDup =  6.
* log2(dpMax[ 6..18])=   --- 15.42  1.33  0.89  4.05  7.98 12.05
* log2(lpMax[ 7..12])=  9.32  1.01  1.16  4.23  8.02 12.45
* log2(fixPt[ 0.. 8])=  1.44  1.44  2.44  4.06  6.01  8.21 11.07 14.09 17.00
* log2(skXor[ 0.. 0])
* log2(skDup[ 0.. 6])=   ---  2.37  0.44  3.94  8.36 13.04 17.99
***********************************************************************/
CONST uchar P8x8[2][256] =
{
/*  p0:   */
/*  dpMax      = 10.  lpMax 	 = 64.  cycleCnt=   1  1  1  0. 		*/
/* 817D6F320B59ECA4.ECB81235F4A6709D.BA5E6D90C8F32471.D7F4126E9B3085CA. */
/* Karnaugh maps:
*  0111 0001 0011 1010. 0001 1001 1100 1111. 1001 1110 0011 1110. 1101 0101 1111 1001.
*  0101 1111 1100 0100. 1011 0101 0010 0000. 0101 1000 1100 0101. 1000 0111 0011 0010.
*  0000 1001 1110 1101. 1011 1000 1010 0011. 0011 1001 0101 0000. 0100 0010 0101 1011.
*  0111 0100 0001 0110. 1000 1011 1110 1001. 0011 0011 1001 1101. 1101 0101 0000 1100.
*/
{
0xA9, 0x67, 0xB3, 0xE8, 0x04, 0xFD, 0xA3, 0x76,
0x9A, 0x92, 0x80, 0x78, 0xE4, 0xDD, 0xD1, 0x38,
0x0D, 0xC6, 0x35, 0x98, 0x18, 0xF7, 0xEC, 0x6C,
0x43, 0x75, 0x37, 0x26, 0xFA, 0x13, 0x94, 0x48,
0xF2, 0xD0, 0x8B, 0x30, 0x84, 0x54, 0xDF, 0x23,
0x19, 0x5B, 0x3D, 0x59, 0xF3, 0xAE, 0xA2, 0x82,
0x63, 0x01, 0x83, 0x2E, 0xD9, 0x51, 0x9B, 0x7C,
0xA6, 0xEB, 0xA5, 0xBE, 0x16, 0x0C, 0xE3, 0x61,
0xC0, 0x8C, 0x3A, 0xF5, 0x73, 0x2C, 0x25, 0x0B,
0xBB, 0x4E, 0x89, 0x6B, 0x53, 0x6A, 0xB4, 0xF1,
0xE1, 0xE6, 0xBD, 0x45, 0xE2, 0xF4, 0xB6, 0x66,
0xCC, 0x95, 0x03, 0x56, 0xD4, 0x1C, 0x1E, 0xD7,
0xFB, 0xC3, 0x8E, 0xB5, 0xE9, 0xCF, 0xBF, 0xBA,
0xEA, 0x77, 0x39, 0xAF, 0x33, 0xC9, 0x62, 0x71,
0x81, 0x79, 0x09, 0xAD, 0x24, 0xCD, 0xF9, 0xD8,
0xE5, 0xC5, 0xB9, 0x4D, 0x44, 0x08, 0x86, 0xE7,
0xA1, 0x1D, 0xAA, 0xED, 0x06, 0x70, 0xB2, 0xD2,
0x41, 0x7B, 0xA0, 0x11, 0x31, 0xC2, 0x27, 0x90,
0x20, 0xF6, 0x60, 0xFF, 0x96, 0x5C, 0xB1, 0xAB,
0x9E, 0x9C, 0x52, 0x1B, 0x5F, 0x93, 0x0A, 0xEF,
0x91, 0x85, 0x49, 0xEE, 0x2D, 0x4F, 0x8F, 0x3B,
0x47, 0x87, 0x6D, 0x46, 0xD6, 0x3E, 0x69, 0x64,
0x2A, 0xCE, 0xCB, 0x2F, 0xFC, 0x97, 0x05, 0x7A,
0xAC, 0x7F, 0xD5, 0x1A, 0x4B, 0x0E, 0xA7, 0x5A,
0x28, 0x14, 0x3F, 0x29, 0x88, 0x3C, 0x4C, 0x02,
0xB8, 0xDA, 0xB0, 0x17, 0x55, 0x1F, 0x8A, 0x7D,
0x57, 0xC7, 0x8D, 0x74, 0xB7, 0xC4, 0x9F, 0x72,
0x7E, 0x15, 0x22, 0x12, 0x58, 0x07, 0x99, 0x34,
0x6E, 0x50, 0xDE, 0x68, 0x65, 0xBC, 0xDB, 0xF8,
0xC8, 0xA8, 0x2B, 0x40, 0xDC, 0xFE, 0x32, 0xA4,
0xCA, 0x10, 0x21, 0xF0, 0xD3, 0x5D, 0x0F, 0x00,
0x6F, 0x9D, 0x36, 0x42, 0x4A, 0x5E, 0xC1, 0xE0
},
/*  p1:   */
/*  dpMax      = 10.  lpMax 	 = 64.  cycleCnt=   2  0  0  1. 		*/
/* 28BDF76E31940AC5.1E2B4C376DA5F908.4C75169A0ED82B3F.B951C3DE647F208A. */
/* Karnaugh maps:
*  0011 1001 0010 0111. 1010 0111 0100 0110. 0011 0001 1111 0100. 1111 1000 0001 1100.
*  1100 1111 1111 1010. 0011 0011 1110 0100. 1001 0110 0100 0011. 0101 0110 1011 1011.
*  0010 0100 0011 0101. 1100 1000 1000 1110. 0111 1111 0010 0110. 0000 1010 0000 0011.
*  1101 1000 0010 0001. 0110 1001 1110 0101. 0001 0100 0101 0111. 0011 1011 1111 0010.
*/
{
0x75, 0xF3, 0xC6, 0xF4, 0xDB, 0x7B, 0xFB, 0xC8,
0x4A, 0xD3, 0xE6, 0x6B, 0x45, 0x7D, 0xE8, 0x4B,
0xD6, 0x32, 0xD8, 0xFD, 0x37, 0x71, 0xF1, 0xE1,
0x30, 0x0F, 0xF8, 0x1B, 0x87, 0xFA, 0x06, 0x3F,
0x5E, 0xBA, 0xAE, 0x5B, 0x8A, 0x00, 0xBC, 0x9D,
0x6D, 0xC1, 0xB1, 0x0E, 0x80, 0x5D, 0xD2, 0xD5,
0xA0, 0x84, 0x07, 0x14, 0xB5, 0x90, 0x2C, 0xA3,
0xB2, 0x73, 0x4C, 0x54, 0x92, 0x74, 0x36, 0x51,
0x38, 0xB0, 0xBD, 0x5A, 0xFC, 0x60, 0x62, 0x96,
0x6C, 0x42, 0xF7, 0x10, 0x7C, 0x28, 0x27, 0x8C,
0x13, 0x95, 0x9C, 0xC7, 0x24, 0x46, 0x3B, 0x70,
0xCA, 0xE3, 0x85, 0xCB, 0x11, 0xD0, 0x93, 0xB8,
0xA6, 0x83, 0x20, 0xFF, 0x9F, 0x77, 0xC3, 0xCC,
0x03, 0x6F, 0x08, 0xBF, 0x40, 0xE7, 0x2B, 0xE2,
0x79, 0x0C, 0xAA, 0x82, 0x41, 0x3A, 0xEA, 0xB9,
0xE4, 0x9A, 0xA4, 0x97, 0x7E, 0xDA, 0x7A, 0x17,
0x66, 0x94, 0xA1, 0x1D, 0x3D, 0xF0, 0xDE, 0xB3,
0x0B, 0x72, 0xA7, 0x1C, 0xEF, 0xD1, 0x53, 0x3E,
0x8F, 0x33, 0x26, 0x5F, 0xEC, 0x76, 0x2A, 0x49,
0x81, 0x88, 0xEE, 0x21, 0xC4, 0x1A, 0xEB, 0xD9,
0xC5, 0x39, 0x99, 0xCD, 0xAD, 0x31, 0x8B, 0x01,
0x18, 0x23, 0xDD, 0x1F, 0x4E, 0x2D, 0xF9, 0x48,
0x4F, 0xF2, 0x65, 0x8E, 0x78, 0x5C, 0x58, 0x19,
0x8D, 0xE5, 0x98, 0x57, 0x67, 0x7F, 0x05, 0x64,
0xAF, 0x63, 0xB6, 0xFE, 0xF5, 0xB7, 0x3C, 0xA5,
0xCE, 0xE9, 0x68, 0x44, 0xE0, 0x4D, 0x43, 0x69,
0x29, 0x2E, 0xAC, 0x15, 0x59, 0xA8, 0x0A, 0x9E,
0x6E, 0x47, 0xDF, 0x34, 0x35, 0x6A, 0xCF, 0xDC,
0x22, 0xC9, 0xC0, 0x9B, 0x89, 0xD4, 0xED, 0xAB,
0x12, 0xA2, 0x0D, 0x52, 0xBB, 0x02, 0x2F, 0xA9,
0xD7, 0x61, 0x1E, 0xB4, 0x50, 0x04, 0xF6, 0xC2,
0x16, 0x25, 0x86, 0x56, 0x55, 0x09, 0xBE, 0x91
}
};

#include	<memory.h>
#include	<assert.h>

#if   defined(min_key)  && !defined(MIN_KEY)
#define	MIN_KEY		1			/* toupper() */
#elif defined(part_key) && !defined(PART_KEY)
#define	PART_KEY	1
#elif defined(zero_key) && !defined(ZERO_KEY)
#define	ZERO_KEY	1
#endif

CONST int debugCompile = 0;
#define DebugDump(x,s,R,XOR,doRot,showT,needBswap)
#define	DebugDumpKey(key)

/*
+*****************************************************************************
*			Constants/Macros/Tables
-****************************************************************************/

#define		CONST					/* help syntax from C++, NOP here */

CONST		fullSbox MDStab;		/* not actually const.  Initialized ONE time */
int needToBuildMDS = 1;		/* is MDStab initialized yet? */

#define		BIG_TAB		0

#if BIG_TAB
uchar bigTab[4][256][256];	/* pre-computed S-box */
#endif

/* number of rounds for various key sizes:  128, 192, 256 */
/* (ignored for now in optimized code!) */
CONST int numRounds[4] =
{
0, ROUNDS_128, ROUNDS_192, ROUNDS_256
};

#if REENTRANT
#define		_sBox_	 key->sBox8x32
#else
static		fullSbox _sBox_;		/* permuted MDStab based on keys */
#endif
#define _sBox8_(N) (((uchar *) _sBox_) + (N)*256)

/*------- see what level of S-box precomputation we need to do -----*/
#if   defined(ZERO_KEY)
#define	MOD_STRING	"(Zero S-box keying)"
#define	Fe32_128(x,R)	\
	(	MDStab[0][p8(01)[p8(02)[_b(x,R  )]^b0(SKEY[1])]^b0(SKEY[0])] ^	\
		MDStab[1][p8(11)[p8(12)[_b(x,R+1)]^b1(SKEY[1])]^b1(SKEY[0])] ^	\
		MDStab[2][p8(21)[p8(22)[_b(x,R+2)]^b2(SKEY[1])]^b2(SKEY[0])] ^	\
		MDStab[3][p8(31)[p8(32)[_b(x,R+3)]^b3(SKEY[1])]^b3(SKEY[0])] )
#define	Fe32_192(x,R)	\
	(	MDStab[0][p8(01)[p8(02)[p8(03)[_b(x,R  )]^b0(SKEY[2])]^b0(SKEY[1])]^b0(SKEY[0])] ^ \
		MDStab[1][p8(11)[p8(12)[p8(13)[_b(x,R+1)]^b1(SKEY[2])]^b1(SKEY[1])]^b1(SKEY[0])] ^ \
		MDStab[2][p8(21)[p8(22)[p8(23)[_b(x,R+2)]^b2(SKEY[2])]^b2(SKEY[1])]^b2(SKEY[0])] ^ \
		MDStab[3][p8(31)[p8(32)[p8(33)[_b(x,R+3)]^b3(SKEY[2])]^b3(SKEY[1])]^b3(SKEY[0])] )
#define	Fe32_256(x,R)	\
	(	MDStab[0][p8(01)[p8(02)[p8(03)[p8(04)[_b(x,R  )]^b0(SKEY[3])]^b0(SKEY[2])]^b0(SKEY[1])]^b0(SKEY[0])] ^ \
		MDStab[1][p8(11)[p8(12)[p8(13)[p8(14)[_b(x,R+1)]^b1(SKEY[3])]^b1(SKEY[2])]^b1(SKEY[1])]^b1(SKEY[0])] ^ \
		MDStab[2][p8(21)[p8(22)[p8(23)[p8(24)[_b(x,R+2)]^b2(SKEY[3])]^b2(SKEY[2])]^b2(SKEY[1])]^b2(SKEY[0])] ^ \
		MDStab[3][p8(31)[p8(32)[p8(33)[p8(34)[_b(x,R+3)]^b3(SKEY[3])]^b3(SKEY[2])]^b3(SKEY[1])]^b3(SKEY[0])] )

#define	GetSboxKey	uint SKEY[4];	/* local copy */ \
					memcpy(SKEY,key->sboxKeys,sizeof(SKEY));
/*----------------------------------------------------------------*/
#elif defined(MIN_KEY)
#define	MOD_STRING	"(Minimal keying)"
#define	Fe32_(x,R)(MDStab[0][p8(01)[_sBox8_(0)[_b(x,R  )]] ^ b0(SKEY0)] ^ \
				   MDStab[1][p8(11)[_sBox8_(1)[_b(x,R+1)]] ^ b1(SKEY0)] ^ \
				   MDStab[2][p8(21)[_sBox8_(2)[_b(x,R+2)]] ^ b2(SKEY0)] ^ \
				   MDStab[3][p8(31)[_sBox8_(3)[_b(x,R+3)]] ^ b3(SKEY0)])
#define sbSet(N,i,J,v) { _sBox8_(N)[i+J] = v; }
#define	GetSboxKey	uint SKEY0	= key->sboxKeys[0]		/* local copy */
/*----------------------------------------------------------------*/
#elif defined(PART_KEY)
#define	MOD_STRING	"(Partial keying)"
#define	Fe32_(x,R)(MDStab[0][_sBox8_(0)[_b(x,R  )]] ^ \
				   MDStab[1][_sBox8_(1)[_b(x,R+1)]] ^ \
				   MDStab[2][_sBox8_(2)[_b(x,R+2)]] ^ \
				   MDStab[3][_sBox8_(3)[_b(x,R+3)]])
#define sbSet(N,i,J,v) { _sBox8_(N)[i+J] = v; }
#define	GetSboxKey
/*----------------------------------------------------------------*/
#else	/* default is FULL_KEY */
#ifndef FULL_KEY
#define	FULL_KEY	1
#endif
#if BIG_TAB
#define	TAB_STR		" (Big table)"
#else
#define	TAB_STR
#endif
#ifdef COMPILE_KEY
#define	MOD_STRING	"(Compiled subkeys)" TAB_STR
#else
#define	MOD_STRING	"(Full keying)" TAB_STR
#endif
/* Fe32_ does a full S-box + MDS lookup.  Need to #define _sBox_ before use.
   Note that we "interleave" 0,1, and 2,3 to avoid cache bank collisions
   in optimized assembly language.
*/
#define	Fe32_(x,R) (_sBox_[0][2*_b(x,R  )] ^ _sBox_[0][2*_b(x,R+1)+1] ^	\
					_sBox_[2][2*_b(x,R+2)] ^ _sBox_[2][2*_b(x,R+3)+1])
/* set a single S-box value, given the input byte */
#define sbSet(N,i,J,v) { _sBox_[N&2][2*i+(N&1)+2*J]=MDStab[N][v]; }
#define	GetSboxKey
#endif

CONST		char* moduleDescription = "Optimized C ";
CONST		char* modeString = MOD_STRING;


/* end of debug macros */

#ifdef GetCodeSize
extern uint Here( uint x );			/* return caller's address! */
uint TwofishCodeStart( void )
{
	return Here( 0 );
}
#endif

/*
+*****************************************************************************
*
* Function Name:	TableOp
*
* Function:			Handle table use checking
*
* Arguments:		op	=	what to do	(see TAB_* defns in AES.H)
*
* Return:			TRUE --> done (for TAB_QUERY)
*
* Notes: This routine is for use in generating the tables KAT file.
*		 For this optimized version, we don't actually track table usage,
*		 since it would make the macros incredibly ugly.  Instead we just
*		 run for a fixed number of queries and then say we're done.
*
-****************************************************************************/
int TableOp( int op )
{
	static int queryCnt = 0;

	switch ( op )
	{
	case TAB_DISABLE:
		break;
	case TAB_ENABLE:
		break;
	case TAB_RESET:
		queryCnt = 0;
		break;
	case TAB_QUERY:
		queryCnt++;
		if ( queryCnt < TAB_MIN_QUERY )
			return FALSE;
	}
	return TRUE;
}


/*
+*****************************************************************************
*
* Function Name:	ParseHexDword
*
* Function:			Parse ASCII hex nibbles and fill in key/iv dwords
*
* Arguments:		bit			=	# bits to read
*					srcTxt		=	ASCII source
*					d			=	ptr to dwords to fill in
*					dstTxt		=	where to make a copy of ASCII source
*									(NULL ok)
*
* Return:			Zero if no error.  Nonzero --> invalid hex or length
*
* Notes:  Note that the parameter d is a uint array, not a byte array.
*	This routine is coded to work both for little-endian and big-endian
*	architectures.  The character stream is interpreted as a LITTLE-ENDIAN
*	byte stream, since that is how the Pentium works, but the conversion
*	happens automatically below.
*
-****************************************************************************/
int ParseHexDword( int bits, CONST char* srcTxt, uint* d, char* dstTxt )
{
	int i;
	char c;
	uint b;

	union	/* make sure LittleEndian is defined correctly */
	{
		uchar b[4];
		uint d[1];
	} v;
	v.d[0] = 1;
	if ( v.b[0 ^ ADDR_XOR] != 1 )
		return BAD_ENDIAN;		/* make sure compile-time switch is set ok */

	for ( i = 0; i*32 < bits; i++ )
		d[i] = 0;					/* first, zero the field */

	for ( i = 0; i*4 < bits; i++ )		/* parse one nibble at a time */
	{
		/* case out the hexadecimal characters */
		c = srcTxt[i];
		if ( dstTxt )
			dstTxt[i] = c;
		if ( ( c >= '0' ) && ( c <= '9' ) )
			b = c - '0';
		else if ( ( c >= 'a' ) && ( c <= 'f' ) )
			b = c - 'a' + 10;
		else if ( ( c >= 'A' ) && ( c <= 'F' ) )
			b = c - 'A' + 10;
		else
			return BAD_KEY_MAT;	/* invalid hex character */
		/* works for big and little endian! */
		d[i / 8] |= b << ( 4 * ( ( i ^ 1 ) & 7 ) );
	}

	return 0;					/* no error */
}

/*
+*****************************************************************************
*
* Function Name:	RS_MDS_encode
*
* Function:			Use (12,8) Reed-Solomon code over GF(256) to produce
*					a key S-box dword from two key material dwords.
*
* Arguments:		k0	=	1st dword
*					k1	=	2nd dword
*
* Return:			Remainder polynomial generated using RS code
*
* Notes:
*	Since this computation is done only once per reKey per 64 bits of key,
*	the performance impact of this routine is imperceptible. The RS code
*	chosen has "simple" coefficients to allow smartcard/hardware implementation
*	without lookup tables.
*
-****************************************************************************/
uint RS_MDS_Encode( uint k0, uint k1 )
{
	int i, j;
	uint r;

	for ( i = r = 0; i < 2; i++ )
	{
		r ^= ( i ) ? k0 : k1;			/* merge in 32 more key bits */
		for ( j = 0; j < 4; j++ )			/* shift one byte at a time */
			RS_rem( r );
	}
	return r;
}


/*
+*****************************************************************************
*
* Function Name:	BuildMDS
*
* Function:			Initialize the MDStab array
*
* Arguments:		None.
*
* Return:			None.
*
* Notes:
*	Here we precompute all the fixed MDS table.  This only needs to be done
*	one time at initialization, after which the table is "CONST".
*
-****************************************************************************/
void BuildMDS( void )
{
	int i;
	uint d;
	uchar m1[2], mX[2], mY[4];

	for ( i = 0; i < 256; i++ )
	{
		m1[0] = P8x8[0][i];		/* compute all the matrix elements */
		mX[0] = ( uchar ) Mul_X( m1[0] );
		mY[0] = ( uchar ) Mul_Y( m1[0] );

		m1[1] = P8x8[1][i];
		mX[1] = ( uchar ) Mul_X( m1[1] );
		mY[1] = ( uchar ) Mul_Y( m1[1] );

#undef	Mul_1					/* change what the pre-processor does with Mij */
#undef	Mul_X
#undef	Mul_Y
#define	Mul_1	m1				/* It will now access m01[], m5B[], and mEF[] */
#define	Mul_X	mX
#define	Mul_Y	mY

#define	SetMDS(N)					\
		b0(d) = M0##N[P_##N##0];	\
		b1(d) = M1##N[P_##N##0];	\
		b2(d) = M2##N[P_##N##0];	\
		b3(d) = M3##N[P_##N##0];	\
		MDStab[N][i] = d;

		SetMDS( 0 );				/* fill in the matrix with elements computed above */
		SetMDS( 1 );
		SetMDS( 2 );
		SetMDS( 3 );
	}
#undef	Mul_1
#undef	Mul_X
#undef	Mul_Y
#define	Mul_1	Mx_1			/* re-enable true multiply */
#define	Mul_X	Mx_X
#define	Mul_Y	Mx_Y

#if BIG_TAB
	{
		int j, k;
		uchar* q0,* q1;

		for ( i = 0; i < 4; i++ )
		{
			switch ( i )
			{
			case 0:
				q0 = p8( 01 ); q1 = p8( 02 );	break;
			case 1:
				q0 = p8( 11 ); q1 = p8( 12 );	break;
			case 2:
				q0 = p8( 21 ); q1 = p8( 22 );	break;
			case 3:
				q0 = p8( 31 ); q1 = p8( 32 );	break;
			}
			for ( j = 0; j < 256; j++ )
				for ( k = 0; k < 256; k++ )
					bigTab[i][j][k] = q0[q1[k] ^ j];
		}
	}
#endif

	needToBuildMDS = 0;			/* NEVER modify the table again! */
}

/*
+*****************************************************************************
*
* Function Name:	ReverseRoundSubkeys
*
* Function:			Reverse order of round subkeys to switch between encrypt/decrypt
*
* Arguments:		key		=	ptr to keyInstance to be reversed
*					newDir	=	new direction value
*
* Return:			None.
*
* Notes:
*	This optimization allows both blockEncrypt and blockDecrypt to use the same
*	"fallthru" switch statement based on the number of rounds.
*	Note that key->numRounds must be even and >= 2 here.
*
-****************************************************************************/
void ReverseRoundSubkeys( keyInstance* key, uchar newDir )
{
	uint t0, t1;
	register uint * r0 = key->subKeys + ROUND_SUBKEYS;
	register uint * r1 = r0 + 2 * key->numRounds - 2;

	for ( ; r0 < r1; r0 += 2,r1 -= 2 )
	{
		t0 = r0[0];			/* swap the order */
		t1 = r0[1];
		r0[0] = r1[0];		/* but keep relative order within pairs */
		r0[1] = r1[1];
		r1[0] = t0;
		r1[1] = t1;
	}

	key->direction = newDir;
}

/*
+*****************************************************************************
*
* Function Name:	Xor256
*
* Function:			Copy an 8-bit permutation (256 bytes), xoring with a byte
*
* Arguments:		dst		=	where to put result
*					src		=	where to get data (can be same asa dst)
*					b		=	byte to xor
*
* Return:			None
*
* Notes:
* 	BorlandC's optimization is terrible!  When we put the code inline,
*	it generates fairly good code in the *following* segment (not in the Xor256
*	code itself).  If the call is made, the code following the call is awful!
*	The penalty is nearly 50%!  So we take the code size hit for inlining for
*	Borland, while Microsoft happily works with a call.
*
-****************************************************************************/
#if defined(__BORLANDC__)	/* do it inline */
#define Xor32(dst,src,i) { ((uint *)dst)[i] = ((uint *)src)[i] ^ tmpX; }
#define	Xor256(dst,src,b)				\
	{									\
	register uint tmpX=0x01010101u * b;\
	for (i=0;i<64;i+=4)					\
		{ Xor32(dst,src,i  ); Xor32(dst,src,i+1); Xor32(dst,src,i+2); Xor32(dst,src,i+3); }	\
	}
#else						/* do it as a function call */
void Xor256( void* dst, void* src, uchar b )
{
	register uint	x = b * 0x01010101u;	/* replicate byte to all four bytes */
	register uint * d = ( uint * ) dst;
	register uint * s = ( uint * ) src;
#define X_8(N)	{ d[N]=s[N] ^ x; d[N+1]=s[N+1] ^ x; }
#define X_32(N)	{ X_8(N); X_8(N+2); X_8(N+4); X_8(N+6); }
	X_32( 0 ); X_32( 8 ); X_32( 16 ); X_32( 24 );	/* all inline */
	d += 32;	/* keep offsets small! */
	s += 32;
	X_32( 0 ); X_32( 8 ); X_32( 16 ); X_32( 24 );	/* all inline */
}
#endif

/*
+*****************************************************************************
*
* Function Name:	reKey
*
* Function:			Initialize the Twofish key schedule from key32
*
* Arguments:		key			=	ptr to keyInstance to be initialized
*
* Return:			TRUE on success
*
* Notes:
*	Here we precompute all the round subkeys, although that is not actually
*	required.  For example, on a smartcard, the round subkeys can
*	be generated on-the-fly	using f32()
*
-****************************************************************************/
int reKey( keyInstance* key )
{
	int i, j, k64Cnt, keyLen;
	int subkeyCnt;
	uint A = 0, B = 0, q;
	uint sKey[MAX_KEY_BITS / 64], k32e[MAX_KEY_BITS / 64], k32o[MAX_KEY_BITS / 64];
	uchar L0[256], L1[256];	/* small local 8-bit permutations */

	if ( needToBuildMDS )			/* do this one time only */
		BuildMDS();

#define	F32(res,x,k32)	\
	{															\
	uint t=x;													\
	switch (k64Cnt & 3)											\
		{														\
		case 0:  /* same as 4 */								\
					b0(t)   = p8(04)[b0(t)] ^ b0(k32[3]);		\
					b1(t)   = p8(14)[b1(t)] ^ b1(k32[3]);		\
					b2(t)   = p8(24)[b2(t)] ^ b2(k32[3]);		\
					b3(t)   = p8(34)[b3(t)] ^ b3(k32[3]);		\
				 /* fall thru, having pre-processed t */		\
		case 3:		b0(t)   = p8(03)[b0(t)] ^ b0(k32[2]);		\
					b1(t)   = p8(13)[b1(t)] ^ b1(k32[2]);		\
					b2(t)   = p8(23)[b2(t)] ^ b2(k32[2]);		\
					b3(t)   = p8(33)[b3(t)] ^ b3(k32[2]);		\
				 /* fall thru, having pre-processed t */		\
		case 2:	 /* 128-bit keys (optimize for this case) */	\
			res=	MDStab[0][p8(01)[p8(02)[b0(t)] ^ b0(k32[1])] ^ b0(k32[0])] ^	\
					MDStab[1][p8(11)[p8(12)[b1(t)] ^ b1(k32[1])] ^ b1(k32[0])] ^	\
					MDStab[2][p8(21)[p8(22)[b2(t)] ^ b2(k32[1])] ^ b2(k32[0])] ^	\
					MDStab[3][p8(31)[p8(32)[b3(t)] ^ b3(k32[1])] ^ b3(k32[0])] ;	\
		}														\
	}

	{
		subkeyCnt = ROUND_SUBKEYS + 2 * key->numRounds;
		keyLen = key->keyLen;
		k64Cnt = ( keyLen + 63 ) / 64;			/* number of 64-bit key words */
		for ( i = 0,j = k64Cnt - 1; i < k64Cnt; i++,j-- )
		{
			/* split into even/odd key dwords */
			k32e[i] = key->key32[2 * i];
			k32o[i] = key->key32[2 * i + 1];
			/* compute S-box keys using (12,8) Reed-Solomon code over GF(256) */
			sKey[j] = key->sboxKeys[j] = RS_MDS_Encode( k32e[i], k32o[i] );	/* reverse order */
		}
	}

	{
		for ( i = q = 0; i < subkeyCnt / 2; i++,q += SK_STEP )
		{
			/* compute round subkeys for PHT */
			F32( A, q, k32e );		/* A uses even key dwords */
			F32( B, q + SK_BUMP, k32o );		/* B uses odd  key dwords */
			B = ROL( B, 8 );
			key->subKeys[2 * i] = A + B;	/* combine with a PHT */
			B = A + 2 * B;
			key->subKeys[2 * i + 1] = ROL( B, SK_ROTL );
		}
#if !defined(ZERO_KEY)
		switch ( keyLen )	/* case out key length for speed in generating S-boxes */
		{
		case 128:
#if defined(FULL_KEY) || defined(PART_KEY)
#if BIG_TAB
#define	one128(N,J)	sbSet(N,i,J,L0[i+J])
#define	sb128(N) {						\
			uchar *qq=bigTab[N][b##N(sKey[1])];	\
			Xor256(L0,qq,b##N(sKey[0]));		\
			for (i=0;i<256;i+=2) { one128(N,0); one128(N,1); } }
#else
#define	one128(N,J)	sbSet(N,i,J,p8(N##1)[L0[i+J]]^k0)
#define	sb128(N) {					\
			Xor256(L0,p8(N##2),b##N(sKey[1]));	\
			{ register uint k0=b##N(sKey[0]);	\
			for (i=0;i<256;i+=2) { one128(N,0); one128(N,1); } } }
#endif
#elif defined(MIN_KEY)
#define	sb128(N) Xor256(_sBox8_(N),p8(N##2),b##N(sKey[1]))
#endif
			sb128( 0 ); sb128( 1 ); sb128( 2 ); sb128( 3 );
			break;
		case 192:
#if defined(FULL_KEY) || defined(PART_KEY)
#define one192(N,J) sbSet(N,i,J,p8(N##1)[p8(N##2)[L0[i+J]]^k1]^k0)
#define	sb192(N) {						\
			Xor256(L0,p8(N##3),b##N(sKey[2]));	\
			{ register uint k0=b##N(sKey[0]);	\
			  register uint k1=b##N(sKey[1]);	\
			  for (i=0;i<256;i+=2) { one192(N,0); one192(N,1); } } }
#elif defined(MIN_KEY)
#define one192(N,J) sbSet(N,i,J,p8(N##2)[L0[i+J]]^k1)
#define	sb192(N) {						\
			Xor256(L0,p8(N##3),b##N(sKey[2]));	\
			{ register uint k1=b##N(sKey[1]);	\
			  for (i=0;i<256;i+=2) { one192(N,0); one192(N,1); } } }
#endif
			sb192( 0 ); sb192( 1 ); sb192( 2 ); sb192( 3 );
			break;
		case 256:
#if defined(FULL_KEY) || defined(PART_KEY)
#define one256(N,J) sbSet(N,i,J,p8(N##1)[p8(N##2)[L0[i+J]]^k1]^k0)
#define	sb256(N) {										\
			Xor256(L1,p8(N##4),b##N(sKey[3]));					\
			for (i=0;i<256;i+=2) {L0[i  ]=p8(N##3)[L1[i]];		\
								  L0[i+1]=p8(N##3)[L1[i+1]]; }	\
			Xor256(L0,L0,b##N(sKey[2]));						\
			{ register uint k0=b##N(sKey[0]);					\
			  register uint k1=b##N(sKey[1]);					\
			  for (i=0;i<256;i+=2) { one256(N,0); one256(N,1); } } }
#elif defined(MIN_KEY)
#define one256(N,J) sbSet(N,i,J,p8(N##2)[L0[i+J]]^k1)
#define	sb256(N) {										\
			Xor256(L1,p8(N##4),b##N(sKey[3]));					\
			for (i=0;i<256;i+=2) {L0[i  ]=p8(N##3)[L1[i]];		\
								  L0[i+1]=p8(N##3)[L1[i+1]]; }	\
			Xor256(L0,L0,b##N(sKey[2]));						\
			{ register uint k1=b##N(sKey[1]);					\
			  for (i=0;i<256;i+=2) { one256(N,0); one256(N,1); } } }
#endif
			sb256( 0 ); sb256( 1 );	sb256( 2 ); sb256( 3 );
			break;
		}
#endif
	}

	DebugDumpKey( key );

	if ( key->direction == DIR_ENCRYPT )
		ReverseRoundSubkeys( key, DIR_ENCRYPT );	/* reverse the round subkey order */

	return TRUE;
}
/*
+*****************************************************************************
*
* Function Name:	makeKey
*
* Function:			Initialize the Twofish key schedule
*
* Arguments:		key			=	ptr to keyInstance to be initialized
*					direction	=	DIR_ENCRYPT or DIR_DECRYPT
*					keyLen		=	# bits of key text at *keyMaterial
*					keyMaterial	=	ptr to hex ASCII chars representing key bits
*
* Return:			TRUE on success
*					else error code (e.g., BAD_KEY_DIR)
*
* Notes:	This parses the key bits from keyMaterial.  Zeroes out unused key bits
*
-****************************************************************************/
int makeKey( keyInstance* key, uchar direction, int keyLen, CONST char* keyMaterial )
{
	key->direction = direction;/* set our cipher direction */
	key->keyLen = ( keyLen + 63 ) & ~63;		/* round up to multiple of 64 */
	key->numRounds = numRounds[( keyLen - 1 ) / 64];
	memset( key->key32, 0, sizeof( key->key32 ) );	/* zero unused bits */
	key->keyMaterial[MAX_KEY_SIZE] = 0;	/* terminate ASCII string */

	if ( ( keyMaterial == NULL ) || ( keyMaterial[0] == 0 ) )
		return TRUE;			/* allow a "dummy" call */

	if ( ParseHexDword( keyLen, keyMaterial, key->key32, key->keyMaterial ) )
		return BAD_KEY_MAT;

	return reKey( key );			/* generate round subkeys */
}


/*
+*****************************************************************************
*
* Function Name:	cipherInit
*
* Function:			Initialize the Twofish cipher in a given mode
*
* Arguments:		cipher		=	ptr to cipherInstance to be initialized
*					mode		=	MODE_ECB, MODE_CBC, or MODE_CFB1
*					IV			=	ptr to hex ASCII test representing IV bytes
*
* Return:			TRUE on success
*					else error code (e.g., BAD_CIPHER_MODE)
*
-****************************************************************************/
int cipherInit( cipherInstance* cipher, uchar mode, CONST char* IV )
{
	int i;
	if ( ( mode != MODE_ECB ) && ( IV ) )	/* parse the IV */
	{
		if ( ParseHexDword( BLOCK_SIZE, IV, cipher->iv32, NULL ) )
			return BAD_IV_MAT;
		for ( i = 0; i < BLOCK_SIZE / 32; i++ )	/* make byte-oriented copy for CFB1 */
			( ( uint * ) cipher->IV )[i] = Bswap( cipher->iv32[i] );
	}

	cipher->mode = mode;

	return TRUE;
}

/*
+*****************************************************************************
*
* Function Name:	blockEncrypt
*
* Function:			Encrypt block(s) of data using Twofish
*
* Arguments:		cipher		=	ptr to already initialized cipherInstance
*					key			=	ptr to already initialized keyInstance
*					input		=	ptr to data blocks to be encrypted
*					inputLen	=	# bits to encrypt (multiple of blockSize)
*					outBuffer	=	ptr to where to put encrypted blocks
*
* Return:			# bits ciphered (>= 0)
*					else error code (e.g., BAD_CIPHER_STATE, BAD_KEY_MATERIAL)
*
* Notes: The only supported block size for ECB/CBC modes is BLOCK_SIZE bits.
*		 If inputLen is not a multiple of BLOCK_SIZE bits in those modes,
*		 an error BAD_INPUT_LEN is returned.  In CFB1 mode, all block
*		 sizes can be supported.
*
-****************************************************************************/
int blockEncrypt( cipherInstance* cipher, keyInstance* key, CONST uchar* input, int inputLen, uchar* outBuffer )
{
	int i, n;						/* loop counters */
	uint x[BLOCK_SIZE / 32];			/* block being encrypted */
	uint t0, t1;					/* temp variables */
	int rounds = key->numRounds;	/* number of rounds */
	uchar bit, bit0, ctBit, carry;		/* temps for CFB */

	/* make local copies of things for faster access */
	int mode = cipher->mode;
	uint sk[TOTAL_SUBKEYS];
	uint IV[BLOCK_SIZE / 32];

	GetSboxKey;

	if ( mode == MODE_CFB1 )
	{
		/* use recursion here to handle CFB, one block at a time */
		cipher->mode = MODE_ECB;	/* do encryption in ECB */
		for ( n = 0; n < inputLen; n++ )
		{
			blockEncrypt( cipher, key, cipher->IV, BLOCK_SIZE, ( uchar * ) x );
			bit0 = 0x80 >> ( n & 7 );/* which bit position in byte */
			ctBit = ( input[n / 8] & bit0 ) ^ ( ( ( ( uchar * ) x )[0] & 0x80 ) >> ( n & 7 ) );
			outBuffer[n / 8] = ( outBuffer[n / 8] & ~bit0 ) | ctBit;
			carry = ctBit >> ( 7 - ( n & 7 ) );
			for ( i = BLOCK_SIZE / 8 - 1; i >= 0; i-- )
			{
				bit = cipher->IV[i] >> 7;	/* save next "carry" from shift */
				cipher->IV[i] = ( cipher->IV[i] << 1 ) ^ carry;
				carry = bit;
			}
		}
		cipher->mode = MODE_CFB1;	/* restore mode for next time */
		return inputLen;
	}

	/* here for ECB, CBC modes */
	if ( key->direction != DIR_ENCRYPT )
		ReverseRoundSubkeys( key, DIR_ENCRYPT );	/* reverse the round subkey order */

	/* make local copy of subkeys for speed */
	memcpy( sk, key->subKeys, sizeof( uint ) * ( ROUND_SUBKEYS + 2 * rounds ) );
	if ( mode == MODE_CBC )
		BlockCopy( IV, cipher->iv32 )
	else
		IV[0] = IV[1] = IV[2] = IV[3] = 0;

	for ( n = 0; n < inputLen; n += BLOCK_SIZE,input += BLOCK_SIZE / 8,outBuffer += BLOCK_SIZE / 8 )
	{
#ifdef DEBUG
		DebugDump( input, "\n", -1, 0, 0, 0, 1 );
		if ( cipher->mode == MODE_CBC )
			DebugDump( cipher->iv32, "", IV_ROUND, 0, 0, 0, 0 );
#endif
#define	LoadBlockE(N)  x[N]=Bswap(((uint *)input)[N]) ^ sk[INPUT_WHITEN+N] ^ IV[N]
		LoadBlockE( 0 );	LoadBlockE( 1 );	LoadBlockE( 2 );	LoadBlockE( 3 );
		DebugDump( x, "", 0, 0, 0, 0, 0 );
#define	EncryptRound(K,R,id)	\
			t0	   = Fe32##id(x[K  ],0);					\
			t1	   = Fe32##id(x[K^1],3);					\
			x[K^3] = ROL(x[K^3],1);							\
			x[K^2]^= t0 +   t1 + sk[ROUND_SUBKEYS+2*(R)  ];	\
			x[K^3]^= t0 + 2*t1 + sk[ROUND_SUBKEYS+2*(R)+1];	\
			x[K^2] = ROR(x[K^2],1);							\
			DebugDump(x,"",rounds-(R),0,0,1,0);
#define		Encrypt2(R,id)	{ EncryptRound(0,R+1,id); EncryptRound(2,R,id); }

#if defined(ZERO_KEY)
		switch ( key->keyLen )
		{
		case 128:
			for ( i = rounds - 2; i >= 0; i -= 2 )
				Encrypt2( i, _128 );
			break;
		case 192:
			for ( i = rounds - 2; i >= 0; i -= 2 )
				Encrypt2( i, _192 );
			break;
		case 256:
			for ( i = rounds - 2; i >= 0; i -= 2 )
				Encrypt2( i, _256 );
			break;
		}
#else
		Encrypt2( 14, _ );
		Encrypt2( 12, _ );
		Encrypt2( 10, _ );
		Encrypt2( 8, _ );
		Encrypt2( 6, _ );
		Encrypt2( 4, _ );
		Encrypt2( 2, _ );
		Encrypt2( 0, _ );
#endif

		/* need to do (or undo, depending on your point of view) final swap */
#if LittleEndian
#define	StoreBlockE(N)	((uint *)outBuffer)[N]=x[N^2] ^ sk[OUTPUT_WHITEN+N]
#else
#define	StoreBlockE(N)	{ t0=x[N^2] ^ sk[OUTPUT_WHITEN+N]; ((uint *)outBuffer)[N]=Bswap(t0); }
#endif
		StoreBlockE( 0 );	StoreBlockE( 1 );	StoreBlockE( 2 );	StoreBlockE( 3 );
		if ( mode == MODE_CBC )
		{
			IV[0] = Bswap( ( ( uint * ) outBuffer )[0] );
			IV[1] = Bswap( ( ( uint * ) outBuffer )[1] );
			IV[2] = Bswap( ( ( uint * ) outBuffer )[2] );
			IV[3] = Bswap( ( ( uint * ) outBuffer )[3] );
		}
#ifdef DEBUG
		DebugDump( outBuffer, "", rounds + 1, 0, 0, 0, 1 );
		if ( cipher->mode == MODE_CBC )
			DebugDump( cipher->iv32, "", IV_ROUND, 0, 0, 0, 0 );
#endif
	}

	if ( mode == MODE_CBC )
		BlockCopy( cipher->iv32, IV );

	return inputLen;
}

/*
+*****************************************************************************
*
* Function Name:	blockDecrypt
*
* Function:			Decrypt block(s) of data using Twofish
*
* Arguments:		cipher		=	ptr to already initialized cipherInstance
*					key			=	ptr to already initialized keyInstance
*					input		=	ptr to data blocks to be decrypted
*					inputLen	=	# bits to encrypt (multiple of blockSize)
*					outBuffer	=	ptr to where to put decrypted blocks
*
* Return:			# bits ciphered (>= 0)
*					else error code (e.g., BAD_CIPHER_STATE, BAD_KEY_MATERIAL)
*
* Notes: The only supported block size for ECB/CBC modes is BLOCK_SIZE bits.
*		 If inputLen is not a multiple of BLOCK_SIZE bits in those modes,
*		 an error BAD_INPUT_LEN is returned.  In CFB1 mode, all block
*		 sizes can be supported.
*
-****************************************************************************/
int blockDecrypt( cipherInstance* cipher, keyInstance* key, CONST uchar* input, int inputLen, uchar* outBuffer )
{
	int i, n;						/* loop counters */
	uint x[BLOCK_SIZE / 32];			/* block being encrypted */
	uint t0, t1;					/* temp variables */
	int rounds = key->numRounds;	/* number of rounds */
	uchar bit, bit0, ctBit, carry;		/* temps for CFB */

	/* make local copies of things for faster access */
	int mode = cipher->mode;
	uint sk[TOTAL_SUBKEYS];
	uint IV[BLOCK_SIZE / 32];

	GetSboxKey;

	if ( cipher->mode == MODE_CFB1 )
	{
		/* use blockEncrypt here to handle CFB, one block at a time */
		cipher->mode = MODE_ECB;	/* do encryption in ECB */
		for ( n = 0; n < inputLen; n++ )
		{
			blockEncrypt( cipher, key, cipher->IV, BLOCK_SIZE, ( uchar * ) x );
			bit0 = 0x80 >> ( n & 7 );
			ctBit = input[n / 8] & bit0;
			outBuffer[n / 8] = ( outBuffer[n / 8] & ~bit0 ) | ( ctBit ^ ( ( ( ( uchar * ) x )[0] & 0x80 ) >> ( n & 7 ) ) );
			carry = ctBit >> ( 7 - ( n & 7 ) );
			for ( i = BLOCK_SIZE / 8 - 1; i >= 0; i-- )
			{
				bit = cipher->IV[i] >> 7;	/* save next "carry" from shift */
				cipher->IV[i] = ( cipher->IV[i] << 1 ) ^ carry;
				carry = bit;
			}
		}
		cipher->mode = MODE_CFB1;	/* restore mode for next time */
		return inputLen;
	}

	/* here for ECB, CBC modes */
	if ( key->direction != DIR_DECRYPT )
		ReverseRoundSubkeys( key, DIR_DECRYPT );	/* reverse the round subkey order */
	/* make local copy of subkeys for speed */
	memcpy( sk, key->subKeys, sizeof( uint ) * ( ROUND_SUBKEYS + 2 * rounds ) );
	if ( mode == MODE_CBC )
		BlockCopy( IV, cipher->iv32 )
	else
		IV[0] = IV[1] = IV[2] = IV[3] = 0;

	for ( n = 0; n < inputLen; n += BLOCK_SIZE,input += BLOCK_SIZE / 8,outBuffer += BLOCK_SIZE / 8 )
	{
		DebugDump( input, "\n", rounds + 1, 0, 0, 0, 1 );
#define LoadBlockD(N) x[N^2]=Bswap(((uint *)input)[N]) ^ sk[OUTPUT_WHITEN+N]
		LoadBlockD( 0 );	LoadBlockD( 1 );	LoadBlockD( 2 );	LoadBlockD( 3 );

#define	DecryptRound(K,R,id)								\
			t0	   = Fe32##id(x[K  ],0);					\
			t1	   = Fe32##id(x[K^1],3);					\
			DebugDump(x,"",(R)+1,0,0,1,0);					\
			x[K^2] = ROL (x[K^2],1);						\
			x[K^2]^= t0 +   t1 + sk[ROUND_SUBKEYS+2*(R)  ];	\
			x[K^3]^= t0 + 2*t1 + sk[ROUND_SUBKEYS+2*(R)+1];	\
			x[K^3] = ROR (x[K^3],1);						\

#define		Decrypt2(R,id)	{ DecryptRound(2,R+1,id); DecryptRound(0,R,id); }

#if defined(ZERO_KEY)
		switch ( key->keyLen )
		{
		case 128:
			for ( i = rounds - 2; i >= 0; i -= 2 )
				Decrypt2( i, _128 );
			break;
		case 192:
			for ( i = rounds - 2; i >= 0; i -= 2 )
				Decrypt2( i, _192 );
			break;
		case 256:
			for ( i = rounds - 2; i >= 0; i -= 2 )
				Decrypt2( i, _256 );
			break;
		}
#else
		{
			Decrypt2( 14, _ );
			Decrypt2( 12, _ );
			Decrypt2( 10, _ );
			Decrypt2( 8, _ );
			Decrypt2( 6, _ );
			Decrypt2( 4, _ );
			Decrypt2( 2, _ );
			Decrypt2( 0, _ );
		}
#endif
		DebugDump( x, "", 0, 0, 0, 0, 0 );
		if ( cipher->mode == MODE_ECB )
		{
#if LittleEndian
#define	StoreBlockD(N)	((uint *)outBuffer)[N] = x[N] ^ sk[INPUT_WHITEN+N]
#else
#define	StoreBlockD(N)	{ t0=x[N]^sk[INPUT_WHITEN+N]; ((uint *)outBuffer)[N] = Bswap(t0); }
#endif
			StoreBlockD( 0 );	StoreBlockD( 1 );	StoreBlockD( 2 );	StoreBlockD( 3 );
#undef  StoreBlockD
			DebugDump( outBuffer, "", -1, 0, 0, 0, 1 );
			continue;
		}
		else
		{
#define	StoreBlockD(N)	x[N]   ^= sk[INPUT_WHITEN+N] ^ IV[N];	\
						IV[N]   = Bswap(((uint *)input)[N]);	\
						((uint *)outBuffer)[N] = Bswap(x[N]);
			StoreBlockD( 0 );	StoreBlockD( 1 );	StoreBlockD( 2 );	StoreBlockD( 3 );
#undef  StoreBlockD
			DebugDump( outBuffer, "", -1, 0, 0, 0, 1 );
		}
	}
	if ( mode == MODE_CBC )	/* restore iv32 to cipher */
		BlockCopy( cipher->iv32, IV )

	return inputLen;
}

}

Twofish::Twofish(unsigned int seed)
: mRecvPos(0),
  mSendPos(0) {
        Twofish2::makeKey(&mKey,DIR_ENCRYPT,128,NULL);
        Twofish2::cipherInit(&mCipher,MODE_ECB,NULL);

        // Is seed in correct byte order? Needs LE BE for sphere
        memcpy(&(mKey.key32),&seed,4);
        //qToLittleEndian(seed,&(mKey.key32));
        Twofish2::reKey(&mKey);
        for(unsigned short i = 0; i < 256; ++i )
                mCipherTable[i] = i;

        initializeCipherTable();
        initializeXORData();
}

void Twofish::initializeCipherTable() {
        char buffer[256];
        // TODO check twofish code for the need of an extra buffer
        Twofish2::blockEncrypt(&mCipher,&mKey,reinterpret_cast<unsigned char*>(mCipherTable),0x800,reinterpret_cast<unsigned char*>(buffer));
        memcpy(mCipherTable,buffer,256);
        mRecvPos = 0;
}

void Twofish::initializeXORData() {
        QByteArray cipher(mCipherTable,256);
        QByteArray xorData = QCryptographicHash::hash(cipher, QCryptographicHash::Md5);
        memcpy(mXORData,xorData.data(),16);
}

QByteArray Twofish::in(QByteArray& data) {
        QByteArray unencrypted;
        char* pos = data.begin();
        for(const char* end = pos + data.length(); pos != end; pos++, mRecvPos++) {
                if(mRecvPos==256)
                        initializeCipherTable();
                unencrypted.append( *pos ^ mCipherTable[mRecvPos]);
        }
        data.clear();
        return unencrypted;
}

QByteArray Twofish::out(QByteArray& data) {
        QByteArray unencrypted;
        char* pos = data.begin();
        for(const char* end = pos + data.length(); pos != end; pos++, mSendPos++, mSendPos&=0x0F)
                unencrypted.append( *pos ^ mXORData[mSendPos]);
        data.clear();
        return unencrypted;
}

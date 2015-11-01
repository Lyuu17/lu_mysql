/*
	SQFuncs.c
	This file is part of lu_mysql.

	--------------------------------------------------

	Copyright (c) 2011, Juppi/Liberty Unleashed
	All rights reserved.

	Redistribution and use in source and binary forms, with or without
	modification, are permitted provided that the following conditions are met:
		* Redistributions of source code must retain the above copyright
		  notice, this list of conditions and the following disclaimer.
		* Redistributions in binary form must reproduce the above copyright
		  notice, this list of conditions and the following disclaimer in the
		  documentation and/or other materials provided with the distribution.
		* Neither the name of lu_mysql nor the names of its contributors may
		  be used to endorse or promote products derived from this software
		  without specific prior written permission.

	THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
	ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
	WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
	DISCLAIMED. IN NO EVENT SHALL JUPPI/LIBERTY UNLEASHED BE LIABLE FOR ANY
	DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
	(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
	LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
	ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
	(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
	SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "SQMain.h"
#include "SQFuncs.h"
#include <stdio.h>

SQAPI sq;
HSQUIRRELVM v;

LU_EXPORT SQRESULT SQLoad( HSQUIRRELVM vm, SQAPI api )
{
	sq = api;
	v = vm;
	
	printf( "[MySQL] Juppi's lu_mysql module initializing... " );
	RegisterFuncs( vm );
	printf( "loaded.\n" );

	return SQ_OK;
}

LU_EXPORT SQRESULT SQUnload( void )
{
	return SQ_OK;
}

LU_EXPORT SQRESULT SQCallback( unsigned int uiCallback, void* pData )
{
	return SQ_OK;
}

LU_EXPORT SQRESULT SQPulse( void )
{
	return SQ_OK;
}

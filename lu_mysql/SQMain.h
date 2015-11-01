//
// SQMain: This is where the module is loaded/unloaded and initialised.
//
//	Written for Liberty Unleashed by the Liberty Unleashed Team.
//

#ifndef _SQMAIN_H
#define _SQMAIN_H

#include <string.h>
#include "SQModule.h"

#ifdef _WIN32
	#define LU_EXPORT __declspec( dllexport )
	#include <windows.h>
#else
	#define LU_EXPORT
#endif

#ifdef __cplusplus
extern "C" {
#endif

	LU_EXPORT SQRESULT			SQLoad				( HSQUIRRELVM v, SQAPI api );
	LU_EXPORT SQRESULT			SQUnload			( void );
	LU_EXPORT SQRESULT 			SQCallback			( unsigned int uiCallback, void* pData );
	LU_EXPORT SQRESULT			SQPulse				( void );

#ifdef __cplusplus
}
#endif

#endif

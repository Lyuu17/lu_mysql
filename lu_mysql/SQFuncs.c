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

#include "SQFuncs.h"
#include "mysql.h"

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

extern SQAPI sq;
extern HSQUIRRELVM v;

_SQUIRRELDEF( SQ_mysql_connect )
{
	const SQChar* szServer = NULL;
	const SQChar* szUser = NULL;
	const SQChar* szPass = NULL;
	const SQChar* szDatabase = NULL;
	SQInteger iPort = 0;
	MYSQL* pConn = NULL;

	if ( sq->gettype( v, 2 ) == OT_STRING &&
		sq->gettype( v, 3 ) == OT_STRING && 
		sq->gettype( v, 4 ) == OT_STRING &&
		sq->gettype( v, 5 ) == OT_STRING )
	{
		sq->getstring( v, 2, &szServer );
		sq->getstring( v, 3, &szUser );
		sq->getstring( v, 4, &szPass );
		sq->getstring( v, 5, &szDatabase );

		if ( sq->gettype( v, 6 ) == OT_INTEGER ) sq->getinteger( v, 6, &iPort );

		if ( *szServer && *szUser && *szPass && *szDatabase )
		{
			pConn = mysql_init( NULL );

			if ( !pConn )
			{
				sq->pushnull( v );
				return sq->throwerror( v, _SC( "Error in 'mysql_connect': Could not initialize." ) );
			}

			if ( !mysql_real_connect( pConn, szServer, szUser, szPass, szDatabase, (unsigned int)iPort, NULL, 0 ) )
			{
				mysql_close( pConn );

				sq->pushnull( v );
				return sq->throwerror( v, _SC( "Error in 'mysql_connect': Connection failed. Make sure connection details are correct." ) );
			}
			sq->pushuserpointer( v, (SQUserPointer)pConn );
			return 1;
		}
	}

	sq->pushnull( v );
	return sq->throwerror( v, _SC( "Error in 'mysql_connect': Wrong number of arguments or invalid argument type" ) );
}

_SQUIRRELDEF( SQ_mysql_close )
{
	SQUserPointer pPointer = NULL;
	MYSQL* pConn = NULL;

	if ( sq->gettype( v, 2 ) == OT_USERPOINTER )
	{
		sq->getuserpointer( v, 2, &pPointer );

		if ( pPointer )
		{
			pConn = (MYSQL*)pPointer;

			mysql_close( pConn );

			sq->pushbool( v, SQTrue );
			return 1;
		}
	}

	return sq->throwerror( v, _SC( "Error in 'mysql_close': Wrong number of arguments or invalid argument type" ) );
}

_SQUIRRELDEF( SQ_mysql_query )
{
	SQUserPointer pPointer = NULL;
	const SQChar* szQuery = NULL;
	MYSQL* pConn = NULL;
	MYSQL_RES* pResult = NULL;

	if ( sq->gettype( v, 2 ) == OT_USERPOINTER &&
		sq->gettype( v, 3 ) == OT_STRING )
	{
		sq->getuserpointer( v, 2, &pPointer );
		sq->getstring( v, 3, &szQuery );

		if ( pPointer && szQuery )
		{
			if ( !*szQuery )
			{
				sq->pushbool( v, SQFalse );
				return 1;
			}

			pConn = (MYSQL*)pPointer;

			if ( mysql_query( pConn, szQuery ) )
			{
				// Query failed (check mysql_error or mysql_errno)
				sq->pushbool( v, SQFalse );
				return 1;
			}

			pResult = mysql_store_result( pConn );

			if ( !pResult )
			{
				// The statement didn't return a result set or reading of the result set failed
				sq->pushnull( v );
				return 1;
			}

			sq->pushuserpointer( v, (SQUserPointer)pResult );
			return 1;
		}
	}

	return sq->throwerror( v, _SC( "Error in 'mysql_query': Wrong number of arguments or invalid argument type" ) );
}

_SQUIRRELDEF( SQ_mysql_num_rows )
{
	SQUserPointer pPointer = NULL;
	MYSQL_RES* pResult = NULL;
	SQInteger iRows = 0;

	if ( sq->gettype( v, 2 ) == OT_USERPOINTER )
	{
		sq->getuserpointer( v, 2, &pPointer );

		if ( pPointer )
		{
			pResult = (MYSQL_RES*)pPointer;

			iRows = (SQInteger)mysql_num_rows( pResult );

			sq->pushinteger( v, iRows );
			return 1;
		}
	}

	return sq->throwerror( v, _SC( "Error in 'mysql_num_rows': Wrong number of arguments or invalid argument type" ) );
}

_SQUIRRELDEF( SQ_mysql_num_fields )
{
	SQUserPointer pPointer = NULL;
	MYSQL_RES* pResult = NULL;
	SQInteger iColumns = 0;

	if ( sq->gettype( v, 2 ) == OT_USERPOINTER )
	{
		sq->getuserpointer( v, 2, &pPointer );

		if ( pPointer )
		{
			pResult = (MYSQL_RES*)pPointer;

			iColumns = (SQInteger)mysql_num_fields( pResult );

			sq->pushinteger( v, iColumns );
			return 1;
		}
	}

	return sq->throwerror( v, _SC( "Error in 'mysql_num_fields': Wrong number of arguments or invalid argument type" ) );
}

_SQUIRRELDEF( SQ_mysql_fetch_row )
{
	SQUserPointer pPointer = NULL;
	MYSQL_RES* pResult = NULL;
	MYSQL_ROW pRow;
	unsigned int ui = 0;
	unsigned int uiColumns = 0;
	MYSQL_FIELD* pField;
	SQInteger i = 0;
	SQFloat f = 0.0f;

	if ( sq->gettype( v, 2 ) == OT_USERPOINTER )
	{
		sq->getuserpointer( v, 2, &pPointer );

		if ( pPointer )
		{
			pResult = (MYSQL_RES*)pPointer;

			pRow = mysql_fetch_row( pResult );

			if ( !pRow )
			{
				// An error occured or no more rows found
				sq->pushnull( v );
				return 1;
			}

			uiColumns = mysql_num_fields( pResult );

			if ( uiColumns > 0 )
			{
				sq->newarray( v, 0 );

				// Avoids the "null field" issue we had earlier <stormeus>
				mysql_field_seek(pResult, 0);

				for ( ; ui < uiColumns; ui++ )
				{
					pField = mysql_fetch_field( pResult );
					
					if ( !pField || !pRow[ ui ] )
					{
						sq->pushnull( v );
						sq->arrayappend( v, -2 );
					}
					else
					{
						switch ( pField->type )
						{
						case MYSQL_TYPE_TINY:
						case MYSQL_TYPE_SHORT:
						case MYSQL_TYPE_LONG:
						case MYSQL_TYPE_LONGLONG:
						case MYSQL_TYPE_INT24:
						case MYSQL_TYPE_YEAR:
						case MYSQL_TYPE_BIT:

							i = (SQInteger)( atoi( pRow[ ui ] ) );
							sq->pushinteger( v, i );
							sq->arrayappend( v, -2 );

							break;

						case MYSQL_TYPE_DECIMAL:
						case MYSQL_TYPE_NEWDECIMAL:
						case MYSQL_TYPE_FLOAT:
						case MYSQL_TYPE_DOUBLE:

							f = (SQFloat)( atof( pRow[ ui ] ) );
							sq->pushfloat( v, f );
							sq->arrayappend( v, -2 );

							break;

						case MYSQL_TYPE_NULL:

							sq->pushnull( v );
							sq->arrayappend( v, -2 );

							break;

						default:

							sq->pushstring( v, pRow[ ui ], -1 );
							sq->arrayappend( v, -2 );

							break;
						}
					}
				}

				return 1;
			}

			sq->pushnull( v );
			return 1;
		}
	}

	return sq->throwerror( v, _SC( "Error in 'mysql_fetch_row': Wrong number of arguments or invalid argument type" ) );
}

_SQUIRRELDEF( SQ_mysql_fetch_assoc )
{
	SQUserPointer pPointer = NULL;
	MYSQL_RES* pResult = NULL;
	MYSQL_ROW pRow;
	unsigned int ui = 0;
	unsigned int uiColumns = 0;
	MYSQL_FIELD* pField;
	SQInteger i = 0;
	SQFloat f = 0.0f;

	if ( sq->gettype( v, 2 ) == OT_USERPOINTER )
	{
		sq->getuserpointer( v, 2, &pPointer );

		if ( pPointer )
		{
			pResult = (MYSQL_RES*)pPointer;

			pRow = mysql_fetch_row( pResult );

			if ( !pRow )
			{
				// An error occured or no more rows found
				sq->pushnull( v );
				return 1;
			}

			uiColumns = mysql_num_fields( pResult );

			if ( uiColumns > 0 )
			{
				sq->newtable( v );

				// Avoids the "null field" issue we had earlier <stormeus>
				mysql_field_seek(pResult, 0);

				for ( ; ui < uiColumns; ui++ )
				{
					pField = mysql_fetch_field( pResult );
					
					sq->pushstring( v, pField->name, pField->name_length );

					if ( !pField || !pRow[ ui ] )
					{
						sq->pushnull( v );
					}
					else
					{
						switch ( pField->type )
						{
						case MYSQL_TYPE_TINY:
						case MYSQL_TYPE_SHORT:
						case MYSQL_TYPE_LONG:
						case MYSQL_TYPE_LONGLONG:
						case MYSQL_TYPE_INT24:
						case MYSQL_TYPE_YEAR:
						case MYSQL_TYPE_BIT:

							i = (SQInteger)( atoi( pRow[ ui ] ) );
							sq->pushinteger( v, i );
							sq->arrayappend( v, -2 );

							break;

						case MYSQL_TYPE_DECIMAL:
						case MYSQL_TYPE_NEWDECIMAL:
						case MYSQL_TYPE_FLOAT:
						case MYSQL_TYPE_DOUBLE:

							f = (SQFloat)( atof( pRow[ ui ] ) );
							sq->pushfloat( v, f );
							sq->arrayappend( v, -2 );

							break;

						case MYSQL_TYPE_NULL:

							sq->pushnull( v );
							sq->arrayappend( v, -2 );

							break;

						default:

							sq->pushstring( v, pRow[ ui ], -1 );
							sq->arrayappend( v, -2 );

							break;
						}
					}

					sq->rawset( v, -3 );
				}

				return 1;
			}

			sq->pushnull( v );
			return 1;
		}
	}

	return sq->throwerror( v, _SC( "Error in 'mysql_fetch_assoc': Wrong number of arguments or invalid argument type" ) );
}

_SQUIRRELDEF( SQ_mysql_fetch_lengths )
{
	SQUserPointer pPointer = NULL;
	MYSQL_RES* pResult = NULL;
	unsigned int ui = 0;
	unsigned int uiColumns = 0;
	unsigned long* ulLengths = NULL;

	if ( sq->gettype( v, 2 ) == OT_USERPOINTER )
	{
		sq->getuserpointer( v, 2, &pPointer );

		if ( pPointer )
		{
			pResult = (MYSQL_RES*)pPointer;

			uiColumns = mysql_num_fields( pResult );

			if ( uiColumns > 0 )
			{
				sq->newarray( v, 0 );

				ulLengths = mysql_fetch_lengths( pResult );

				for ( ; ui < uiColumns; ui++ )
				{
					sq->pushinteger( v, (SQInteger)ulLengths[ ui ] );
					sq->arrayappend( v, -2 );
				}

				return 1;
			}

			sq->pushnull( v );
			return 1;
		}
	}

	return sq->throwerror( v, _SC( "Error in 'mysql_fetch_lengths': Wrong number of arguments or invalid argument type" ) );
}

_SQUIRRELDEF( SQ_mysql_free_result )
{
	SQUserPointer pPointer = NULL;
	MYSQL_RES* pResult = NULL;

	if ( sq->gettype( v, 2 ) == OT_USERPOINTER )
	{
		sq->getuserpointer( v, 2, &pPointer );

		if ( pPointer )
		{
			pResult = (MYSQL_RES*)pPointer;

			mysql_free_result( pResult );

			sq->pushbool( v, SQTrue );
			return 1;
		}
	}

	return sq->throwerror( v, _SC( "Error in 'mysql_free_result': Wrong number of arguments or invalid argument type" ) );
}

_SQUIRRELDEF( SQ_mysql_errno )
{
	SQUserPointer pPointer = NULL;
	MYSQL* pConn = NULL;

	if ( sq->gettype( v, 2 ) == OT_USERPOINTER )
	{
		sq->getuserpointer( v, 2, &pPointer );

		if ( pPointer )
		{
			pConn = (MYSQL*)pPointer;

			sq->pushinteger( v, (SQInteger)mysql_errno( pConn ) );
			return 1;
		}
	}

	return sq->throwerror( v, _SC( "Error in 'mysql_errno': Wrong number of arguments or invalid argument type" ) );
}

_SQUIRRELDEF( SQ_mysql_error )
{
	SQUserPointer pPointer = NULL;
	MYSQL* pConn = NULL;
	const char* szError = NULL;

	if ( sq->gettype( v, 2 ) == OT_USERPOINTER )
	{
		sq->getuserpointer( v, 2, &pPointer );

		if ( pPointer )
		{
			pConn = (MYSQL*)pPointer;

			szError = mysql_error( pConn );

			sq->pushstring( v, (const SQChar*)szError, -1 );
			return 1;
		}
	}

	return sq->throwerror( v, _SC( "Error in 'mysql_error': Wrong number of arguments or invalid argument type" ) );
}

_SQUIRRELDEF( SQ_mysql_ping )
{
	SQUserPointer pPointer = NULL;
	MYSQL* pConn = NULL;

	if ( sq->gettype( v, 2 ) == OT_USERPOINTER )
	{
		sq->getuserpointer( v, 2, &pPointer );

		if ( pPointer )
		{
			pConn = (MYSQL*)pPointer;

			if ( mysql_ping( pConn ) ) sq->pushbool( v, SQFalse );
			else sq->pushbool( v, SQTrue );

			return 1;
		}
	}

	return sq->throwerror( v, _SC( "Error in 'mysql_ping': Wrong number of arguments or invalid argument type" ) );
}

_SQUIRRELDEF( SQ_mysql_escape_string )
{
	SQUserPointer pPointer = NULL;
	MYSQL* pConn = NULL;
	const SQChar* szText = NULL;
	size_t uiLen = 0;
	char* szTemp = NULL;

	if ( sq->gettype( v, 2 ) == OT_USERPOINTER &&
		sq->gettype( v, 3 ) == OT_STRING )
	{
		sq->getuserpointer( v, 2, &pPointer );
		sq->getstring( v, 3, &szText );

		if ( pPointer && szText )
		{
			pConn = (MYSQL*)pPointer;

			uiLen = strlen( szText );
			szTemp = (char*)alloca( uiLen * 2 + 1 );

			mysql_real_escape_string( pConn, szTemp, szText, uiLen );

			uiLen = strlen( szTemp );
			szTemp[ uiLen + 1 ] = '\0';

			sq->pushstring( v, szTemp, uiLen );
			return 1;
		}
	}

	return sq->throwerror( v, _SC( "Error in 'mysql_escape_string': Wrong number of arguments or invalid argument type" ) );
}

_SQUIRRELDEF( SQ_mysql_select_db )
{
	SQUserPointer pPointer = NULL;
	MYSQL* pConn = NULL;
	const SQChar* szDatabase = NULL;

	if ( sq->gettype( v, 2 ) == OT_USERPOINTER &&
		sq->gettype( v, 3 ) == OT_STRING )
	{
		sq->getuserpointer( v, 2, &pPointer );
		sq->getstring( v, 3, &szDatabase );

		if ( pPointer && szDatabase )
		{
			pConn = (MYSQL*)pPointer;

			if ( !*szDatabase )
			{
				sq->pushbool( v, SQFalse );
				return 1;
			}

			if ( mysql_select_db( pConn, szDatabase ) ) sq->pushbool( v, SQFalse );
			else sq->pushbool( v, SQTrue );

			return 1;
		}
	}

	return sq->throwerror( v, _SC( "Error in 'mysql_select_db': Wrong number of arguments or invalid argument type" ) );
}

_SQUIRRELDEF( SQ_mysql_change_user )
{
	SQUserPointer pPointer = NULL;
	MYSQL* pConn = NULL;
	const SQChar* szUser = NULL;
	const SQChar* szPass = NULL;
	const SQChar* szDatabase = NULL;

	if ( sq->gettype( v, 2 ) == OT_USERPOINTER &&
		sq->gettype( v, 3 ) == OT_STRING &&
		sq->gettype( v, 4 ) == OT_STRING )
	{
		sq->getuserpointer( v, 2, &pPointer );
		sq->getstring( v, 3, &szUser );
		sq->getstring( v, 4, &szPass );

		if ( sq->gettype( v, 5 ) == OT_STRING ) sq->getstring( v, 5, &szDatabase );

		if ( pPointer && szUser && szPass )
		{
			pConn = (MYSQL*)pPointer;

			if ( !*szUser || !*szPass )
			{
				sq->pushbool( v, SQFalse );
				return 1;
			}

			if ( mysql_change_user( pConn, szUser, szPass, szDatabase ) ) sq->pushbool( v, SQFalse );
			else sq->pushbool( v, SQTrue );

			return 1;
		}
	}

	return sq->throwerror( v, _SC( "Error in 'mysql_change_user': Wrong number of arguments or invalid argument type" ) );
}

_SQUIRRELDEF( SQ_mysql_warning_count )
{
	SQUserPointer pPointer = NULL;
	MYSQL* pConn = NULL;

	if ( sq->gettype( v, 2 ) == OT_USERPOINTER )
	{
		sq->getuserpointer( v, 2, &pPointer );

		if ( pPointer )
		{
			pConn = (MYSQL*)pPointer;

			sq->pushinteger( v, (SQInteger)mysql_warning_count( pConn ) );
			return 1;
		}
	}

	return sq->throwerror( v, _SC( "Error in 'mysql_warning_count': Wrong number of arguments or invalid argument type" ) );
}

_SQUIRRELDEF( SQ_mysql_affected_rows )
{
	SQUserPointer pPointer = NULL;
	MYSQL* pConn = NULL;

	if ( sq->gettype( v, 2 ) == OT_USERPOINTER )
	{
		sq->getuserpointer( v, 2, &pPointer );

		if ( pPointer )
		{
			pConn = (MYSQL*)pPointer;

			sq->pushinteger( v, (SQInteger)mysql_affected_rows( pConn ) );
			return 1;
		}
	}

	return sq->throwerror( v, _SC( "Error in 'mysql_affected_rows': Wrong number of arguments or invalid argument type" ) );
}

_SQUIRRELDEF( SQ_mysql_insert_id )
{
	SQUserPointer pPointer = NULL;
	MYSQL* pConn = NULL;

	if ( sq->gettype( v, 2 ) == OT_USERPOINTER )
	{
		sq->getuserpointer( v, 2, &pPointer );

		if ( pPointer )
		{
			pConn = (MYSQL*)pPointer;

			sq->pushinteger( v, (SQInteger)mysql_insert_id( pConn ) );
			return 1;
		}
	}

	return sq->throwerror( v, _SC( "Error in 'mysql_insert_id': Wrong number of arguments or invalid argument type" ) );
}

_SQUIRRELDEF( SQ_mysql_info )
{
	SQUserPointer pPointer = NULL;
	MYSQL* pConn = NULL;
	const char* szInfo = NULL;

	if ( sq->gettype( v, 2 ) == OT_USERPOINTER )
	{
		sq->getuserpointer( v, 2, &pPointer );

		if ( pPointer )
		{
			pConn = (MYSQL*)pPointer;

			szInfo = mysql_info( pConn );

			if ( !szInfo ) sq->pushnull( v );
			else sq->pushstring( v, (const SQChar*)szInfo, -1 );

			return 1;
		}
	}

	return sq->throwerror( v, _SC( "Error in 'mysql_info': Wrong number of arguments or invalid argument type" ) );
}

//--------------------------------------------------

SQInteger RegisterSquirrelFunc( HSQUIRRELVM v, SQFUNCTION f, const SQChar* fname, unsigned char ucParams, const SQChar* szParams )
{
	char szNewParams[ 32 ];

	sq->pushroottable( v );
	sq->pushstring( v, fname, -1 );
	sq->newclosure( v, f, 0 ); /* create a new function */

	if ( ucParams > 0 ) 
	{
		ucParams++; /* This is to compensate for the root table */

#ifdef _WIN32
		sprintf_s( szNewParams, 32, "t%s", szParams );
#else
		sprintf( szNewParams, "t%s", szParams );
#endif

		sq->setparamscheck( v, ucParams, szNewParams ); /* Add a param type check */
	}

	sq->setnativeclosurename( v, -1, fname );
	sq->newslot( v, -3, SQFalse );
	sq->pop( v, 1 ); /* pops the root table */

	return 0;
}

void RegisterFuncs( HSQUIRRELVM v )
{
	RegisterSquirrelFunc( v, SQ_mysql_connect, "mysql_connect", 0, 0 );
	RegisterSquirrelFunc( v, SQ_mysql_close, "mysql_close", 0, 0 );
	RegisterSquirrelFunc( v, SQ_mysql_query, "mysql_query", 0, 0 );
	RegisterSquirrelFunc( v, SQ_mysql_num_rows, "mysql_num_rows", 0, 0 );
	RegisterSquirrelFunc( v, SQ_mysql_num_fields, "mysql_num_fields", 0, 0 );
	RegisterSquirrelFunc( v, SQ_mysql_fetch_row, "mysql_fetch_row", 0, 0 );
	RegisterSquirrelFunc( v, SQ_mysql_fetch_assoc, "mysql_fetch_assoc", 0, 0 );
	RegisterSquirrelFunc( v, SQ_mysql_fetch_lengths, "mysql_fetch_lengths", 0, 0 );
	RegisterSquirrelFunc( v, SQ_mysql_free_result, "mysql_free_result", 0, 0 );
	RegisterSquirrelFunc( v, SQ_mysql_errno, "mysql_errno", 0, 0 );
	RegisterSquirrelFunc( v, SQ_mysql_error, "mysql_error", 0, 0 );
	RegisterSquirrelFunc( v, SQ_mysql_ping, "mysql_ping", 0, 0 );
	RegisterSquirrelFunc( v, SQ_mysql_escape_string, "mysql_escape_string", 0, 0 );
	RegisterSquirrelFunc( v, SQ_mysql_select_db, "mysql_select_db", 0, 0 );
	RegisterSquirrelFunc( v, SQ_mysql_change_user, "mysql_change_user", 0, 0 );
	RegisterSquirrelFunc( v, SQ_mysql_warning_count, "mysql_warning_count", 0, 0 );
	RegisterSquirrelFunc( v, SQ_mysql_affected_rows, "mysql_affected_rows", 0, 0 );
	RegisterSquirrelFunc( v, SQ_mysql_insert_id, "mysql_insert_id", 0, 0 );
	RegisterSquirrelFunc( v, SQ_mysql_info, "mysql_info", 0, 0 );
}

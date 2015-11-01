//
// SQModule: An interface to access Squirrel's C API functions
//
//	Written for Liberty Unleashed by the Liberty Unleashed Team.
//

#pragma once
#ifndef _SQMODULE_H
#define _SQMODULE_H

#include "squirrel.h"

#ifdef __cplusplus
extern "C" {
#endif

enum eSquirrelCallback
{
	/* Returns a list of pointers to Squirrel native classes (typetags) as an SQCLASSES struct */
	ID_CALLBACK_CLASSES,

	ID_CALLBACK_FORCE_DWORD = 0x7fffffff
};

typedef struct
{
	SQUserPointer Player;
	SQUserPointer Vehicle;
	SQUserPointer Object;
	SQUserPointer Pickup;
	SQUserPointer Checkpoint;
	SQUserPointer Sphere;
	SQUserPointer Blip;
	SQUserPointer SpawnClass;
	SQUserPointer Sound;
	SQUserPointer Vector;
} SquirrelClasses;


typedef struct
{
	/* VM Funcs */
	HSQUIRRELVM			( *open )					( SQInteger initialstacksize );
	HSQUIRRELVM			( *newthread )				( HSQUIRRELVM friendvm, SQInteger initialstacksize );
	void				( *seterrorhandler )		( HSQUIRRELVM v );
	void				( *close )					( HSQUIRRELVM v );
	void				( *setforeignptr )			( HSQUIRRELVM v, SQUserPointer p );
	SQUserPointer		( *getforeignptr )			( HSQUIRRELVM v );
	void				( *setprintfunc )			( HSQUIRRELVM v, SQPRINTFUNCTION printfunc, SQPRINTFUNCTION errfunc );
	SQPRINTFUNCTION		( *getprintfunc )			( HSQUIRRELVM v );
	SQRESULT			( *suspendvm )				( HSQUIRRELVM v );
	SQRESULT			( *wakeupvm )				( HSQUIRRELVM v, SQBool resumedret, SQBool retval, SQBool raiseerror, SQBool throwerror );
	SQInteger			( *getvmstate )				( HSQUIRRELVM v );

	/* Compiler Funcs */
	SQRESULT			( *compile )				( HSQUIRRELVM v, SQLEXREADFUNC read, SQUserPointer p, const SQChar* sourcename, SQBool raiseerror );
	SQRESULT			( *compilebuffer )			( HSQUIRRELVM v, const SQChar* s, SQInteger size, const SQChar* sourcename, SQBool raiseerror );
	void				( *enabledebuginfo )		( HSQUIRRELVM v, SQBool enable );
	void				( *notifyallexceptions )	( HSQUIRRELVM v, SQBool enable );
	void				( *setcompilererrorhandler )( HSQUIRRELVM v, SQCOMPILERERROR f );

	/* Stack Operations */
	void				( *push )					( HSQUIRRELVM v, SQInteger idx );
	void				( *pop )					( HSQUIRRELVM v, SQInteger nelemstopop );
	void				( *poptop )					( HSQUIRRELVM v );
	void				( *remove )					( HSQUIRRELVM v, SQInteger idx );
	SQInteger			( *gettop )					( HSQUIRRELVM v );
	void				( *settop )					( HSQUIRRELVM v, SQInteger newtop );
	SQRESULT			( *reservestack )			( HSQUIRRELVM v, SQInteger nsize );
	SQInteger			( *cmp )					( HSQUIRRELVM v );
	void				( *move )					( HSQUIRRELVM dest, HSQUIRRELVM src, SQInteger idx );

	/* Object Creation Handling */
	SQUserPointer		( *newuserdata )			( HSQUIRRELVM v, SQUnsignedInteger size );
	void				( *newtable )				( HSQUIRRELVM v );
	void				( *newarray )				( HSQUIRRELVM v, SQInteger size );
	void				( *newclosure )				( HSQUIRRELVM v, SQFUNCTION func, SQUnsignedInteger nfreevars );
	SQRESULT			( *setparamscheck )			( HSQUIRRELVM v, SQInteger nparamscheck, const SQChar* typemask );
	SQRESULT			( *bindenv )				( HSQUIRRELVM v, SQInteger idx );
	void				( *pushstring )				( HSQUIRRELVM v, const SQChar* s, SQInteger len );
	void				( *pushfloat )				( HSQUIRRELVM v, SQFloat f );
	void				( *pushinteger )			( HSQUIRRELVM v, SQInteger n );
	void				( *pushbool )				( HSQUIRRELVM v, SQBool b );
	void				( *pushuserpointer )		( HSQUIRRELVM v, SQUserPointer p );
	void				( *pushnull )				( HSQUIRRELVM v );
	SQObjectType		( *gettype )				( HSQUIRRELVM v, SQInteger idx );
	SQInteger			( *getsize )				( HSQUIRRELVM v, SQInteger idx );
	SQRESULT			( *getbase )				( HSQUIRRELVM v, SQInteger idx );
	SQBool				( *instanceof )				( HSQUIRRELVM v );
	void				( *tostring )				( HSQUIRRELVM v, SQInteger idx );
	void				( *tobool )					( HSQUIRRELVM v, SQInteger idx, SQBool* b );
	SQRESULT			( *getstring )				( HSQUIRRELVM v, SQInteger idx, const SQChar** c );
	SQRESULT			( *getinteger )				( HSQUIRRELVM v, SQInteger idx, SQInteger* i );
	SQRESULT			( *getfloat )				( HSQUIRRELVM v, SQInteger idx, SQFloat* f );
	SQRESULT			( *getbool )				( HSQUIRRELVM v, SQInteger idx, SQBool* b );
	SQRESULT			( *getthread )				( HSQUIRRELVM v, SQInteger idx, HSQUIRRELVM* thread );
	SQRESULT			( *getuserpointer )			( HSQUIRRELVM v, SQInteger idx, SQUserPointer* p );
	SQRESULT			( *getuserdata )			( HSQUIRRELVM v, SQInteger idx, SQUserPointer* p, SQUserPointer* typetag );
	SQRESULT			( *settypetag )				( HSQUIRRELVM v, SQInteger idx, SQUserPointer typetag );
	SQRESULT			( *gettypetag )				( HSQUIRRELVM v, SQInteger idx, SQUserPointer* typetag );
	void				( *setreleasehook )			( HSQUIRRELVM v, SQInteger idx, SQRELEASEHOOK hook );
	SQChar*				( *getscratchpad )			( HSQUIRRELVM v, SQInteger minsize );
	SQRESULT			( *getclosureinfo )			( HSQUIRRELVM v, SQInteger idx, SQUnsignedInteger* nparams, SQUnsignedInteger* nfreevars );
	SQRESULT			( *setnativeclosurename )	( HSQUIRRELVM v, SQInteger idx, const SQChar* name );
	SQRESULT			( *setinstanceup )			( HSQUIRRELVM v, SQInteger idx, SQUserPointer p );
	SQRESULT			( *getinstanceup )			( HSQUIRRELVM v, SQInteger idx, SQUserPointer* p, SQUserPointer typetag );
	SQRESULT			( *setclassudsize )			( HSQUIRRELVM v, SQInteger idx, SQInteger udsize );
	SQRESULT			( *newclass )				( HSQUIRRELVM v, SQBool hasbase );
	SQRESULT			( *createinstance )			( HSQUIRRELVM v, SQInteger idx );
	SQRESULT			( *setattributes )			( HSQUIRRELVM v, SQInteger idx );
	SQRESULT			( *getattributes )			( HSQUIRRELVM v, SQInteger idx );
	SQRESULT			( *getclass )				( HSQUIRRELVM v, SQInteger idx );
	void				( *weakref )				( HSQUIRRELVM v, SQInteger idx );
	SQRESULT			( *getdefaultdelegate )		( HSQUIRRELVM v, SQObjectType t );

	/* Object Manipulation Funcs */
	void				( *pushroottable )			( HSQUIRRELVM v );
	void				( *pushregistrytable )		( HSQUIRRELVM v );
	void				( *pushconsttable )			( HSQUIRRELVM v );
	SQRESULT			( *setroottable )			( HSQUIRRELVM v );
	SQRESULT			( *setconsttable )			( HSQUIRRELVM v );
	SQRESULT			( *newslot )				( HSQUIRRELVM v, SQInteger idx, SQBool bstatic );
	SQRESULT			( *deleteslot )				( HSQUIRRELVM v, SQInteger idx, SQBool pushval );
	SQRESULT			( *set )					( HSQUIRRELVM v, SQInteger idx );
	SQRESULT			( *get )					( HSQUIRRELVM v, SQInteger idx );
	SQRESULT			( *rawget )					( HSQUIRRELVM v, SQInteger idx );
	SQRESULT			( *rawset )					( HSQUIRRELVM v, SQInteger idx );
	SQRESULT			( *rawdeleteslot )			( HSQUIRRELVM v, SQInteger idx, SQBool pushval );
	SQRESULT			( *arrayappend )			( HSQUIRRELVM v, SQInteger idx );
	SQRESULT			( *arraypop )				( HSQUIRRELVM v, SQInteger idx, SQBool pushval );
	SQRESULT			( *arrayresize )			( HSQUIRRELVM v, SQInteger idx, SQInteger newsize );
	SQRESULT			( *arrayreverse )			( HSQUIRRELVM v, SQInteger idx );
	SQRESULT			( *arrayremove )			( HSQUIRRELVM v, SQInteger idx, SQInteger itemidx );
	SQRESULT			( *arrayinsert )			( HSQUIRRELVM v, SQInteger idx, SQInteger destpos );
	SQRESULT			( *setdelegate )			( HSQUIRRELVM v, SQInteger idx );
	SQRESULT			( *getdelegate )			( HSQUIRRELVM v, SQInteger idx );
	SQRESULT			( *clone )					( HSQUIRRELVM v, SQInteger idx );
	SQRESULT			( *setfreevariable )		( HSQUIRRELVM v, SQInteger idx, SQUnsignedInteger nval );
	SQRESULT			( *next )					( HSQUIRRELVM v, SQInteger idx );
	SQRESULT			( *getweakrefval )			( HSQUIRRELVM v, SQInteger idx );
	SQRESULT			( *clear )					( HSQUIRRELVM v, SQInteger idx );

	/* Call Funcs */
	SQRESULT			( *call )					( HSQUIRRELVM v, SQInteger params, SQBool retval, SQBool raiseerror );
	SQRESULT			( *resume )					( HSQUIRRELVM v, SQBool retval, SQBool raiseerror );
	const SQChar*		( *getlocal )				( HSQUIRRELVM v, SQUnsignedInteger level, SQUnsignedInteger idx );
	const SQChar*		( *getfreevariable )		( HSQUIRRELVM v, SQInteger idx, SQUnsignedInteger nval );
	SQRESULT			( *throwerror )				( HSQUIRRELVM v, const SQChar* err );
	void				( *reseterror )				( HSQUIRRELVM v );
	void				( *getlasterror )			( HSQUIRRELVM v );

	/* Raw Object Handling */
	SQRESULT			( *getstackobj )			( HSQUIRRELVM v, SQInteger idx, HSQOBJECT* po );
	void				( *pushobject )				( HSQUIRRELVM v, HSQOBJECT obj );
	void				( *addref )					( HSQUIRRELVM v, HSQOBJECT* po );
	SQBool				( *release )				( HSQUIRRELVM v, HSQOBJECT* po );
	void				( *resetobject )			( HSQOBJECT* po );
	const SQChar*		( *objtostring )			( HSQOBJECT* o );
	SQBool				( *objtobool )				( HSQOBJECT* o );
	SQInteger			( *objtointeger )			( HSQOBJECT* o );
	SQFloat				( *objtofloat )				( HSQOBJECT* o );
	SQRESULT			( *getobjtypetag )			( HSQOBJECT* o, SQUserPointer* typetag );

	/* Garbage Collection */
	SQInteger			( *collectgarbage )			( HSQUIRRELVM v );

	/* Serialization Funcs */
	SQRESULT			( *writeclosure )			( HSQUIRRELVM vm, SQWRITEFUNC writef, SQUserPointer up );
	SQRESULT			( *readclosure )			( HSQUIRRELVM vm, SQREADFUNC readf, SQUserPointer up );

	/* Memory Allocation Funcs */
	void*				( *malloc )					( SQUnsignedInteger size );
	void*				( *realloc )				( void* p, SQUnsignedInteger oldsize, SQUnsignedInteger newsize );
	void				( *free )					( void *p, SQUnsignedInteger size );

	/* Debug Funcs */
	SQRESULT			( *stackinfos )				( HSQUIRRELVM v, SQInteger level, SQStackInfos* si );
	void				( *setdebughook )			( HSQUIRRELVM v );
}
SquirrelAPI;

typedef SquirrelAPI* SQAPI;
typedef SquirrelClasses* SQCLASSES;

#ifdef __cplusplus
}
#endif

#endif

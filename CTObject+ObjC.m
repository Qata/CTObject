#include <string.h>
#include "CTObject+ObjC.h"

#define __new_object(TYPE) \
CTObjectCreate(alloc, ptr, TYPE)

CTObjectRef _CTObjectWrap(CTAllocatorRef restrict alloc, void * ptr, const char * restrict type)
{
	if (strcmp(type, @encode(CTDictionaryRef)) == 0)
	{
		return __new_object(CTOBJECT_TYPE_DICTIONARY);
	}
	else if (strcmp(type, @encode(CTArrayRef)) == 0)
	{
		return __new_object(CTOBJECT_TYPE_ARRAY);
	}
	else if (strcmp(type, @encode(CTStringRef)) == 0)
	{
		return __new_object(CTOBJECT_TYPE_STRING);
	}
	else if (strcmp(type, @encode(CTNumberRef)) == 0)
	{
		return __new_object(CTOBJECT_TYPE_NUMBER);
	}
	else if (strcmp(type, @encode(CTLargeNumberRef)) == 0)
	{
		return __new_object(CTOBJECT_TYPE_LARGE_NUMBER);
	}
	else if (strcmp(type, @encode(CTNullRef)) == 0)
	{
		return __new_object(CTOBJECT_TYPE_NULL);
	}
	
	return __new_object(CTOBJECT_NOT_AN_OBJECT);
}
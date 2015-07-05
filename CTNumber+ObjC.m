
#include "CTNumber.h"
#include <stdarg.h>
#include <string.h>

CTNumberRef _CTNumberCreate(CTAllocatorRef restrict alloc, const char * restrict type, ...)
{
	va_list list;
	va_start(list, type);
	CTNumberRef number = CTAllocatorAllocate(alloc, sizeof(CTNumber));
	number->alloc = alloc;
	
	if ((strcmp(type, @encode(int8_t)) == 0) || (strcmp(type, @encode(int16_t)) == 0) || (strcmp(type, @encode(int32_t)) == 0))
	{
		number->type = CTNUMBER_TYPE_LONG;
		number->value.Long = va_arg(list, int32_t);
	}
	else if (strcmp(type, @encode(int64_t)) == 0)
	{
		number->type = CTNUMBER_TYPE_LONG;
		number->value.Long = va_arg(list, int64_t);
	}
	else if ((strcmp(type, @encode(uint8_t)) == 0) || (strcmp(type, @encode(uint16_t)) == 0) || (strcmp(type, @encode(uint32_t)) == 0))
	{
		number->type = CTNUMBER_TYPE_ULONG;
		number->value.ULong = va_arg(list, uint32_t);
	}
	else if (strcmp(type, @encode(uint64_t)) == 0)
	{
		number->type = CTNUMBER_TYPE_ULONG;
		number->value.ULong = va_arg(list, uint64_t);
	}
	else if ((strcmp(type, @encode(float)) == 0) || (strcmp(type, @encode(double)) == 0) || (strcmp(type, @encode(long double)) == 0))
	{
		number->type = CTNUMBER_TYPE_DOUBLE;
		number->value.Double = va_arg(list, double);
	}
	else if (strcmp(type, @encode(long double)) == 0)
	{
		number->type = CTNUMBER_TYPE_DOUBLE;
		number->value.Double = va_arg(list, long double);
	}
	
	va_end(list);
	return number;
}

#pragma once
#include "CTPrelude.h"

#define CTObjectWrap(alloc, value) \
_CTObjectWrap(alloc, value, @encode(__typeof__((value))))
CTObjectRef _CTObjectWrap(CTAllocatorRef restrict alloc, void * ptr, const char * restrict type);
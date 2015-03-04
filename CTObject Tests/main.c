//
//  main.c
//  CTObject Tests
//
//  Created by Carlo Tortorella on 23/10/13.
//  Copyright (c) 2013 Carlo Tortorella. All rights reserved.
//

#include "CTPrelude.h"
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <time.h>

void recurseJSON(void * obj, int type, int indentation)
{
	return;
    for (int i = 0; i < indentation; ++i) printf("\t");
    switch (type)
    {
        case CTOBJECT_TYPE_DICTIONARY:
        {
            printf("{\n");
            CTDictionaryRef object = (CTDictionaryRef)obj;
            for (unsigned long i = 0; i < object->count; ++i)
            {
                CTStringRef key = object->elements[i]->key;
                void * ptr = CTObjectValue(object->elements[i]->value);
                for (int i = 0; i <= indentation; ++i) printf("\t");
                switch (object->elements[i]->value->type)
                {
                    case CTOBJECT_TYPE_DICTIONARY:
                        printf("'%s' = \n", CTStringUTF8String(key));
                        recurseJSON(ptr, CTOBJECT_TYPE_DICTIONARY, indentation + 1);
                        break;
                    case CTOBJECT_TYPE_ARRAY:
                        printf("'%s' = \n", CTStringUTF8String(key));
                        recurseJSON(ptr, CTOBJECT_TYPE_ARRAY, indentation + 1);
                        break;
                    case CTOBJECT_TYPE_STRING:
                        printf("'%s' = '%s'\n", CTStringUTF8String(key), ((CTStringRef)ptr)->characters);
                        break;
                    case CTOBJECT_TYPE_NUMBER:
					{
						switch (((CTNumberRef)ptr)->type)
						{
							case CTNUMBER_TYPE_DOUBLE:
								printf("'%s' = %Lf\n", CTStringUTF8String(key), ((CTNumberRef)ptr)->value.Double);
								break;
							case CTNUMBER_TYPE_INT:
								printf("'%s' = %i\n", CTStringUTF8String(key), ((CTNumberRef)ptr)->value.Int);
								break;
							case CTNUMBER_TYPE_UINT:
								printf("'%s' = %u\n", CTStringUTF8String(key), ((CTNumberRef)ptr)->value.UInt);
								break;
							case CTNUMBER_TYPE_LONG:
								printf("'%s' = %lli\n", CTStringUTF8String(key), (long long)((CTNumberRef)ptr)->value.Long);
								break;
							case CTNUMBER_TYPE_ULONG:
								printf("'%s' = %llu\n", CTStringUTF8String(key), (long long)((CTNumberRef)ptr)->value.ULong);
								break;
						}
                        break;
					}
                    case CTOBJECT_TYPE_NULL:
                        printf("'%s' = %s\n", CTStringUTF8String(key), ((CTNullRef)ptr)->value);
                        break;
					case CTOBJECT_TYPE_LARGE_NUMBER:
						printf("'%s' = ", CTStringUTF8String(key));
						switch (((CTLargeNumberRef)ptr)->base->type)
						{
							case CTNUMBER_TYPE_DOUBLE:
								printf("%Lf", ((CTLargeNumberRef)ptr)->base->value.Double);
								break;
							case CTNUMBER_TYPE_INT:
								printf("%i", ((CTLargeNumberRef)ptr)->base->value.Int);
								break;
							case CTNUMBER_TYPE_UINT:
								printf("%u", ((CTLargeNumberRef)ptr)->base->value.UInt);
								break;
							case CTNUMBER_TYPE_LONG:
								printf("%lli", (long long)((CTLargeNumberRef)ptr)->base->value.Long);
								break;
							case CTNUMBER_TYPE_ULONG:
								printf("%llu", (long long)((CTLargeNumberRef)ptr)->base->value.ULong);
								break;
						}
						printf("e%lli\n", (long long)((CTLargeNumberRef)ptr)->exponent->value.Long);
						break;
					case CTOBJECT_NOT_AN_OBJECT:
						printf("'%s' = %p\n", CTStringUTF8String(key), ptr);
						break;
				}
            }
            for (int i = 0; i < indentation; ++i) printf("\t");
            printf("}\n");
            break;
        }
            
        case CTOBJECT_TYPE_ARRAY:
        {
            printf ("[\n");
            CTArrayRef array = (CTArrayRef)obj;
            for (unsigned long i = 0; i < array->count; ++i)
            {
                if (array->elements[i]->type != CTOBJECT_TYPE_DICTIONARY && array->elements[i]->type != CTOBJECT_TYPE_ARRAY)
                    for (int i = 0; i <= indentation; ++i) printf("\t");
                void * ptr = array->elements[i]->ptr;
                switch (array->elements[i]->type)
                {
                    case CTOBJECT_TYPE_DICTIONARY:
                        recurseJSON(ptr, CTOBJECT_TYPE_DICTIONARY, indentation + 1);
                        break;
                    case CTOBJECT_TYPE_ARRAY:
                        recurseJSON(ptr, CTOBJECT_TYPE_ARRAY, indentation + 1);
                        break;
                    case CTOBJECT_TYPE_STRING:
                        printf("'%s'\n", ((CTStringRef)ptr)->characters);
                        break;
                    case CTOBJECT_TYPE_NUMBER:
					{
						switch (((CTNumberRef)ptr)->type)
						{
							case CTNUMBER_TYPE_DOUBLE:
								printf("%Lf\n", ((CTNumberRef)ptr)->value.Double);
								break;
							case CTNUMBER_TYPE_INT:
								printf("%i\n", ((CTNumberRef)ptr)->value.Int);
								break;
							case CTNUMBER_TYPE_UINT:
								printf("%u\n", ((CTNumberRef)ptr)->value.UInt);
								break;
							case CTNUMBER_TYPE_LONG:
								printf("%lli\n", (long long)((CTNumberRef)ptr)->value.Long);
								break;
							case CTNUMBER_TYPE_ULONG:
								printf("%llu\n", (long long)((CTNumberRef)ptr)->value.ULong);
								break;
						}
                        break;
					}
					case CTOBJECT_TYPE_LARGE_NUMBER:
						switch (((CTLargeNumberRef)ptr)->base->type)
						{
							case CTNUMBER_TYPE_DOUBLE:
								printf("%Lf", ((CTLargeNumberRef)ptr)->base->value.Double);
								break;
							case CTNUMBER_TYPE_INT:
								printf("%i", ((CTLargeNumberRef)ptr)->base->value.Int);
								break;
							case CTNUMBER_TYPE_UINT:
								printf("%u", ((CTLargeNumberRef)ptr)->base->value.UInt);
								break;
							case CTNUMBER_TYPE_LONG:
								printf("%lli", (long long)((CTLargeNumberRef)ptr)->base->value.Long);
								break;
							case CTNUMBER_TYPE_ULONG:
								printf("%llu", (long long)((CTLargeNumberRef)ptr)->base->value.ULong);
								break;
						}
						printf("e%lli\n", (long long)((CTLargeNumberRef)ptr)->exponent->value.Long);
						break;
                    case CTOBJECT_TYPE_NULL:
                        printf("'%s'\n", ((CTNullRef)ptr)->value);
                        break;
					case CTOBJECT_NOT_AN_OBJECT:
						printf("'%p'\n", ((CTNullRef)ptr)->value);
						break;
                }
            }
            for (int i = 0; i < indentation; ++i) printf("\t");
            printf("]\n");
            break;
        }
    }
}

void recurseBencode(CTObjectRef obj, int indentation)
{
    for (int i = 0; i < indentation; ++i) printf("\t");
    switch (obj->type)
    {
        case CTOBJECT_TYPE_DICTIONARY:
        {
            printf("{\n");
            CTDictionaryRef object = CTObjectValue(obj);
            for (unsigned long i = 0; i < object->count; ++i)
            {
                CTStringRef key = object->elements[i]->key;
                void * ptr = CTObjectValue(object->elements[i]->value);
                for (int i = 0; i <= indentation; ++i)
                    printf("\t");
                switch (object->elements[i]->value->type)
                {
                    case CTOBJECT_TYPE_DICTIONARY:
                    case CTOBJECT_TYPE_ARRAY:
                        printf("'%s' = \n", CTStringUTF8String(key));
                        recurseBencode(object->elements[i]->value, indentation + 1);
                        break;
                    case CTOBJECT_TYPE_STRING:
                        printf("'%s' = '%s'\n", CTStringUTF8String(key), ((CTStringRef)ptr)->characters);
                        break;
                    case CTOBJECT_TYPE_NUMBER:
                        printf("'%s' = '%i'\n", CTStringUTF8String(key), ((CTNumberRef)ptr)->value.Int);
                        break;
					case CTOBJECT_TYPE_NULL:
						break;
					case CTOBJECT_TYPE_LARGE_NUMBER:
						break;
					case CTOBJECT_NOT_AN_OBJECT:
						break;
                }
            }
            for (int i = 0; i < indentation; ++i) printf("\t");
            printf("}\n");
            break;
        }
            
        case CTOBJECT_TYPE_ARRAY:
        {
            printf ("[\n");
            CTArrayRef array = CTObjectValue(obj);
            for (unsigned long i = 0; i < array->count; ++i)
            {
                if (array->elements[i]->type != CTOBJECT_TYPE_DICTIONARY && array->elements[i]->type != CTOBJECT_TYPE_ARRAY)
                    for (int i = 0; i <= indentation; ++i) printf("\t");
                void * ptr = array->elements[i]->ptr;
                switch (array->elements[i]->type)
                {
                    case CTOBJECT_TYPE_DICTIONARY:
                    case CTOBJECT_TYPE_ARRAY:
                        recurseBencode(array->elements[i], indentation + 1);
                        break;
                    case CTOBJECT_TYPE_STRING:
                        printf("'%s'\n", ((CTStringRef)ptr)->characters);
                        break;
                    case CTOBJECT_TYPE_NUMBER:
                        printf("'%i'\n", ((CTNumberRef)ptr)->value.Int);
                        break;
					case CTOBJECT_TYPE_LARGE_NUMBER:
						break;
					case CTOBJECT_TYPE_NULL:
						break;
					case CTOBJECT_NOT_AN_OBJECT:
						break;
                }
            }
            for (int i = 0; i < indentation; ++i) printf("\t");
            printf("]\n");
            break;
        }
        case CTOBJECT_TYPE_STRING:
        {
            printf("'%s'\n", CTStringUTF8String(CTObjectValue(obj)));
            break;
        }
        case CTOBJECT_TYPE_NUMBER:
        {
            printf("'%lli'\n", CTNumberLongValue(CTObjectValue(obj)));
            break;
        }
		case CTOBJECT_TYPE_LARGE_NUMBER:
			break;
		case CTOBJECT_TYPE_NULL:
			break;
		case CTOBJECT_NOT_AN_OBJECT:
			break;
    }
}

void CTArrayTests()
{
	{
		CTAllocatorRef allocator = CTAllocatorCreate();
		CTErrorRef error = NULL;
		CTObjectRef array1 = CTJSONParse(allocator, "{'r':{'ip':'115.70.52.245', 'st':'2014-11-12 14:17:29', 'd':{'s':{'a':'00134801DD25','lt':'2014-11-12 14:18:49','ut':1415762329,'c':[{'cp':1,'be':0,'bs':1,'d':[{'i':52,'ut':1415762244,'t':[6,8],'g':1,'sc':[0,204],'bs':'','og':'','os':'','ss':'','fv':'','lt':0,'mx':192,'mn':170,'fr':7,'po':254,'sf':0,'at':170,'s':{'e':0,'l':0,'f':0,'le':0,'li':202,'ce':0}},{'i':52,'ut':1415762244,'t':[6,8],'g':1,'sc':[0,204],'bs':'','og':'','os':'','ss':'','fv':'','lt':0,'mx':192,'mn':170,'fr':7,'po':254,'sf':0,'at':170,'s':{'e':0,'l':192,'f':0,'le':0,'li':203,'ce':0}},{'i':52,'ut':1415762244,'t':[6,8],'g':1,'sc':[0,204],'bs':'','og':'','os':'','ss':'','fv':'','lt':0,'mx':192,'mn':170,'fr':7,'po':254,'sf':0,'at':170,'s':{'e':0,'l':0,'f':0,'le':0,'li':203,'ce':0}},{'i':52,'ut':1415762245,'t':[6,8],'g':1,'sc':[0,204],'bs':'','og':'','os':'','ss':'','fv':'','lt':0,'mx':192,'mn':170,'fr':7,'po':254,'sf':0,'at':170,'s':{'e':0,'l':192,'f':0,'le':0,'li':204,'ce':0}},{'i':52,'ut':1415762245,'t':[6,8],'g':1,'sc':[0,204],'bs':'','og':'','os':'','ss':'','fv':'','lt':0,'mx':192,'mn':170,'fr':7,'po':254,'sf':0,'at':170,'s':{'e':0,'l':0,'f':0,'le':0,'li':204,'ce':0}},{'i':52,'ut':1415762245,'t':[6,8],'g':1,'sc':[0,204],'bs':'','og':'','os':'','ss':'','fv':'','lt':0,'mx':192,'mn':170,'fr':7,'po':254,'sf':0,'at':170,'s':{'e':0,'l':192,'f':0,'le':0,'li':205,'ce':0}},{'i':52,'ut':1415762246,'t':[6,8],'g':1,'sc':[0,204],'bs':'','og':'','os':'','ss':'','fv':'','lt':0,'mx':192,'mn':170,'fr':7,'po':254,'sf':0,'at':170,'s':{'e':0,'l':0,'f':0,'le':0,'li':205,'ce':0}},{'i':52,'ut':1415762246,'t':[6,8],'g':1,'sc':[0,204],'bs':'','og':'','os':'','ss':'','fv':'','lt':0,'mx':192,'mn':170,'fr':7,'po':254,'sf':0,'at':170,'s':{'e':0,'l':192,'f':0,'le':0,'li':206,'ce':0}},{'i':52,'ut':1415762246,'t':[6,8],'g':1,'sc':[0,204],'bs':'','og':'','os':'','ss':'','fv':'','lt':0,'mx':192,'mn':170,'fr':7,'po':254,'sf':0,'at':170,'s':{'e':0,'l':0,'f':0,'le':0,'li':206,'ce':0}},{'i':52,'ut':1415762247,'t':[6,8],'g':1,'sc':[0,204],'bs':'','og':'','os':'','ss':'','fv':'','lt':0,'mx':192,'mn':170,'fr':7,'po':254,'sf':0,'at':170,'s':{'e':0,'l':192,'f':0,'le':0,'li':207,'ce':0}},{'i':52,'ut':1415762247,'t':[6,8],'g':1,'sc':[0,204],'bs':'','og':'','os':'','ss':'','fv':'','lt':0,'mx':192,'mn':170,'fr':7,'po':254,'sf':0,'at':170,'s':{'e':0,'l':0,'f':0,'le':0,'li':207,'ce':0}},{'i':52,'ut':1415762247,'t':[6,8],'g':1,'sc':[0,204],'bs':'','og':'','os':'','ss':'','fv':'','lt':0,'mx':192,'mn':170,'fr':7,'po':254,'sf':0,'at':170,'s':{'e':0,'l':192,'f':0,'le':0,'li':208,'ce':0}},{'i':52,'ut':1415762248,'t':[6,8],'g':1,'sc':[0,204],'bs':'','og':'','os':'','ss':'','fv':'','lt':0,'mx':192,'mn':170,'fr':7,'po':254,'sf':0,'at':170,'s':{'e':0,'l':0,'f':0,'le':0,'li':208,'ce':0}},{'i':52,'ut':1415762248,'t':[6,8],'g':1,'sc':[0,204],'bs':'','og':'','os':'','ss':'','fv':'','lt':0,'mx':192,'mn':170,'fr':7,'po':254,'sf':0,'at':170,'s':{'e':0,'l':192,'f':0,'le':0,'li':209,'ce':0}},{'i':52,'ut':1415762248,'t':[6,8],'g':1,'sc':[0,204],'bs':'','og':'','os':'','ss':'','fv':'','lt':0,'mx':192,'mn':170,'fr':7,'po':254,'sf':0,'at':170,'s':{'e':0,'l':0,'f':0,'le':0,'li':209,'ce':0}},{'i':52,'ut':1415762249,'t':[6,8],'g':1,'sc':[0,204],'bs':'','og':'','os':'','ss':'','fv':'','lt':0,'mx':192,'mn':170,'fr':7,'po':254,'sf':0,'at':170,'s':{'e':0,'l':192,'f':0,'le':0,'li':210,'ce':0}},{'i':52,'ut':1415762249,'t':[6,8],'g':1,'sc':[0,204],'bs':'','og':'','os':'','ss':'','fv':'','lt':0,'mx':192,'mn':170,'fr':7,'po':254,'sf':0,'at':170,'s':{'e':0,'l':0,'f':0,'le':0,'li':210,'ce':0}},{'i':52,'ut':1415762249,'t':[6,8],'g':1,'sc':[0,204],'bs':'','og':'','os':'','ss':'','fv':'','lt':0,'mx':192,'mn':170,'fr':7,'po':254,'sf':0,'at':170,'s':{'e':0,'l':192,'f':0,'le':0,'li':211,'ce':0}},{'i':52,'ut':1415762250,'t':[6,8],'g':1,'sc':[0,204],'bs':'','og':'','os':'','ss':'','fv':'','lt':0,'mx':192,'mn':170,'fr':7,'po':254,'sf':0,'at':170,'s':{'e':0,'l':0,'f':0,'le':0,'li':211,'ce':0}},{'i':52,'ut':1415762250,'t':[6,8],'g':1,'sc':[0,204],'bs':'','og':'','os':'','ss':'','fv':'','lt':0,'mx':192,'mn':170,'fr':7,'po':254,'sf':0,'at':170,'s':{'e':0,'l':192,'f':0,'le':0,'li':212,'ce':0}},{'i':52,'ut':1415762250,'t':[6,8],'g':1,'sc':[0,204],'bs':'','og':'','os':'','ss':'','fv':'','lt':0,'mx':192,'mn':170,'fr':7,'po':254,'sf':0,'at':170,'s':{'e':0,'l':0,'f':0,'le':0,'li':212,'ce':0}},{'i':52,'ut':1415762251,'t':[6,8],'g':1,'sc':[0,204],'bs':'','og':'','os':'','ss':'','fv':'','lt':0,'mx':192,'mn':170,'fr':7,'po':254,'sf':0,'at':170,'s':{'e':0,'l':192,'f':0,'le':0,'li':213,'ce':0}},{'i':52,'ut':1415762251,'t':[6,8],'g':1,'sc':[0,204],'bs':'','og':'','os':'','ss':'','fv':'','lt':0,'mx':192,'mn':170,'fr':7,'po':254,'sf':0,'at':170,'s':{'e':0,'l':0,'f':0,'le':0,'li':213,'ce':0}},{'i':52,'ut':1415762252,'t':[6,8],'g':1,'sc':[0,204],'bs':'','og':'','os':'','ss':'','fv':'','lt':0,'mx':192,'mn':170,'fr':7,'po':254,'sf':0,'at':170,'s':{'e':0,'l':9,'f':0,'le':0,'li':214,'ce':0}},{'i':52,'ut':1415762252,'t':[6,8],'g':1,'sc':[0,204],'bs':'','og':'','os':'','ss':'','fv':'','lt':0,'mx':192,'mn':170,'fr':7,'po':254,'sf':0,'at':170,'s':{'e':0,'l':192,'f':0,'le':0,'li':214,'ce':0}},{'i':26,'ut':1415762253,'t':[0],'g':16384,'sc':[],'bs':'','og':'','os':'','ss':'','fv':'','lt':0,'mx':254,'mn':85,'fr':7,'po':254,'sf':254,'at':255,'s':{'e':0,'l':254,'f':0,'le':0,'li':0,'ce':0}},{'i':52,'ut':1415762254,'t':[6,8],'g':1,'sc':[0,204],'bs':'','og':'','os':'','ss':'','fv':'','lt':0,'mx':192,'mn':170,'fr':7,'po':254,'sf':0,'at':170,'s':{'e':0,'l':0,'f':0,'le':0,'li':214,'ce':0}},{'i':52,'ut':1415762254,'t':[6,8],'g':1,'sc':[0,204],'bs':'','og':'','os':'','ss':'','fv':'','lt':0,'mx':192,'mn':170,'fr':7,'po':254,'sf':0,'at':170,'s':{'e':0,'l':192,'f':0,'le':0,'li':215,'ce':0}},{'i':27,'ut':1415762256,'t':[0],'g':16384,'sc':[],'bs':'','og':'','os':'','ss':'','fv':'','lt':0,'mx':254,'mn':85,'fr':7,'po':254,'sf':254,'at':255,'s':{'e':0,'l':254,'f':0,'le':0,'li':0,'ce':0}},{'i':52,'ut':1415762256,'t':[6,8],'g':1,'sc':[0,204],'bs':'','og':'','os':'','ss':'','fv':'','lt':0,'mx':192,'mn':170,'fr':7,'po':254,'sf':0,'at':170,'s':{'e':0,'l':0,'f':0,'le':0,'li':215,'ce':0}},{'i':52,'ut':1415762256,'t':[6,8],'g':1,'sc':[0,204],'bs':'','og':'','os':'','ss':'','fv':'','lt':0,'mx':192,'mn':170,'fr':7,'po':254,'sf':0,'at':170,'s':{'e':0,'l':192,'f':0,'le':0,'li':216,'ce':0}},{'i':52,'ut':1415762257,'t':[6,8],'g':1,'sc':[0,204],'bs':'','og':'','os':'','ss':'','fv':'','lt':0,'mx':192,'mn':170,'fr':7,'po':254,'sf':0,'at':170,'s':{'e':0,'l':0,'f':0,'le':0,'li':216,'ce':0}},{'i':52,'ut':1415762257,'t':[6,8],'g':1,'sc':[0,204],'bs':'','og':'','os':'','ss':'','fv':'','lt':0,'mx':192,'mn':170,'fr':7,'po':254,'sf':0,'at':170,'s':{'e':0,'l':192,'f':0,'le':0,'li':217,'ce':0}},{'i':52,'ut':1415762258,'t':[6,8],'g':1,'sc':[0,204],'bs':'','og':'','os':'','ss':'','fv':'','lt':0,'mx':192,'mn':170,'fr':7,'po':254,'sf':0,'at':170,'s':{'e':0,'l':0,'f':0,'le':0,'li':217,'ce':0}},{'i':52,'ut':1415762258,'t':[6,8],'g':1,'sc':[0,204],'bs':'','og':'','os':'','ss':'','fv':'','lt':0,'mx':192,'mn':170,'fr':7,'po':254,'sf':0,'at':170,'s':{'e':0,'l':192,'f':0,'le':0,'li':218,'ce':0}},{'i':52,'ut':1415762258,'t':[6,8],'g':1,'sc':[0,204],'bs':'','og':'','os':'','ss':'','fv':'','lt':0,'mx':192,'mn':170,'fr':7,'po':254,'sf':0,'at':170,'s':{'e':0,'l':0,'f':0,'le':0,'li':218,'ce':0}},{'i':52,'ut':1415762259,'t':[6,8],'g':1,'sc':[0,204],'bs':'','og':'','os':'','ss':'','fv':'','lt':0,'mx':192,'mn':170,'fr':7,'po':254,'sf':0,'at':170,'s':{'e':0,'l':192,'f':0,'le':0,'li':219,'ce':0}},{'i':52,'ut':1415762259,'t':[6,8],'g':1,'sc':[0,204],'bs':'','og':'','os':'','ss':'','fv':'','lt':0,'mx':192,'mn':170,'fr':7,'po':254,'sf':0,'at':170,'s':{'e':0,'l':0,'f':0,'le':0,'li':219,'ce':0}},{'i':52,'ut':1415762259,'t':[6,8],'g':1,'sc':[0,204],'bs':'','og':'','os':'','ss':'','fv':'','lt':0,'mx':192,'mn':170,'fr':7,'po':254,'sf':0,'at':170,'s':{'e':0,'l':192,'f':0,'le':0,'li':220,'ce':0}},{'i':52,'ut':1415762259,'t':[6,8],'g':1,'sc':[0,204],'bs':'','og':'','os':'','ss':'','fv':'','lt':0,'mx':192,'mn':170,'fr':7,'po':254,'sf':0,'at':170,'s':{'e':0,'l':0,'f':0,'le':0,'li':220,'ce':0}},{'i':52,'ut':1415762260,'t':[6,8],'g':1,'sc':[0,204],'bs':'','og':'','os':'','ss':'','fv':'','lt':0,'mx':192,'mn':170,'fr':7,'po':254,'sf':0,'at':170,'s':{'e':0,'l':192,'f':0,'le':0,'li':221,'ce':0}},{'i':52,'ut':1415762260,'t':[6,8],'g':1,'sc':[0,204],'bs':'','og':'','os':'','ss':'','fv':'','lt':0,'mx':192,'mn':170,'fr':7,'po':254,'sf':0,'at':170,'s':{'e':0,'l':0,'f':0,'le':0,'li':221,'ce':0}},{'i':52,'ut':1415762260,'t':[6,8],'g':1,'sc':[0,204],'bs':'','og':'','os':'','ss':'','fv':'','lt':0,'mx':192,'mn':170,'fr':7,'po':254,'sf':0,'at':170,'s':{'e':0,'l':192,'f':0,'le':0,'li':222,'ce':0}},{'i':52,'ut':1415762261,'t':[6,8],'g':1,'sc':[0,204],'bs':'','og':'','os':'','ss':'','fv':'','lt':0,'mx':192,'mn':170,'fr':7,'po':254,'sf':0,'at':170,'s':{'e':0,'l':0,'f':0,'le':0,'li':222,'ce':0}},{'i':52,'ut':1415762261,'t':[6,8],'g':1,'sc':[0,204],'bs':'','og':'','os':'','ss':'','fv':'','lt':0,'mx':192,'mn':170,'fr':7,'po':254,'sf':0,'at':170,'s':{'e':0,'l':192,'f':0,'le':0,'li':223,'ce':0}},{'i':52,'ut':1415762262,'t':[6,8],'g':1,'sc':[0,204],'bs':'','og':'','os':'','ss':'','fv':'','lt':0,'mx':192,'mn':170,'fr':7,'po':254,'sf':0,'at':170,'s':{'e':0,'l':0,'f':0,'le':0,'li':223,'ce':0}},{'i':51,'ut':1415762262,'t':[0],'g':1024,'sc':[],'bs':'','og':'','os':'','ss':'','fv':'','lt':0,'mx':254,'mn':85,'fr':7,'po':254,'sf':0,'at':255,'s':{'e':1,'l':0,'f':0,'le':0,'li':0,'ce':0}},{'i':52,'ut':1415762262,'t':[6,8],'g':1,'sc':[0,204],'bs':'','og':'','os':'','ss':'','fv':'','lt':0,'mx':192,'mn':170,'fr':7,'po':254,'sf':0,'at':170,'s':{'e':0,'l':192,'f':0,'le':0,'li':224,'ce':0}},{'i':52,'ut':1415762262,'t':[6,8],'g':1,'sc':[0,204],'bs':'','og':'','os':'','ss':'','fv':'','lt':0,'mx':192,'mn':170,'fr':7,'po':254,'sf':0,'at':170,'s':{'e':0,'l':0,'f':0,'le':0,'li':224,'ce':0}},{'i':52,'ut':1415762263,'t':[6,8],'g':1,'sc':[0,204],'bs':'','og':'','os':'','ss':'','fv':'','lt':0,'mx':192,'mn':170,'fr':7,'po':254,'sf':0,'at':170,'s':{'e':0,'l':192,'f':0,'le':0,'li':225,'ce':0}},{'i':52,'ut':1415762263,'t':[6,8],'g':1,'sc':[0,204],'bs':'','og':'','os':'','ss':'','fv':'','lt':0,'mx':192,'mn':170,'fr':7,'po':254,'sf':0,'at':170,'s':{'e':0,'l':0,'f':0,'le':0,'li':225,'ce':0}},{'i':52,'ut':1415762263,'t':[6,8],'g':1,'sc':[0,204],'bs':'','og':'','os':'','ss':'','fv':'','lt':0,'mx':192,'mn':170,'fr':7,'po':254,'sf':0,'at':170,'s':{'e':0,'l':192,'f':0,'le':0,'li':226,'ce':0}},{'i':52,'ut':1415762264,'t':[6,8],'g':1,'sc':[0,204],'bs':'','og':'','os':'','ss':'','fv':'','lt':0,'mx':192,'mn':170,'fr':7,'po':254,'sf':0,'at':170,'s':{'e':0,'l':0,'f':0,'le':0,'li':226,'ce':0}},{'i':52,'ut':1415762264,'t':[6,8],'g':1,'sc':[0,204],'bs':'','og':'','os':'','ss':'','fv':'','lt':0,'mx':192,'mn':170,'fr':7,'po':254,'sf':0,'at':170,'s':{'e':0,'l':192,'f':0,'le':0,'li':227,'ce':0}},{'i':52,'ut':1415762264,'t':[6,8],'g':1,'sc':[0,204],'bs':'','og':'','os':'','ss':'','fv':'','lt':0,'mx':192,'mn':170,'fr':7,'po':254,'sf':0,'at':170,'s':{'e':0,'l':0,'f':0,'le':0,'li':227,'ce':0}},{'i':52,'ut':1415762265,'t':[6,8],'g':1,'sc':[0,204],'bs':'','og':'','os':'','ss':'','fv':'','lt':0,'mx':192,'mn':170,'fr':7,'po':254,'sf':0,'at':170,'s':{'e':0,'l':192,'f':0,'le':0,'li':228,'ce':0}},{'i':52,'ut':1415762265,'t':[6,8],'g':1,'sc':[0,204],'bs':'','og':'','os':'','ss':'','fv':'','lt':0,'mx':192,'mn':170,'fr':7,'po':254,'sf':0,'at':170,'s':{'e':0,'l':0,'f':0,'le':0,'li':228,'ce':0}},{'i':52,'ut':1415762265,'t':[6,8],'g':1,'sc':[0,204],'bs':'','og':'','os':'','ss':'','fv':'','lt':0,'mx':192,'mn':170,'fr':7,'po':254,'sf':0,'at':170,'s':{'e':0,'l':192,'f':0,'le':0,'li':229,'ce':0}},{'i':52,'ut':1415762266,'t':[6,8],'g':1,'sc':[0,204],'bs':'','og':'','os':'','ss':'','fv':'','lt':0,'mx':192,'mn':170,'fr':7,'po':254,'sf':0,'at':170,'s':{'e':0,'l':0,'f':0,'le':0,'li':229,'ce':0}},{'i':52,'ut':1415762266,'t':[6,8],'g':1,'sc':[0,204],'bs':'','og':'','os':'','ss':'','fv':'','lt':0,'mx':192,'mn':170,'fr':7,'po':254,'sf':0,'at':170,'s':{'e':0,'l':192,'f':0,'le':0,'li':230,'ce':0}},{'i':52,'ut':1415762266,'t':[6,8],'g':1,'sc':[0,204],'bs':'','og':'','os':'','ss':'','fv':'','lt':0,'mx':192,'mn':170,'fr':7,'po':254,'sf':0,'at':170,'s':{'e':0,'l':0,'f':0,'le':0,'li':230,'ce':0}},{'i':26,'ut':1415762266,'t':[0],'g':16384,'sc':[],'bs':'','og':'','os':'','ss':'','fv':'','lt':0,'mx':254,'mn':85,'fr':7,'po':254,'sf':254,'at':255,'s':{'e':1,'l':254,'f':0,'le':0,'li':0,'ce':0}},{'i':52,'ut':1415762267,'t':[6,8],'g':1,'sc':[0,204],'bs':'','og':'','os':'','ss':'','fv':'','lt':0,'mx':192,'mn':170,'fr':7,'po':254,'sf':0,'at':170,'s':{'e':0,'l':9,'f':0,'le':0,'li':231,'ce':0}},{'i':27,'ut':1415762267,'t':[0],'g':16384,'sc':[],'bs':'','og':'','os':'','ss':'','fv':'','lt':0,'mx':254,'mn':85,'fr':7,'po':254,'sf':254,'at':255,'s':{'e':1,'l':254,'f':0,'le':0,'li':0,'ce':0}},{'i':52,'ut':1415762267,'t':[6,8],'g':1,'sc':[0,204],'bs':'','og':'','os':'','ss':'','fv':'','lt':0,'mx':192,'mn':170,'fr':7,'po':254,'sf':0,'at':170,'s':{'e':0,'l':9,'f':0,'le':0,'li':232,'ce':0}},{'i':52,'ut':1415762268,'t':[6,8],'g':1,'sc':[0,204],'bs':'','og':'','os':'','ss':'','fv':'','lt':0,'mx':192,'mn':170,'fr':7,'po':254,'sf':0,'at':170,'s':{'e':0,'l':192,'f':0,'le':0,'li':233,'ce':0}},{'i':52,'ut':1415762268,'t':[6,8],'g':1,'sc':[0,204],'bs':'','og':'','os':'','ss':'','fv':'','lt':0,'mx':192,'mn':170,'fr':7,'po':254,'sf':0,'at':170,'s':{'e':0,'l':0,'f':0,'le':0,'li':233,'ce':0}},{'i':52,'ut':1415762268,'t':[6,8],'g':1,'sc':[0,204],'bs':'','og':'','os':'','ss':'','fv':'','lt':0,'mx':192,'mn':170,'fr':7,'po':254,'sf':0,'at':170,'s':{'e':0,'l':192,'f':0,'le':0,'li':234,'ce':0}},{'i':52,'ut':1415762269,'t':[6,8],'g':1,'sc':[0,204],'bs':'','og':'','os':'','ss':'','fv':'','lt':0,'mx':192,'mn':170,'fr':7,'po':254,'sf':0,'at':170,'s':{'e':0,'l':0,'f':0,'le':0,'li':234,'ce':0}},{'i':52,'ut':1415762269,'t':[6,8],'g':1,'sc':[0,204],'bs':'','og':'','os':'','ss':'','fv':'','lt':0,'mx':192,'mn':170,'fr':7,'po':254,'sf':0,'at':170,'s':{'e':0,'l':9,'f':0,'le':0,'li':235,'ce':0}},{'i':52,'ut':1415762269,'t':[6,8],'g':1,'sc':[0,204],'bs':'','og':'','os':'','ss':'','fv':'','lt':0,'mx':192,'mn':170,'fr':7,'po':254,'sf':0,'at':170,'s':{'e':0,'l':9,'f':0,'le':0,'li':236,'ce':0}},{'i':52,'ut':1415762270,'t':[6,8],'g':1,'sc':[0,204],'bs':'','og':'','os':'','ss':'','fv':'','lt':0,'mx':192,'mn':170,'fr':7,'po':254,'sf':0,'at':170,'s':{'e':0,'l':9,'f':0,'le':0,'li':237,'ce':0}},{'i':52,'ut':1415762270,'t':[6,8],'g':1,'sc':[0,204],'bs':'','og':'','os':'','ss':'','fv':'','lt':0,'mx':192,'mn':170,'fr':7,'po':254,'sf':0,'at':170,'s':{'e':0,'l':192,'f':0,'le':0,'li':238,'ce':0}},{'i':52,'ut':1415762270,'t':[6,8],'g':1,'sc':[0,204],'bs':'','og':'','os':'','ss':'','fv':'','lt':0,'mx':192,'mn':170,'fr':7,'po':254,'sf':0,'at':170,'s':{'e':0,'l':0,'f':0,'le':0,'li':238,'ce':0}},{'i':52,'ut':1415762270,'t':[6,8],'g':1,'sc':[0,204],'bs':'','og':'','os':'','ss':'','fv':'','lt':0,'mx':192,'mn':170,'fr':7,'po':254,'sf':0,'at':170,'s':{'e':0,'l':192,'f':0,'le':0,'li':239,'ce':0}},{'i':52,'ut':1415762271,'t':[6,8],'g':1,'sc':[0,204],'bs':'','og':'','os':'','ss':'','fv':'','lt':0,'mx':192,'mn':170,'fr':7,'po':254,'sf':0,'at':170,'s':{'e':0,'l':0,'f':0,'le':0,'li':239,'ce':0}},{'i':52,'ut':1415762271,'t':[6,8],'g':1,'sc':[0,204],'bs':'','og':'','os':'','ss':'','fv':'','lt':0,'mx':192,'mn':170,'fr':7,'po':254,'sf':0,'at':170,'s':{'e':0,'l':192,'f':0,'le':0,'li':240,'ce':0}},{'i':52,'ut':1415762271,'t':[6,8],'g':1,'sc':[0,204],'bs':'','og':'','os':'','ss':'','fv':'','lt':0,'mx':192,'mn':170,'fr':7,'po':254,'sf':0,'at':170,'s':{'e':0,'l':0,'f':0,'le':0,'li':240,'ce':0}},{'i':52,'ut':1415762272,'t':[6,8],'g':1,'sc':[0,204],'bs':'','og':'','os':'','ss':'','fv':'','lt':0,'mx':192,'mn':170,'fr':7,'po':254,'sf':0,'at':170,'s':{'e':0,'l':192,'f':0,'le':0,'li':241,'ce':0}},{'i':52,'ut':1415762272,'t':[6,8],'g':1,'sc':[0,204],'bs':'','og':'','os':'','ss':'','fv':'','lt':0,'mx':192,'mn':170,'fr':7,'po':254,'sf':0,'at':170,'s':{'e':0,'l':0,'f':0,'le':0,'li':241,'ce':0}},{'i':52,'ut':1415762272,'t':[6,8],'g':1,'sc':[0,204],'bs':'','og':'','os':'','ss':'','fv':'','lt':0,'mx':192,'mn':170,'fr':7,'po':254,'sf':0,'at':170,'s':{'e':0,'l':192,'f':0,'le':0,'li':242,'ce':0}},{'i':52,'ut':1415762272,'t':[6,8],'g':1,'sc':[0,204],'bs':'','og':'','os':'','ss':'','fv':'','lt':0,'mx':192,'mn':170,'fr':7,'po':254,'sf':0,'at':170,'s':{'e':0,'l':0,'f':0,'le':0,'li':242,'ce':0}},{'i':52,'ut':1415762273,'t':[6,8],'g':1,'sc':[0,204],'bs':'','og':'','os':'','ss':'','fv':'','lt':0,'mx':192,'mn':170,'fr':7,'po':254,'sf':0,'at':170,'s':{'e':0,'l':192,'f':0,'le':0,'li':243,'ce':0}},{'i':52,'ut':1415762273,'t':[6,8],'g':1,'sc':[0,204],'bs':'','og':'','os':'','ss':'','fv':'','lt':0,'mx':192,'mn':170,'fr':7,'po':254,'sf':0,'at':170,'s':{'e':0,'l':0,'f':0,'le':0,'li':243,'ce':0}},{'i':52,'ut':1415762273,'t':[6,8],'g':1,'sc':[0,204],'bs':'','og':'','os':'','ss':'','fv':'','lt':0,'mx':192,'mn':170,'fr':7,'po':254,'sf':0,'at':170,'s':{'e':0,'l':192,'f':0,'le':0,'li':244,'ce':0}},{'i':52,'ut':1415762273,'t':[6,8],'g':1,'sc':[0,204],'bs':'','og':'','os':'','ss':'','fv':'','lt':0,'mx':192,'mn':170,'fr':7,'po':254,'sf':0,'at':170,'s':{'e':0,'l':0,'f':0,'le':0,'li':244,'ce':0}},{'i':52,'ut':1415762274,'t':[6,8],'g':1,'sc':[0,204],'bs':'','og':'','os':'','ss':'','fv':'','lt':0,'mx':192,'mn':170,'fr':7,'po':254,'sf':0,'at':170,'s':{'e':0,'l':192,'f':0,'le':0,'li':245,'ce':0}},{'i':52,'ut':1415762274,'t':[6,8],'g':1,'sc':[0,204],'bs':'','og':'','os':'','ss':'','fv':'','lt':0,'mx':192,'mn':170,'fr':7,'po':254,'sf':0,'at':170,'s':{'e':0,'l':0,'f':0,'le':0,'li':245,'ce':0}},{'i':52,'ut':1415762280,'t':[6,8],'g':1,'sc':[0,204],'bs':'','og':'','os':'','ss':'','fv':'','lt':0,'mx':192,'mn':170,'fr':7,'po':254,'sf':0,'at':170,'s':{'e':0,'l':192,'f':0,'le':0,'li':246,'ce':0}},{'i':52,'ut':1415762281,'t':[6,8],'g':1,'sc':[0,204],'bs':'','og':'','os':'','ss':'','fv':'','lt':0,'mx':192,'mn':170,'fr':7,'po':254,'sf':0,'at':170,'s':{'e':0,'l':0,'f':0,'le':0,'li':246,'ce':0}},{'i':52,'ut':1415762282,'t':[6,8],'g':1,'sc':[0,204],'bs':'','og':'','os':'','ss':'','fv':'','lt':0,'mx':192,'mn':170,'fr':7,'po':254,'sf':0,'at':170,'s':{'e':0,'l':192,'f':0,'le':0,'li':247,'ce':0}},{'i':52,'ut':1415762284,'t':[6,8],'g':1,'sc':[0,204],'bs':'','og':'','os':'','ss':'','fv':'','lt':0,'mx':192,'mn':170,'fr':7,'po':254,'sf':0,'at':170,'s':{'e':0,'l':0,'f':0,'le':0,'li':247,'ce':0}},{'i':52,'ut':1415762285,'t':[6,8],'g':1,'sc':[0,204],'bs':'','og':'','os':'','ss':'','fv':'','lt':0,'mx':192,'mn':170,'fr':7,'po':254,'sf':0,'at':170,'s':{'e':0,'l':192,'f':0,'le':0,'li':248,'ce':0}},{'i':52,'ut':1415762286,'t':[6,8],'g':1,'sc':[0,204],'bs':'','og':'','os':'','ss':'','fv':'','lt':0,'mx':192,'mn':170,'fr':7,'po':254,'sf':0,'at':170,'s':{'e':0,'l':0,'f':0,'le':0,'li':248,'ce':0}},{'i':52,'ut':1415762287,'t':[6,8],'g':1,'sc':[0,204],'bs':'','og':'','os':'','ss':'','fv':'','lt':0,'mx':192,'mn':170,'fr':7,'po':254,'sf':0,'at':170,'s':{'e':0,'l':192,'f':0,'le':0,'li':249,'ce':0}},{'i':52,'ut':1415762287,'t':[6,8],'g':1,'sc':[0,204],'bs':'','og':'','os':'','ss':'','fv':'','lt':0,'mx':192,'mn':170,'fr':7,'po':254,'sf':0,'at':170,'s':{'e':0,'l':0,'f':0,'le':0,'li':249,'ce':0}},{'i':52,'ut':1415762288,'t':[6,8],'g':1,'sc':[0,204],'bs':'','og':'','os':'','ss':'','fv':'','lt':0,'mx':192,'mn':170,'fr':7,'po':254,'sf':0,'at':170,'s':{'e':0,'l':192,'f':0,'le':0,'li':250,'ce':0}},{'i':52,'ut':1415762288,'t':[6,8],'g':1,'sc':[0,204],'bs':'','og':'','os':'','ss':'','fv':'','lt':0,'mx':192,'mn':170,'fr':7,'po':254,'sf':0,'at':170,'s':{'e':0,'l':0,'f':0,'le':0,'li':250,'ce':0}},{'i':52,'ut':1415762291,'t':[6,8],'g':1,'sc':[0,204],'bs':'','og':'','os':'','ss':'','fv':'','lt':0,'mx':192,'mn':170,'fr':7,'po':254,'sf':0,'at':170,'s':{'e':0,'l':192,'f':0,'le':0,'li':251,'ce':0}},{'i':52,'ut':1415762291,'t':[6,8],'g':1,'sc':[0,204],'bs':'','og':'','os':'','ss':'','fv':'','lt':0,'mx':192,'mn':170,'fr':7,'po':254,'sf':0,'at':170,'s':{'e':0,'l':0,'f':0,'le':0,'li':251,'ce':0}}]}]}}}}", CTJSONOptionsSingleQuoteStrings, &error);
		assert(!error);
		CTObjectRelease(array1);
		CTAllocatorRelease(allocator);
	}
	{
		CTAllocatorRef allocator = CTAllocatorCreate();
		CTErrorRef error = NULL;
		CTObjectRef array1 = CTJSONParse(allocator, "['a', 1, '2', 2, [], {}, 1343e380]", CTJSONOptionsSingleQuoteStrings, &error);
		assert(!error);
		CTObjectRef array2 = CTJSONParse(allocator, "[1, '2', [], {}, 2, 1343e380, 'a']", CTJSONOptionsSingleQuoteStrings, &error);
		assert(!error);
		assert(CTArrayCompare(CTObjectValue(array1), CTObjectValue(array2)));
		
		recurseJSON(array1->ptr, CTOBJECT_TYPE_ARRAY, 0);
		
		assert(CTArrayIndexOfEntryByValue(CTObjectValue(array1), CTObjectWithString(allocator, CTStringCreate(allocator, "a"))) == 0);
		assert(CTArrayIndexOfEntryByValue(CTObjectValue(array1), CTObjectWithNumber(allocator, CTNumberCreateWithLong(allocator, 1))) == 1);
		assert(CTArrayIndexOfEntryByValue(CTObjectValue(array1), CTObjectWithString(allocator, CTStringCreate(allocator, "2"))) == 2);
		assert(CTArrayIndexOfEntryByValue(CTObjectValue(array1), CTObjectWithNumber(allocator, CTNumberCreateWithLong(allocator, 2))) == 3);
		assert(CTArrayIndexOfEntryByValue(CTObjectValue(array1), CTObjectWithArray(allocator, CTArrayCreate(allocator))) == 4);
		assert(CTArrayIndexOfEntryByValue(CTObjectValue(array1), CTObjectWithDictionary(allocator, CTDictionaryCreate(allocator))) == 5);
		assert(CTArrayIndexOfEntryByValue(CTObjectValue(array1), CTObjectWithLargeNumber(allocator, CTLargeNumberCreate(allocator, CTNumberCreateWithLong(allocator, 1343), CTNumberCreateWithLong(allocator, 380)))) == 6);
		CTAllocatorRelease(allocator);
	}
	{
		CTAllocatorRef allocator = CTAllocatorCreate();
		CTErrorRef error = NULL;
		CTObjectRef array1 = CTJSONParse(allocator, "['a', 1, '2', 2, [1, 2, '4', 3, '5'], {'1':1, '2':2, '3':3, '4':4, '5':5}, 1343e380]", CTJSONOptionsSingleQuoteStrings, &error);
		assert(!error);
		CTObjectRef array2 = CTJSONParse(allocator, "[1, '2', [1, 2, 3, '4', '5'], {'5':5, '1':1, '2':2, '4':4, '3':3}, 2, 'a', 1343e380]", CTJSONOptionsSingleQuoteStrings, &error);
		assert(!error);
		assert(CTArrayCompare(CTObjectValue(array1), CTObjectValue(array2)));
		CTAllocatorRelease(allocator);
	}
	{
		CTAllocatorRef allocator = CTAllocatorCreate();
		CTErrorRef error = NULL;
		CTObjectRef array1 = CTJSONParse(allocator, "['a', 1, '2', 2, [1, 2, '4', 3, '5'], {'1':1, '2':2, '3':3, '4':4, '5':5}, 1343e380]", CTJSONOptionsSingleQuoteStrings, &error);
		assert(!error);
		CTObjectRef array2 = CTJSONParse(allocator, "[1, '2', [1, 2, 3, '4', '5'], {'5':3, '1':1, '2':2, '4':4, '3':3}, 2, 'a', 1343e380]", CTJSONOptionsSingleQuoteStrings, &error);
		assert(!error);
		assert(!CTArrayCompare(CTObjectValue(array1), CTObjectValue(array2)));
		CTAllocatorRelease(allocator);
	}
	{
		CTAllocatorRef allocator = CTAllocatorCreate();
		CTErrorRef error = NULL;
		CTObjectRef array1 = CTJSONParse(allocator, "['a', 1, '2', 2, [1, 2, '4', 3, '5'], {'1':1, '2':2, '3':3, '4':4, '5':5}, 1343e380]", CTJSONOptionsSingleQuoteStrings, &error);
		assert(!error);
		CTObjectRef array2 = CTJSONParse(allocator, "[1, '2', [1, 2, 3, '4', '5'], {'1':1, '2':2, '4':4, '3':3}, 2, 'a', 1343e380]", CTJSONOptionsSingleQuoteStrings, &error);
		assert(!error);
		assert(!CTArrayCompare(CTObjectValue(array1), CTObjectValue(array2)));
		CTAllocatorRelease(allocator);
	}
	{
		CTAllocatorRef allocator = CTAllocatorCreate();
		CTErrorRef error = NULL;
		CTObjectRef array1 = CTJSONParse(allocator, "['a', 1, '2', 2, [1, 2, '4', 3, '5'], {'1':1, '2':2, '3':3, '4':4, '5':5}, 1343e380]", CTJSONOptionsSingleQuoteStrings, &error);
		assert(!error);
		CTObjectRef array2 = CTJSONParse(allocator, "[1, '2', [1, 2, 3, '4', '5'], {'5':5, '1':1, '2':2, '4':4, '3':3}, 2, 'a', 1343.0e380]", CTJSONOptionsSingleQuoteStrings, &error);
		assert(!error);
		assert(!CTArrayCompare(CTObjectValue(array1), CTObjectValue(array2)));
		CTAllocatorRelease(allocator);
	}
}

int main(int argc, const char * argv[])
{
	uint64_t clock_values = 0;
	uint64_t smoothing_factor = 100;
	for (uint64_t i = 0; i < smoothing_factor; ++i)
	{
		clock_t t = clock();
#pragma mark - CTAllocator Test Begin
		CTAllocatorRef allocator = CTAllocatorCreate();
		CTAllocatorRelease(allocator);
		allocator = CTAllocatorCreate();
		
		char ** testStrings = CTAllocatorAllocate(allocator, sizeof(void *) * 0x10);
		char ** testStrings2 = CTAllocatorAllocate(allocator, sizeof(void *) * 0x10);
		
		for (int i = 0; i < 0x10; ++i)
		{
			char string[0x10];
			char string2[0x10];
			memset(string, 0, 0x10);
			sprintf(string, "Test%i", i);
			sprintf(string2, "Test%i", i + 0x10);
			testStrings[i] = stringDuplicate(allocator, string);
			testStrings2[i] = stringDuplicate(allocator, string);
		}
		
#pragma mark - CTArray Test Begin
		CTArrayTests();
		CTArrayRef array = CTArrayCreate(allocator);
		
		for (int i = 0; i < 0x10; ++i)
		{
			CTArrayAddEntry(array, testStrings[i], CTOBJECT_NOT_AN_OBJECT);
		}
		
		assert(array->count == 0x10);
		assert(CTArrayIndexOfEntryByReference(array, (void *)"not found in array") == CT_NOT_FOUND);
		
		for (int i = 0; i < 0x10; ++i)
		{
			assert(strcmp(array->elements[i]->ptr, testStrings[i]) == 0);
		}
		
		for (int i = 0xF; i >= 0x0; --i)
		{
			CTArrayDeleteEntry(array, i);
		}
		
		assert(array->count == 0x0);
		
#pragma mark - CTDictionary Test Begin
		CTDictionaryRef dict = CTDictionaryCreate(allocator);
		
		for (int i = 0; i < 0x10; ++i)
		{
			CTDictionaryAddEntry(dict, testStrings[i], CTObjectCreate(allocator, testStrings2[i], CTOBJECT_NOT_AN_OBJECT));
		}
		
		assert(dict->count == 0x10);
		assert(CTDictionaryIndexOfEntry(dict, "not found in dictionary") == CT_NOT_FOUND);
		assert(CTDictionaryObjectForKey(dict, "not found in dictionary") == NULL);
		
		for (int i = 0; i < 0x10; ++i)
		{
			assert(CTDictionaryIndexOfEntry(dict, testStrings[i]) == i);
			assert(strcmp(CTDictionaryObjectForKey(dict, testStrings[i])->ptr, testStrings2[i]) == 0);
		}
		
		for (int i = 0; i < 0x10; ++i)
		{
			CTDictionaryDeleteEntry(dict, testStrings[i]);
		}
		
		assert(dict->count == 0x0);
		
#pragma mark - CTString Test Begin
		char * stringTest = "Test of string";
		char * prepend = "Prepended Characters. ";
		char * append = ". Appended Characters";
		
		CTStringRef string = CTStringCreate(allocator, stringTest);
		printf("%llu\n", CTStringHash(string));
		CTStringAppendCharacters(string, append, CTSTRING_NO_LIMIT);
		printf("%llu\n", CTStringHash(string));
		CTStringPrependCharacters(string, prepend, CTSTRING_NO_LIMIT);
		assert(strcmp(CTStringUTF8String(string), "Prepended Characters. Test of string. Appended Characters") == 0);
		assert(strcmp(CTStringStringBetween(string, prepend, append), "Test of string") == 0);
		assert(CTStringStringBetween(string, append, prepend) == NULL);
		assert(CTStringStringBetween(string, "not found", "in string") == NULL);
		CTStringRemoveCharactersFromEnd(string, strlen(append));
		assert(strcmp(CTStringUTF8String(string), "Prepended Characters. Test of string") == 0);
		CTStringRemoveCharactersFromStart(string, strlen(prepend));
		assert(strcmp(CTStringUTF8String(string), stringTest) == 0);
		
#pragma mark - CTNumber Test Begin
		CTNumberRef number = CTNumberCreateWithLong(allocator, 0xFF);
		assert((number->value.Int & number->value.UInt & number->value.ULong & number->value.Long) == 0xFF);
		CTNumberSetDoubleValue(number, 255.5);
		assert(number->value.Double == 255.5);
		
#pragma mark - CTJSON Test Begin
		
		CTAllocatorRelease(allocator);
		allocator = CTAllocatorCreate();
		
		array = CTArrayCreate(allocator);
		CTArrayAddEntry(array, "{}", CTOBJECT_NOT_AN_OBJECT);
		CTArrayAddEntry(array, "{ \"v\":\"1\"}", CTOBJECT_NOT_AN_OBJECT);
		CTArrayAddEntry(array, "{ \"v\":\"1\"\r\n}", CTOBJECT_NOT_AN_OBJECT);
		CTArrayAddEntry(array, "{ \"v\":1}", CTOBJECT_NOT_AN_OBJECT);
		CTArrayAddEntry(array, "{ \"v\":\"ab'c\"}", CTOBJECT_NOT_AN_OBJECT);
		CTArrayAddEntry(array, "{ \"PI\":3141593e-6}", CTOBJECT_NOT_AN_OBJECT);
		CTArrayAddEntry(array, "{ \"PI\":3141593.0E-6}", CTOBJECT_NOT_AN_OBJECT);
		CTArrayAddEntry(array, "{ \"PI\":0.3141593e1}", CTOBJECT_NOT_AN_OBJECT);
		CTArrayAddEntry(array, "{ \"v\":12345123456789}", CTOBJECT_NOT_AN_OBJECT);
		CTArrayAddEntry(array, "{ \"v\":123456789123456789123456789}", CTOBJECT_NOT_AN_OBJECT);
		CTArrayAddEntry(array, "{ \"v\":[ 0,-1,2,3,4]}", CTOBJECT_NOT_AN_OBJECT);
		CTArrayAddEntry(array, "{ \"v\":[ \"1\",\"2\",\"3\",\"4\"]}", CTOBJECT_NOT_AN_OBJECT);
		CTArrayAddEntry(array, "{ \"v\":[ { \n}, { },[]]}", CTOBJECT_NOT_AN_OBJECT);
		CTArrayAddEntry(array, "{ \"v\":\"\u03bc\u00bf\"}", CTOBJECT_NOT_AN_OBJECT);
		CTArrayAddEntry(array, "{ \"v\":\"\u00B1\u00B6\"}", CTOBJECT_NOT_AN_OBJECT);
		CTArrayAddEntry(array, "{ \"a\":\"hp://foo\"}", CTOBJECT_NOT_AN_OBJECT);
		CTArrayAddEntry(array, "{ \"a\":null}", CTOBJECT_NOT_AN_OBJECT);
		CTArrayAddEntry(array, "{ \"a\":true}", CTOBJECT_NOT_AN_OBJECT);
		CTArrayAddEntry(array, "{ \"a\":\n \tfalse}", CTOBJECT_NOT_AN_OBJECT);
		CTArrayAddEntry(array, "{ \"a\" : true }", CTOBJECT_NOT_AN_OBJECT);
		CTArrayAddEntry(array, "{ \"a\" : 1.0e7 }", CTOBJECT_NOT_AN_OBJECT);
		CTArrayAddEntry(array, "{ \"a\" : 1.0 }", CTOBJECT_NOT_AN_OBJECT);
		CTArrayAddEntry(array, "{ \"a\" : 4095e-8 }", CTOBJECT_NOT_AN_OBJECT);
		CTArrayAddEntry(array, "{ \"v\":1.797693134E308}", CTOBJECT_NOT_AN_OBJECT);
		CTArrayAddEntry(array, "{\"menu\": {\"header\": \"SVG Viewer\",\"items\": [1.7, true, false, {\"id\": \"Open\"},{\"id\": \"OpenNew\", \"label\": \"Open New\"},null,{\"id\": \"ZoomIn\", \"label\": \"Zoom In\"},{\"id\": \"ZoomOut\", \"label\": \"Zoom Out\"},{\"id\": \"OriginalView\", \"label\": \"Original View\"},null,{\"id\": \"Quality\"},{\"id\": \"Pause\"},{\"id\": \"Mute\"},null,{\"id\": \"Find\", \"label\": \"Find...\"},{\"id\": \"FindAgain\", \"label\": \"Find Again\"},{\"id\": \"Copy\"},{\"id\": \"CopyAgain\", \"label\": \"Copy Again\"},{\"id\": \"CopySVG\", \"label\": \"Copy SVG\"},{\"id\": \"ViewSVG\", \"label\": \"View SVG\"},{\"id\": \"ViewSource\", \"label\": \"View Source\"},{\"id\": \"SaveAs\", \"label\": \"Save As\"},null,{\"id\": \"Help\"},{\"id\": \"About\", \"label\": \"About Adobe CVG Viewer...\"}]}}", CTOBJECT_NOT_AN_OBJECT);
		CTArrayAddEntry(array, "\"hello\"", CTOBJECT_NOT_AN_OBJECT);
		CTArrayAddEntry(array, "1.0e7", CTOBJECT_NOT_AN_OBJECT);
		CTArrayAddEntry(array, "1", CTOBJECT_NOT_AN_OBJECT);
		
		CTErrorRef error = NULL;
		for (int i = 0; i < array->count; ++i)
		{
			CTObjectRef dict = CTJSONParse(allocator, array->elements[i]->ptr, 0, &error);
			recurseJSON(CTObjectValue(dict), dict->type, 0);
			assert(!error);
			CTJSONSerialise(allocator, dict, 0);
			assert(!error);
		}
		CTArrayEmpty(array);
		
		CTArrayAddEntry(array, "", CTOBJECT_NOT_AN_OBJECT);
		CTArrayAddEntry(array, "{'X':'s", CTOBJECT_NOT_AN_OBJECT);
		CTArrayAddEntry(array, "{{\"k\":\"v\"}}", CTOBJECT_NOT_AN_OBJECT);
		CTArrayAddEntry(array, "{\"l\":[\"e\",\"]}", CTOBJECT_NOT_AN_OBJECT);
		CTArrayAddEntry(array, "1.3.3", CTOBJECT_NOT_AN_OBJECT);
		CTArrayAddEntry(array, "01", CTOBJECT_NOT_AN_OBJECT);
		for (int i = 0; i < array->count; ++i)
		{
			error = NULL;
			CTAllocatorRef allocll = CTAllocatorCreate();
			CTJSONParse(allocll, CTObjectValue(CTArrayObjectAtIndex(array, i)), 0, &error);
			assert(error);
			CTAllocatorRelease(allocll);
		}
#pragma mark - CTBencode Test Begin
		CTArrayEmpty(array);
		
		CTArrayAddEntry(array, "de", CTOBJECT_NOT_AN_OBJECT);
		CTArrayAddEntry(array, "li7483ee", CTOBJECT_NOT_AN_OBJECT);
		CTArrayAddEntry(array, "d4:yololllleeeleli720eeli-230eld4:hulli-233eeeeee", CTOBJECT_NOT_AN_OBJECT);
		CTArrayAddEntry(array, "d4:yoloi3ee", CTOBJECT_NOT_AN_OBJECT);
		CTArrayAddEntry(array, "i-3240.0e", CTOBJECT_NOT_AN_OBJECT);
		CTArrayAddEntry(array, "l0:e", CTOBJECT_NOT_AN_OBJECT);
		CTArrayAddEntry(array, "5:12345", CTOBJECT_NOT_AN_OBJECT);
		
		error = NULL;
		for (int i = 0; i < array->count; ++i)
		{
			CTObjectRef obj = CTBencodeParse(allocator, CTArrayObjectAtIndex(array, i)->ptr, &error);
			assert(!error);
			CTStringRef string = CTBencodeSerialise(allocator, obj, &error);
			puts(string->characters);
			assert(!error);
		}
		
		CTArrayEmpty(array);
		CTArrayAddEntry(array, "", CTOBJECT_NOT_AN_OBJECT);
		CTArrayAddEntry(array, "d", CTOBJECT_NOT_AN_OBJECT);
		CTArrayAddEntry(array, "l", CTOBJECT_NOT_AN_OBJECT);
		CTArrayAddEntry(array, "di0ee", CTOBJECT_NOT_AN_OBJECT);
		for (int i = 0; i < array->count; ++i)
		{
			error = NULL;
			CTBencodeParse(allocator, CTArrayObjectAtIndex(array, i)->ptr, &error);
			assert(error);
			fprintf(stderr, "%s\n", CTStringUTF8String(CTErrorGetErrorString(error)));
			CTErrorRelease(error);
		}
		
#pragma mark - CTBencode Test End
		
		CTArrayRelease(array);
		CTAllocatorRelease(allocator);
		
#pragma mark - CTAllocator Test End
		clock_values += clock() - t;
	}
	printf("%.0f µseconds\n", ((clock_values / (double)smoothing_factor) / (double)CLOCKS_PER_SEC) * 1e6);
    return 0;
}
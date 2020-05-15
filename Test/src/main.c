#include "STD_TYPES.h"
#include "test_dynamicAlloc.h"
#include "test_list.h"
#include <diag/Trace.h>


void main(void)
{
	trace_printf("--- Testing linked list ---\n");
	TEST_list();
	trace_printf("---------------------------\n\n");

	trace_printf("--- Testing dynamic allocation ---\n");
	TEST_dynamicAllocation();
	trace_printf("----------------------------------\n\n");

	while (1)
	{

	}
}

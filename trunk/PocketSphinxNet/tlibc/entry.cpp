#include "libct.h"
#include "stdlib.h"

void main()
{
	char* s = strdup("hello");

	if(s!=0)
	{
		free(s);
	}
}
#include <string.h>
#include <utils.h>

// (C) Arash Partow http://www.partow.net/programming/hashfunctions/
unsigned int APHash(const char* str, unsigned int len)
{
   unsigned int hash = 0xAAAAAAAA;
   unsigned int i    = 0;

   for(i = 0; i < len; str++, i++)
   {
      hash ^= ((i & 1) == 0) ? (  (hash <<  7) ^  (*str) * (hash >> 3)) :
                               (~((hash << 11) + ((*str) ^ (hash >> 5))));
   }

   return hash;
}

unsigned int ap_hash(const char *str)
{
    return APHash(str, strlen(str));
}


#if 0

#include <stdio.h>
#include <stdlib.h>

int main(int ac, char *av[])
{
    if (ac != 2)
    {
	fprintf(stderr, "Usage: %s string\n", av[0]);
	return EXIT_FAILURE;
    }

    unsigned int hash = ap_hash(av[1]);
    printf("0x%08X\n", hash);
    return EXIT_SUCCESS;
}

#endif


/*Cachelab assignment for CS-2011 B'17
 * Written by Surya Vadivazhagu (svadivazhagu) and
 * Daniel McDonough (dmcdonough) Team
 *
 */
#include "cachelab.h"

struct cacheStruct{
	int s; //
	int S;
	int E;
	int b;
	int B;
	int hits;
	int misses;
	int evictions;
};
int main()
{
    printSummary(0, 0, 0);
    return 0;
}

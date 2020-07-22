
#define FFS_TEST

#include <iostream>
#include "FFS.h"

void ffs_main() {
	std::cout << "ok" << std::endl;
	assert(1 == 1);
	FFS::suspendCurrentTask();
}

#pragma once


#if _DEBUG
	#pragma comment(lib, "libhdf5_D.lib")
#else
	#pragma comment(lib, "libhdf5.lib")
#endif

#include "hdf5.h"

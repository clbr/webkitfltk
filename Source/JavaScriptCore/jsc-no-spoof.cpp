#include "wtf/spoofing.h"
#include <stdio.h>

// This file is only for building the standalone jsc.

int (*spoofedTZ)() = NULL;

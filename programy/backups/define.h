// Macros and Defines for SESC simulation.
// Young Jin Yoon <youngjin@cs.columbia.edu>

#ifndef DEFINE_H_
#define DEFINE_H_

#ifdef SESC
#include "sescapi.h"
#endif
#define RAND(x,y) ((x) + (int) ((y) * (rand() / (RAND_MAX + 1.0))))
#define RAND_DOUBLE ((double)(rand() / (RAND_MAX + 1.0))) 
#define ALPHA 0.0001

#endif //DEFINE_H_

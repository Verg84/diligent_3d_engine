#include"structures.fxh"

cbuffer Constants {
    GlobalConstants g_Constants;
};

#ifndef THREAD_GROUP_SIZE
# define THREAD_GROUP_SIZE 64
#endif
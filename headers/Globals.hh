#ifndef GLOBALS_HH
#define GLOBALS_HH
#include "Player.hh"

extern Player *g_player;
extern float g_deltaT;
extern LARGE_INTEGER g_deltaStart;
extern LARGE_INTEGER g_deltaEnd;
extern LARGE_INTEGER g_performanceFrequency;

#endif
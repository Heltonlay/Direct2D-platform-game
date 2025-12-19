#include "Globals.hh"

Player *g_player{NULL};
float g_deltaT{0};
LARGE_INTEGER g_deltaStart;
LARGE_INTEGER g_deltaEnd;
LARGE_INTEGER g_performanceFrequency{};
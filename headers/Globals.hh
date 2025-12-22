#ifndef GLOBALS_HH
#define GLOBALS_HH
#include "Player.hh"
#include "Obstacle.hh"
#include <vector>

extern Player *g_player;
extern float g_deltaT;
extern LARGE_INTEGER g_deltaStart;
extern LARGE_INTEGER g_deltaEnd;
extern LARGE_INTEGER g_performanceFrequency;
extern std::vector<Obstacle> g_obstacles;

#endif
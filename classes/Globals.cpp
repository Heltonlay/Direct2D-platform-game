#include "Globals.hh"

Player *g_player{NULL};
float g_deltaT{0};
LARGE_INTEGER g_deltaStart{0};
LARGE_INTEGER g_deltaEnd{0};
LARGE_INTEGER g_performanceFrequency{};
std::vector<Obstacle> g_obstacles{};
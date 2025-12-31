#include "Globals.hh"

Player *g_player{NULL};
D2D1_POINT_2F g_cameraPos{};
float g_deltaT{0};
LARGE_INTEGER g_deltaStart{0};
LARGE_INTEGER g_deltaEnd{0};
LARGE_INTEGER g_performanceFrequency{};
std::vector<Obstacle> g_obstacles{};
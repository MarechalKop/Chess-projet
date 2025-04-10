#pragma once
#include "imgui.h"

enum class TimeOfDay { Day, Dusk, Night, Dawn };

extern TimeOfDay currentTime;
extern int currentTimeTurn;

TimeOfDay getNextTimeOfDay(TimeOfDay current);
ImVec4 getBackgroundColor(TimeOfDay time);

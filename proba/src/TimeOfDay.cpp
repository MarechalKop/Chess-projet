#include "TimeOfDay.hpp"
#include <random>
#include <vector>

TimeOfDay currentTime = TimeOfDay::Day;
int currentTimeTurn = -1;

TimeOfDay getNextTimeOfDay(TimeOfDay current) {
    static std::random_device rd;
    static std::mt19937 gen(rd());

    int index = static_cast<int>(current);
    const std::vector<std::vector<double>> timeOfDayMatrix = {
        {0.6, 0.4, 0.0, 0.0}, // Day
        {0.0, 0.4, 0.6, 0.0}, // Dusk
        {0.0, 0.0, 0.6, 0.4}, // Night
        {0.6, 0.0, 0.0, 0.4}  // Dawn
    };
    const auto& probs = timeOfDayMatrix[index];
    std::discrete_distribution<> dist(probs.begin(), probs.end());

    return static_cast<TimeOfDay>(dist(gen));
}

ImVec4 getBackgroundColor(TimeOfDay time) {
    switch (time) {
        case TimeOfDay::Day:   return ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
        case TimeOfDay::Dusk:  return ImVec4(0.9f, 0.5f, 0.3f, 1.0f);
        case TimeOfDay::Night: return ImVec4(0.1f, 0.1f, 0.2f, 1.0f);
        case TimeOfDay::Dawn:  return ImVec4(0.7f, 0.7f, 1.0f, 1.0f);
    }
    return ImVec4(0.5f, 0.5f, 0.5f, 1.0f);
}

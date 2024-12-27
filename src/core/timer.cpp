#include <core/timer.h>
#include <core/common.h>

Timer::Timer() { reset(); }

void Timer::reset() { start = std::chrono::system_clock::now(); }

double Timer::elapsed() const {
    auto now = std::chrono::system_clock::now();
    auto duration =
        std::chrono::duration_cast<std::chrono::milliseconds>(now - start);
    return static_cast<double>(duration.count());
}

std::string Timer::elapsed_string(bool precise) const {
    return time_string(elapsed(), precise);
}

double Timer::lap() {
    auto now = std::chrono::system_clock::now();
    auto duration =
        std::chrono::duration_cast<std::chrono::milliseconds>(now - start);
    start = now;
    return static_cast<double>(duration.count());
}

std::string Timer::lap_string(bool precise) {
    return time_string(lap(), precise);
}

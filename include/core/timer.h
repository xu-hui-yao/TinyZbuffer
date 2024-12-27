#pragma once

#include <chrono>

class Timer {
public:
    Timer();

    void reset();

    [[nodiscard]] double elapsed() const;

    [[nodiscard]] std::string elapsed_string(bool precise = false) const;

    double lap();

    std::string lap_string(bool precise = false);

private:
    std::chrono::system_clock::time_point start;
};

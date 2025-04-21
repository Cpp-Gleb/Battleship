#pragma once
#include <cstddef>

class Statistics {
public:
    Statistics();
    void RecordHit();
    void RecordMiss();
    double GetHitRatio() const;

private:
    std::size_t hits_;
    std::size_t misses_;
};
#include "Statistics.h"

Statistics::Statistics()
    : hits_(0), misses_(0)
{}

void Statistics::RecordHit() {
    hits_++;
}

void Statistics::RecordMiss() {
    misses_++;
}

double Statistics::GetHitRatio() const {
    if (hits_ + misses_ == 0) return 0.0;
    return static_cast<double>(hits_ / (hits_+misses_));
}

#pragma once
#include "DungeonState.h"
#include "Logger.h"

class Bot {
public:
    virtual ~Bot() = default;
    virtual void run(DungeonState& ds, Logger& logger) = 0;
};

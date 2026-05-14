#pragma once
#include "Bot.h"
#include "Pathfinder.h"

class AliceBot : public Bot {
public:
    void run(DungeonState& ds, Logger& logger) override;

private:
    int  chooseNextRoom(const DungeonState& ds);
    int  costToReach(const DungeonState& ds, int targetRoom);
    bool travelTo(DungeonState& ds, Logger& logger, int target);
    int  nextStepTo(const DungeonState& ds, int target);
    void collectBestInRoom(DungeonState& ds, Logger& logger);
    void returnHome(DungeonState& ds, Logger& logger);
    void collectAllAffordable(DungeonState& ds, Logger& logger, int stepsToHome);
};

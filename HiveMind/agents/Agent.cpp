#include "Agent.h"

Agent :: Agent(Position startPosition) : position(startPosition) , state(AgentState :: IDLE) ,
    batteryLevel(100) , maxBatteryLevel(100) , speed(1) , consume(1) {}
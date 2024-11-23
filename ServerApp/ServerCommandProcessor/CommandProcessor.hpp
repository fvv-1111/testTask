#pragma once

#include "GetResultCommand.hpp"
#include "GetStatusCommand.hpp"
#include "SetRangeCommand.hpp"
#include "GetRangeCommand.hpp"
#include "StartMeasureCommand.hpp"
#include "StopMeasureCommand.hpp"
#include "../../Utils/CommandProcessor/CommandProcessor.hpp"

namespace ServerApp
{

namespace CommandProcessor
{

using Processor = Utils::CommandProcessor::Processor<GetResultCommand,
                                                     GetStatusCommand,
                                                     SetRangeCommand,
                                                     GetRangeCommand,
                                                     StartMeasureCommand,
                                                     StopMeasureCommand>;
}

}

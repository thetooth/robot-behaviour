#pragma once

#include "../bt.hpp"
#include "spdlog/spdlog.h"

namespace BT
{
    class Start : public Decorator
    {
        Status update() override
        {
            spdlog::info("Start");

            if (hasChild())
            {
                return child->tick();
            }

            return Status::Failure;
        }
    };
} // namespace BT
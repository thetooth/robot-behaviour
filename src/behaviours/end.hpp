#pragma once

#include "../bt.hpp"
#include "spdlog/spdlog.h"

namespace BT
{
    class End : public Leaf
    {
        Status update() override
        {
            spdlog::info("End");
            return Node::Status::Success;
        }
    };
} // namespace BT
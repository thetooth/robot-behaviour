#pragma once

#include "../bt.hpp"
#include "spdlog/spdlog.h"

namespace BT
{
    class MoveTo : public Leaf
    {
        Status update() override
        {
            spdlog::info("MoveTo");
            return Node::Status::Success;
        }
    };
} // namespace BT
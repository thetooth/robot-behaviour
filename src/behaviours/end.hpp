#pragma once

#include "../bt.hpp"
#include "nlohmann/json.hpp"
#include "spdlog/spdlog.h"

namespace BT
{
    using json = nlohmann::json;

    class End : public Leaf
    {
      public:
        End(json data)
        {
            shouldContinue = data.value("continue", false);
        }

        Status update() override
        {
            spdlog::info("End");
            if (shouldContinue)
            {
                return Node::Status::Success;
            }

            return Node::Status::Failure;
        }

        bool shouldContinue;
    };
} // namespace BT
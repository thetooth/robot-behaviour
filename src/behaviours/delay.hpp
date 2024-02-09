#pragma once

#include <chrono>

#include <BrainTree.hpp>
#include <nlohmann/json.hpp>
#include <spdlog/spdlog.h>

namespace BT
{
    using namespace BrainTree;
    using namespace std::literals::chrono_literals;

    class Delay : public BrainTree::Decorator
    {
      public:
        Delay(json data)
        {
            delay = std::chrono::milliseconds(data.value("delay", 0));
        }

        void initialize() override
        {
            start = std::chrono::system_clock::now().time_since_epoch();
        }

        Status update() override
        {
            if (std::chrono::system_clock::now().time_since_epoch() - start >= delay)
            {
                status = child->tick();
            }
            else
            {
                status = Status::Running;
            }

            return status;
        }

      private:
        std::chrono::milliseconds delay;
        std::chrono::nanoseconds start;
    };
} // namespace BT
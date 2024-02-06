#pragma once

#include <BrainTree.hpp>
#include <spdlog/spdlog.h>

namespace BT
{
    using namespace BrainTree;
    class Start : public Decorator
    {
      public:
        Start([[maybe_unused]] json data, std::shared_ptr<BT::Manager> m) : manager(m)
        {
        }
        void initialize() override
        {
            // manager->sendStart();
        }
        Status update() override
        {
            spdlog::trace("Start");
            if (hasChild())
            {
                return child->tick();
            }

            return Status::Failure;
        }

      private:
        [[maybe_unused]] std::shared_ptr<BT::Manager> manager;
    };
} // namespace BT
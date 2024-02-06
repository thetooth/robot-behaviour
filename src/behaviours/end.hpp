#pragma once

#include <BrainTree.hpp>
#include <nlohmann/json.hpp>
#include <spdlog/spdlog.h>

namespace BT
{
    using namespace BrainTree;
    using json = nlohmann::json;

    class End : public Leaf
    {
      public:
        End(json data, std::shared_ptr<BT::Manager> m) : manager(m)
        {
            shouldContinue = data.value("continue", false);
        }

        //! @brief End the execution
        //!
        //! If the continue flag is set, returns success, otherwise sends a stop command to the controller.
        //!
        //! @return Node::Status
        Status update() override
        {
            spdlog::trace("End");
            if (shouldContinue)
            {
                return Node::Status::Success;
            }

            manager->stop();

            return Node::Status::Success;
        }

        void terminate([[maybe_unused]] Status s) override
        {
            spdlog::trace("End terminate");
        }

      private:
        std::shared_ptr<BT::Manager> manager;
        bool shouldContinue;
    };
} // namespace BT
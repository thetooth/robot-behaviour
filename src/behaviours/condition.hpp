#pragma once

#include <chrono>

#include <BrainTree.hpp>
#include <chaiscript/chaiscript.hpp>
#include <nlohmann/json.hpp>
#include <spdlog/spdlog.h>

namespace BT
{
    using namespace BrainTree;
    using namespace std::literals::chrono_literals;

    class Condition : public BrainTree::Decorator
    {
      public:
        Condition(json data, std::shared_ptr<BT::Manager> m) : manager(m)
        {
            expression = data["expression"].get<std::string>();
        }

        void initialize() override
        {
        }

        Status update() override
        {
            bool result = false;

            try
            {
                result = manager->chai->eval<bool>(expression);
            }
            catch (const chaiscript::exception::eval_error &e)
            {
                spdlog::error("Error evaluating expression: {}", e.pretty_print());
                return Status::Invalid;
            }
            catch (const std::exception &e)
            {
                spdlog::error("Error evaluating expression: {}", e.what());
                return Status::Invalid;
            }

            if (result)
            {
                return child->tick();
            }

            return Status::Failure;
        }

      private:
        std::string expression;
        std::shared_ptr<BT::Manager> manager;
    };
} // namespace BT
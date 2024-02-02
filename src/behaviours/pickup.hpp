#pragma once

#include <chrono>

#include <BrainTree.hpp>
#include <spdlog/spdlog.h>

namespace BT
{
    using namespace BrainTree;
    using namespace std::literals::chrono_literals;
    class PickUp : public Leaf
    {
      public:
        PickUp(json data, BT::Manager *m) : manager(m)
        {
            height = data["height"].get<double>();
        }
        void initialize() override
        {
            step = 0;
            origin = manager->status.pose;
            target = origin;
            target.z = height;
            spdlog::debug("PickUp {} {} {} {}", target.x, target.y, target.z, target.r);
            startTimestamp = std::chrono::system_clock::now().time_since_epoch();
            hasMoved = false;
        }
        Status update() override
        {
            if (manager->status.alarm ||
                std::chrono::system_clock::now().time_since_epoch() - startTimestamp > MOVE_TIMEOUT)
            {
                hasMoved = false;
                return Node::Status::Failure;
            }

            if (manager->status.run)
            {
                // spdlog::debug("PickUp {} {} {} {}", step, hasMoved, manager->inPosition(target), target.z);
                if (hasMoved && manager->inPosition(target))
                {
                    step++;
                    hasMoved = false;
                    target = origin;
                    if (step > 1)
                    {
                        return Node::Status::Success;
                    }
                    return Node::Status::Running;
                }
                else if (!hasMoved)
                {
                    switch (step)
                    {
                    case 0:
                        manager->sendMoveTo(target);
                        break;
                    case 1:
                        manager->sendMoveTo(origin);
                        break;

                    default:
                        break;
                    }
                    hasMoved = true;
                    return Node::Status::Running;
                }
                else
                {
                    return Node::Status::Running;
                }
            }

            return Node::Status::Invalid;
        }

      private:
        double height;
        int step;
        Model::IK::Pose origin;
        Model::IK::Pose target;
        BT::Manager *manager;
        bool hasMoved = false;
        std::chrono::nanoseconds startTimestamp;
        std::chrono::nanoseconds MOVE_TIMEOUT = 1s;
    };
} // namespace BT
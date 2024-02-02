#pragma once

#include <chrono>

#include <BrainTree.hpp>
#include <spdlog/spdlog.h>

namespace BT
{
    using namespace BrainTree;
    using namespace std::literals::chrono_literals;
    class MoveTo : public Leaf
    {
      public:
        MoveTo(json data, BT::Manager *m) : manager(m)
        {
            pose = data["pose"].get<Model::Pose>();
        }
        void initialize() override
        {
            spdlog::debug("MoveTo {} {} {} {}", pose.x, pose.y, pose.z, pose.r);
            startTimestamp = std::chrono::system_clock::now().time_since_epoch();
            hasMoved = false;
        }
        Status update() override
        {
            spdlog::trace("MoveTo {} {} {} {}", pose.x, pose.y, pose.z, pose.r);

            if (manager->status.alarm ||
                std::chrono::system_clock::now().time_since_epoch() - startTimestamp > MOVE_TIMEOUT)
            {
                hasMoved = false;
                return Node::Status::Failure;
            }

            if (manager->status.run)
            {
                if (inPosition())
                {
                    hasMoved = false;
                    return Node::Status::Success;
                }
                else if (!hasMoved)
                {
                    manager->sendMoveTo(pose);
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

        bool inPosition()
        {
            auto otg = manager->status.otg.result == 1;
            auto currentPose = manager->status.pose;
            if (compareAxis(currentPose.x, pose.x) && compareAxis(currentPose.y, pose.y) &&
                compareAxis(currentPose.z, pose.z) && compareAxis(currentPose.r, pose.r) && otg)
            {
                return true;
            }
            return false;
        }
        bool compareAxis(double a, double b, double threshold = 0.01)
        {
            return std::abs(a - b) < threshold;
        }

      private:
        Model::Pose pose;
        BT::Manager *manager;
        bool hasMoved = false;
        std::chrono::nanoseconds startTimestamp;
        std::chrono::nanoseconds MOVE_TIMEOUT = 1s;
    };
} // namespace BT
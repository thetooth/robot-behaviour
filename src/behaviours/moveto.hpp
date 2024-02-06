#pragma once

#include <BrainTree.hpp>
#include <spdlog/spdlog.h>

namespace BT
{
    using namespace BrainTree;

    class MoveTo : public Leaf
    {
      public:
        MoveTo(json data, std::shared_ptr<BT::Manager> m) : manager(m)
        {
            pose = data["pose"].get<Model::IK::Pose>();
        }

        void initialize() override
        {
            hasMoved = false;
        }

        //! @brief Move to a given pose
        //!
        //! If an alarm is set returns failure, otherwise returns success when the robot is in position,
        //! If the robot is not in position, command the controller to move to the given pose and return running.
        //!
        //! @return Node::Status
        Status update() override
        {
            if (manager->status.alarm)
            {
                hasMoved = false;
                return Node::Status::Failure;
            }

            if (manager->status.run)
            {
                if (manager->inPosition(pose))
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

      private:
        Model::IK::Pose pose;
        std::shared_ptr<BT::Manager> manager;
        bool hasMoved = false;
    };
} // namespace BT
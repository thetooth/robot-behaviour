#pragma once

#include <BrainTree.hpp>
#include <spdlog/spdlog.h>

namespace BT
{
    using namespace BrainTree;

    //! @brief Pick up an object
    //!
    //! Starting at the current position, move to the given height, then return to the original position.
    //!
    //! @param data
    //! @param m
    class PickUp : public Leaf
    {
      public:
        PickUp(json data, std::shared_ptr<BT::Manager> m) : manager(m)
        {
            height = data["height"].get<double>();
        }

        void initialize() override
        {
            step = 0;
            origin = manager->status.pose;
            target = origin;
            target.z = height;
            hasMoved = false;
        }

        Status update() override
        {
            if (manager->status.alarm)
            {
                hasMoved = false;
                return Node::Status::Failure;
            }

            if (manager->status.run)
            {
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
        std::shared_ptr<BT::Manager> manager;
        bool hasMoved = false;
    };
} // namespace BT
#include "behaviour.hpp"

void BT::Manager::sendNodeStatus()
{
    auto status = Model::Status{.id = getID(), .name = getName()};
    for (auto &[id, node] : execNodes)
    {
        status.nodes.push_back({.id = id, .status = int(node->getStatus())});
    }
    json j = status;
    publish("behaviour.status", j);
}

bool BT::Manager::inPosition(Model::IK::Pose pose)
{
    auto otg = status.otg.result == 1;
    auto currentPose = status.pose;
    auto compare = [](double a, double b, double threshold = 0.01) { return std::abs(a - b) < threshold; };
    if (compare(currentPose.x, pose.x) && compare(currentPose.y, pose.y) && compare(currentPose.z, pose.z) &&
        compare(currentPose.r, pose.r) && otg)
    {
        return true;
    }
    return false;
}
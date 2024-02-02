#include "behaviour.hpp"

void BT::Manager::sendNodeStatus()
{
    json j = json::array();
    for (auto &[id, node] : execNodes)
    {
        j.push_back({{"id", id}, {"status", node->getStatus()}});
    }
    publish("behaviour.status", j);
}

bool BT::Manager::inPosition(Model::Pose pose)
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
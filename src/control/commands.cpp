#include "nc.hpp"

bool Control::NC::sendStart()
{
    json j = R"({"command": "start"})"_json;

    return publish("motion.command", j);
}

bool Control::NC::sendStop()
{
    json j = R"({"command": "stop"})"_json;

    return publish("motion.command", j);
}

bool Control::NC::sendMoveTo(Model::IK::Pose pose)
{
    json j = R"({"command": "goto"})"_json;
    j["pose"] = pose;

    return publish("motion.command", j);
}

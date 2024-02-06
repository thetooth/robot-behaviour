#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include "nlohmann/json.hpp"

namespace Model
{
    using json = nlohmann::json;

    struct NodeStatus
    {
        std::string id;
        int status;
    };
    void to_json(json &j, const NodeStatus &p);

    struct Status
    {
        std::string id;
        std::string name;
        int64_t revision;
        bool run;
        std::vector<NodeStatus> nodes;
    };
    void to_json(json &j, const Status &p);

    namespace IK
    {
        enum Result
        {
            Success,
            JointLimit,
            Singularity,
        };
        std::string resultToString(Result result);

        struct Pose
        {
            double x, y, z, r;
            double alpha, beta, theta, phi;
            double alphaVelocity, betaVelocity;
            double thetaVelocity, phiVelocity;
        };
        void to_json(json &j, const Pose &p);
        void from_json(const json &j, Pose &p);
    } // namespace IK

    namespace Robot
    {
        struct OTGStatus
        {
            int result;
            IK::Result kinematicResult;
        };
        void from_json(const json &j, OTGStatus &p);

        struct EtherCATStatus
        {
            int64_t interval;
            int64_t sync0;
            int64_t compensation;
            int64_t integral;
        };
        void from_json(const json &j, EtherCATStatus &p);

        struct Status
        {
            bool run;
            bool alarm;
            bool needsHoming;
            std::string state;
            OTGStatus otg;
            EtherCATStatus ethercat;
            IK::Pose pose;
        };
        void from_json(const json &j, Status &p);
    } // namespace Robot
} // namespace Model
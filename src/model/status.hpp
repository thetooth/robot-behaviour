#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include "nlohmann/json.hpp"

namespace Model
{
    using json = nlohmann::json;

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

    struct OTGStatus
    {
        int result;
        Result kinematicResult;
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
        Pose pose;
    };
    void from_json(const json &j, Status &p);
} // namespace Model
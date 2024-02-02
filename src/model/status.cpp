#include "status.hpp"

void Model::from_json(const json &j, Pose &p)
{
    p.x = j.value("x", 0.0);
    p.y = j.value("y", 0.0);
    p.z = j.value("z", 0.0);
    p.r = j.value("r", 0.0);

    // p.alpha = j.at("alpha").get<double>();
    // p.beta = j.at("beta").get<double>();
    // p.theta = j.at("theta").get<double>();
    // p.phi = j.at("phi").get<double>();
    // p.alphaVelocity = j.at("alphaVelocity").get<double>();
    // p.betaVelocity = j.at("betaVelocity").get<double>();
    // p.thetaVelocity = j.at("thetaVelocity").get<double>();
    // p.phiVelocity = j.at("phiVelocity").get<double>();
}

void Model::to_json(json &j, const Pose &p)
{
    j = json{
        {"x", p.x},
        {"y", p.y},
        {"z", p.z},
        {"r", p.r},
        {"alpha", p.alpha},
        {"beta", p.beta},
        {"phi", p.phi},
        {"theta", p.theta},
        {"alphaVelocity", p.alphaVelocity},
        {"betaVelocity", p.betaVelocity},
        {"phiVelocity", p.phiVelocity},
        {"thetaVelocity", p.thetaVelocity},
    };
}

void Model::from_json(const json &j, OTGStatus &p)
{
    p.result = j.at("result").get<int>();
    p.kinematicResult = static_cast<Result>(j.at("kinematicResult").get<int>());
}

void Model::from_json(const json &j, EtherCATStatus &p)
{
    p.interval = j.at("interval").get<int64_t>();
    p.sync0 = j.at("sync0").get<int64_t>();
    p.compensation = j.at("compensation").get<int64_t>();
    p.integral = j.at("integral").get<int64_t>();
}

void Model::from_json(const json &j, Status &p)
{
    p.run = j.at("run").get<bool>();
    p.alarm = j.at("alarm").get<bool>();
    p.needsHoming = j.at("needsHoming").get<bool>();
    p.state = j.at("state").get<std::string>();
    p.otg = j.at("otg").get<OTGStatus>();
    p.ethercat = j.at("ethercat").get<EtherCATStatus>();
    p.pose = j.at("pose").get<Pose>();
}
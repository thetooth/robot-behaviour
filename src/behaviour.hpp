#pragma once

#include <map>
#include <mutex>
#include <string>
#include <vector>

#include <BrainTree.hpp>
#include <chaiscript/chaiscript.hpp>
#include <nlohmann/json.hpp>
#include <spdlog/spdlog.h>

#include "control/nc.hpp"
#include "model/behaviour.hpp"
#include "model/command.hpp"
#include "model/edge.hpp"
#include "model/node.hpp"
#include "model/status.hpp"
#include "model/type.hpp"

namespace BT
{
    using json = nlohmann::json;

    class Manager : public Control::NC, public std::enable_shared_from_this<Manager>
    {
      public:
        Manager(std::string url, std::shared_ptr<chaiscript::ChaiScript> runtime);
        ~Manager();

        int64_t getRevision() const;
        std::string getID() const;
        std::string getName() const;
        std::string getDescription() const;
        void load(std::string id);
        std::pair<int64_t, Model::Behaviour> getBehaviour(std::string id);
        BrainTree::BehaviorTree getExecutionTree(Model::Behaviour behaviour, int depth, std::string nestedPrefix = "");

        void receiveStatus(json payload);
        void receiveCommand(json payload);

        void start();
        void stop();
        void reset();
        bool inPosition(Model::IK::Pose pose);

        Model::Robot::Status status;
        Model::Behaviour behaviour;
        BrainTree::Blackboard blackboard;
        std::shared_ptr<chaiscript::ChaiScript> chai;

      private:
        void execute();
        void sendNodeStatus();

        int64_t revision = 0;
        BrainTree::BehaviorTree tree;
        std::map<std::string, std::shared_ptr<BrainTree::Node>> execNodes;
        std::mutex execMutex;
        bool ready = false;
        bool run = false;
        bool alarm = false;
        std::string lastFault = "No fault";
    };

} // namespace BT
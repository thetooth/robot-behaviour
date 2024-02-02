#pragma once

#include <map>
#include <mutex>
#include <string>
#include <vector>

#include <BrainTree.hpp>
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

    class Manager : public Control::NC
    {
      public:
        Manager(std::string url);
        ~Manager();

        std::string getID() const;
        std::string getName() const;
        std::string getDescription() const;
        void getBehaviour(std::string id);
        BrainTree::BehaviorTree getExecutionTree();
        std::map<std::string, Model::Node> getNodes();
        std::map<std::string, Model::Edge> getEdges();
        void receiveStatus(json payload);
        void receiveCommand(json payload);

        void start();
        void stop();
        bool inPosition(Model::IK::Pose pose);

        Model::Robot::Status status;
        Model::Behaviour behaviour;

      private:
        void execute();
        void sendNodeStatus();

        BrainTree::BehaviorTree tree;
        std::map<std::string, std::shared_ptr<BrainTree::Node>> execNodes;
        std::mutex execMutex;
        bool ready = false;
        bool run = false;
    };

} // namespace BT
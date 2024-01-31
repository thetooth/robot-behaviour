#pragma once

#include <map>
#include <string>
#include <vector>

#include <nlohmann/json.hpp>

#include "../bt.hpp"
#include "edge.hpp"
#include "node.hpp"
#include "type.hpp"

namespace Deserializer
{
    using json = nlohmann::json;

    struct Behaviour
    {
        std::string id;
        std::string name;
        std::string description;
        std::vector<Node> nodes;
        std::vector<Edge> edges;
    };
    void from_json(const json &j, Behaviour &b);

    class Loader
    {
      public:
        Loader(std::string payload);

        std::string getID();
        std::string getName();
        std::string getDescription();
        BT::BehaviorTree getExecutionTree();
        std::map<std::string, Node> getNodes();
        std::map<std::string, Edge> getEdges();

        Behaviour behaviour;
    };

} // namespace Deserializer
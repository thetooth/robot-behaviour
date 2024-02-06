#pragma once

#include <string>
#include <vector>

#include <nlohmann/json.hpp>

#include "edge.hpp"
#include "node.hpp"

namespace Model
{
    using json = nlohmann::json;
    struct Behaviour
    {
        std::map<std::string, Model::Node> getNodes();
        std::map<std::string, Model::Edge> getEdges();

        std::string id;
        std::string name;
        std::string description;
        std::vector<Model::Node> nodes;
        std::vector<Model::Edge> edges;
    };
    void from_json(const json &j, Behaviour &b);
} // namespace Model
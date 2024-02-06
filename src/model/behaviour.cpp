#include "behaviour.hpp"

std::map<std::string, Model::Node> Model::Behaviour::getNodes()
{
    std::map<std::string, Model::Node> map;
    for (auto &&node : nodes)
    {
        map[node.id] = node;
    }
    return map;
}

std::map<std::string, Model::Edge> Model::Behaviour::getEdges()
{
    std::map<std::string, Model::Edge> map;
    for (auto &&edge : edges)
    {
        map[edge.id] = edge;
    }
    return map;
}

void Model::from_json(const json &j, Behaviour &b)
{
    j.at("id").get_to(b.id);
    j.at("name").get_to(b.name);
    j.at("description").get_to(b.description);
    j.at("nodes").get_to(b.nodes);
    j.at("edges").get_to(b.edges);
}
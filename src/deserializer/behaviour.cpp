#include "behaviour.hpp"

void Deserializer::from_json(const json &j, Behaviour &b)
{
    j.at("id").get_to(b.id);
    j.at("name").get_to(b.name);
    j.at("description").get_to(b.description);
    j.at("nodes").get_to(b.nodes);
    j.at("edges").get_to(b.edges);
}

Deserializer::Loader::Loader(std::string payload)
{
    behaviour = json::parse(payload).get<Deserializer::Behaviour>();
}

std::string Deserializer::Loader::getID()
{
    return behaviour.id;
}

std::string Deserializer::Loader::getName()
{
    return behaviour.name;
}

std::string Deserializer::Loader::getDescription()
{
    return behaviour.description;
}

std::map<std::string, Deserializer::Node> Deserializer::Loader::getNodes()
{
    std::map<std::string, Deserializer::Node> nodes;
    for (auto &&node : behaviour.nodes)
    {
        nodes[node.id] = node;
    }
    return nodes;
}

std::map<std::string, Deserializer::Edge> Deserializer::Loader::getEdges()
{
    std::map<std::string, Deserializer::Edge> edges;
    for (auto &&edge : behaviour.edges)
    {
        edges[edge.id] = edge;
    }
    return edges;
}
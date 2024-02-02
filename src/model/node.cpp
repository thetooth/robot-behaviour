#include "node.hpp"

void Model::to_json(json &j, const Node &n)
{
    j = json{{"id", n.id}, {"type", n.type}, {"data", n.data}};
}

void Model::from_json(const json &j, Node &n)
{
    j.at("id").get_to(n.id);
    j.at("type").get_to(n.type);
    j.at("data").get_to(n.data);
}
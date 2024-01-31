#include "node.hpp"

void Deserializer::from_json(const json &j, Node &n)
{
    j.at("id").get_to(n.id);
    j.at("type").get_to(n.type);
    j.at("data").get_to(n.data);
}
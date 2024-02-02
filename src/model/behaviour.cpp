#include "behaviour.hpp"

void Model::from_json(const json &j, Behaviour &b)
{
    j.at("id").get_to(b.id);
    j.at("name").get_to(b.name);
    j.at("description").get_to(b.description);
    j.at("nodes").get_to(b.nodes);
    j.at("edges").get_to(b.edges);
}
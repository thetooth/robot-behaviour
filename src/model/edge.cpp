#include "edge.hpp"

void Model::from_json(const json &j, Edge &e)
{
    j.at("id").get_to(e.id);
    j.at("source").get_to(e.source);
    // j.at("sourceHandle").get_to(e.sourceHandle);
    e.sourceHandle = j.value("sourceHandle", "");
    j.at("target").get_to(e.target);
}
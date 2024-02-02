#pragma once

#include <string>
#include <vector>

#include <nlohmann/json.hpp>

namespace Model
{
    using json = nlohmann::json;

    struct Edge
    {
        std::string id;
        std::string source;
        std::string sourceHandle;
        std::string target;
    };
    void from_json(const json &j, Edge &e);
} // namespace Model
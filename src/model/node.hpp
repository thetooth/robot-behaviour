#pragma once

#include <string>
#include <vector>

#include <nlohmann/json.hpp>

#include "type.hpp"

namespace Model
{
    using json = nlohmann::json;

    class Node
    {
      public:
        std::string id;
        Type type;
        json data;
    };
    void to_json(json &j, const Node &n);
    void from_json(const json &j, Node &n);
} // namespace Model
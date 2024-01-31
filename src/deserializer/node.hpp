#pragma once

#include <string>
#include <vector>

#include <nlohmann/json.hpp>

#include "type.hpp"

namespace Deserializer
{
    using json = nlohmann::json;

    class Node
    {
      public:
        std::string id;
        Type type;
        json data;
    };
    void from_json(const json &j, Node &n);
} // namespace Deserializer
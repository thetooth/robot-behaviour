#pragma once

#include <string>
#include <vector>

#include <nlohmann/json.hpp>
#include <spdlog/spdlog.h>

namespace Deserializer
{
    namespace fmt = spdlog::fmt_lib;
    using json = nlohmann::json;

    enum class Type
    {
        Start,
        End,
        Selector,
        Sequence,
        Action,
        Condition,
        MoveTo,
    };
    void from_json(const json &j, Type &t);
} // namespace Deserializer
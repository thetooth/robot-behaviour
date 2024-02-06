#pragma once

#include <string>
#include <vector>

#include <nlohmann/json.hpp>
#include <spdlog/spdlog.h>

namespace Model
{
    namespace fmt = spdlog::fmt_lib;
    using json = nlohmann::json;

    enum class Type
    {
        Start,
        End,
        Nested,
        Selector,
        Sequence,
        Repeater,
        Condition,
        MoveTo,
        PickUp
    };
    void from_json(const json &j, Type &t);
} // namespace Model
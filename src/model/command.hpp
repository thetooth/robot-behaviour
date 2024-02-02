#pragma once

#include <string>

#include <nlohmann/json.hpp>

namespace Model
{
    using json = nlohmann::json;
    enum Command
    {
        Start,
        Stop,
        Reset,
        Load
    };
    void from_json(const json &j, Command &c);
} // namespace Model
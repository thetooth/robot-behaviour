#include "type.hpp"

void Deserializer::from_json(const json &j, Type &t)
{
    if (j == "start")
    {
        t = Type::Start;
    }
    else if (j == "end")
    {
        t = Type::End;
    }
    else if (j == "selector")
    {
        t = Type::Selector;
    }
    else if (j == "sequence")
    {
        t = Type::Sequence;
    }
    else if (j == "action")
    {
        t = Type::Action;
    }
    else if (j == "condition")
    {
        t = Type::Condition;
    }
    else if (j == "moveTo")
    {
        t = Type::MoveTo;
    }
    else
    {
        throw std::invalid_argument(fmt::format("Invalid node type: {}", j.get<std::string>()));
    }
}
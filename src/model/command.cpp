#include "command.hpp"

void Model::from_json(const json &j, Command &c)
{
    std::string command = j.get<std::string>();
    if (command == "start")
    {
        c = Command::Start;
    }
    else if (command == "stop")
    {
        c = Command::Stop;
    }
    else if (command == "reset")
    {
        c = Command::Reset;
    }
    else if (command == "load")
    {
        c = Command::Load;
    }
    else
    {
        throw std::invalid_argument("Invalid command");
    }
}
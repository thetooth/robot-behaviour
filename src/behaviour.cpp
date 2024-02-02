#include "behaviour.hpp"

BT::Manager::Manager(std::string url) : Control::NC(url)
{
}

BT::Manager::~Manager()
{
    execNodes.clear();
}

void BT::Manager::getBehaviour(std::string id)
{
    execMutex.lock();
    try
    {
        behaviour = Control::NC::getBehaviour(id).get<Model::Behaviour>();
        tree = getExecutionTree();
    }
    catch (std::exception &e)
    {
        spdlog::error("Error: {}", e.what());
        execMutex.unlock();
        return;
    }
    spdlog::info("Behaviour: {} {}", getName(), getID());
    spdlog::info("{}", getDescription());
    ready = true;
    execMutex.unlock();
}

std::string BT::Manager::getID() const
{
    return behaviour.id;
}

std::string BT::Manager::getName() const
{
    return behaviour.name;
}

std::string BT::Manager::getDescription() const
{
    return behaviour.description;
}

std::map<std::string, Model::Node> BT::Manager::getNodes()
{
    std::map<std::string, Model::Node> nodes;
    for (auto &&node : behaviour.nodes)
    {
        nodes[node.id] = node;
    }
    return nodes;
}

std::map<std::string, Model::Edge> BT::Manager::getEdges()
{
    std::map<std::string, Model::Edge> edges;
    for (auto &&edge : behaviour.edges)
    {
        edges[edge.id] = edge;
    }
    return edges;
}

void BT::Manager::receiveStatus(json payload)
{
    status = payload.get<Model::Robot::Status>();
}

void BT::Manager::receiveCommand(json payload)
{
    auto command = payload["command"].get<Model::Command>();
    switch (command)
    {
    case Model::Command::Start:
        start();
        break;
    case Model::Command::Stop:
        stop();
        break;
    case Model::Command::Reset:
        stop();
        for (auto &[id, node] : execNodes)
        {
            node->reset();
        }
        break;
    case Model::Command::Load: {
        auto id = payload.value("id", std::string(""));
        spdlog::debug("Load {}", id);
        if (!id.empty())
        {
            getBehaviour(payload["id"].get<std::string>());
        }
    }
    break;
    }
}

void BT::Manager::start()
{
    if (ready)
    {
        run = true;
        sendStart();
    }
}

void BT::Manager::stop()
{
    run = false;
    sendStop();
}
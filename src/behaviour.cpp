#include "behaviour.hpp"

BT::Manager::Manager(std::string url) : Control::NC(url)
{
}

BT::Manager::~Manager()
{
    execNodes.clear();
}

int64_t BT::Manager::getRevision() const
{
    return revision;
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

std::pair<int64_t, Model::Behaviour> BT::Manager::getBehaviour(std::string id)
{
    auto [r, b] = Control::NC::getBehaviour(id);
    return {r, b.get<Model::Behaviour>()};
}

void BT::Manager::load(std::string id)
{
    execMutex.lock();
    try
    {
        auto [r, b] = Control::NC::getBehaviour(id);
        revision = r;
        behaviour = b.get<Model::Behaviour>();
        tree = getExecutionTree(behaviour);
    }
    catch (std::exception &e)
    {
        spdlog::error("Error: {}", e.what());
        execMutex.unlock();
        return;
    }
    spdlog::info("Behaviour: {} {} rev {}", getName(), getID(), getRevision());
    spdlog::info("{}", getDescription());
    ready = true;
    execMutex.unlock();
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
        reset();
        break;
    case Model::Command::Load: {
        auto id = payload.value("id", std::string(""));
        spdlog::debug("Load {}", id);
        if (!id.empty())
        {
            load(id);
        }
    }
    break;
    }
}

void BT::Manager::start()
{
    if (ready)
    {
        run = sendStart();
    }
}

void BT::Manager::stop()
{
    run = false;
    sendStop();
}

void BT::Manager::reset()
{
    sendReset();
    for (auto &[id, node] : execNodes)
    {
        node->reset();
    }
}
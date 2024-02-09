#include "behaviour.hpp"

void BT::Manager::execute()
{
    static size_t counter = 0;

    if (counter++ % 10 == 0)
    {
        sendNodeStatus();
    }
    if (!ready || !run)
    {
        return;
    }

    execMutex.lock();
    auto status = tree.tick();

    execMutex.unlock();
    if (status == BrainTree::Node::Status::Failure)
    {
        alarm = true;
        lastFault = "Stopped due to failure";
        run = false;
        spdlog::warn(lastFault);
    }
}
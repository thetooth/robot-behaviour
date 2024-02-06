#include <future>
#include <map>
#include <sanitizer/lsan_interface.h>
#include <signal.h>

#include <BrainTree.hpp>
#include <nlohmann/json.hpp>
#include <spdlog/spdlog.h>

#include "behaviour.hpp"

std::promise<void> exitSignal;

void abort_handler([[maybe_unused]] int signum)
{
    printf("\n");
    spdlog::warn("Received SIGINT, exiting...");
    exitSignal.set_value();
}

int main()
{
    spdlog::set_level(spdlog::level::debug);
    spdlog::set_pattern("%^[%=8l]%$ %s:%# %v");

    signal(SIGINT, abort_handler);

    std::shared_ptr<BT::Manager> b;
    try
    {
        spdlog::info("Starting...");
        b = std::make_shared<BT::Manager>("nats://192.168.0.107:4222");
    }
    catch (const std::exception &e)
    {
        spdlog::critical("{}", e.what());
        return 1;
    }

    spdlog::info("Ready for execution");
    exitSignal.get_future().wait();

#ifdef WITH_ADDR_SANITIZE
    __lsan_do_leak_check();
#endif

    return 0;
}
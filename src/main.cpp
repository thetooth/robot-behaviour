#include <map>

#include "bt.hpp"
#include "deserializer/behaviour.hpp"
#include "nats.h"
#include "nlohmann/json.hpp"
#include "spdlog/spdlog.h"

#include "behaviours/end.hpp"
#include "behaviours/moveTo.hpp"
#include "behaviours/start.hpp"

using namespace BT;
using json = nlohmann::json;

int main()
{
    spdlog::set_level(spdlog::level::debug);
    spdlog::set_pattern("%^[%=8l]%$ %s:%# %v");

    // Communications
    natsConnection *nc = nullptr;

    auto ncStatus = natsConnection_ConnectTo(&nc, "nats://192.168.0.107:4222");
    if (ncStatus != NATS_OK)
    {
        spdlog::critical("Failed to connect to NATS server");
        return 1;
    }
    jsCtx *js = nullptr;
    // jsOptions jsOpts;
    auto jsStatus = natsConnection_JetStream(&js, nc, NULL);
    if (jsStatus != NATS_OK)
    {
        spdlog::critical("Failed to get JetStream context");
        return 1;
    }
    kvStore *kv = nullptr;
    jsStatus = js_KeyValue(&kv, js, "behaviour");
    if (jsStatus != NATS_OK)
    {
        spdlog::critical("Failed to get JetStream key-value store");
        return 1;
    }
    kvEntry *res = nullptr;
    jsStatus = kvStore_Get(&res, kv, "data.944aa6c2-5609-46b6-9821-df9d5a471432");
    if (jsStatus != NATS_OK)
    {
        spdlog::critical("Failed to get JetStream key-value store entries");
        return 1;
    }

    // spdlog::info("Got key-value store entry: {}", kvEntry_ValueString(res));

    auto b = Deserializer::Loader(kvEntry_ValueString(res));
    spdlog::info("Behaviour: {} {}", b.getName(), b.getID());
    spdlog::info("{}", b.getDescription());
    auto tree = b.getExecutionTree();

    tree.tick();

    return 0;
}
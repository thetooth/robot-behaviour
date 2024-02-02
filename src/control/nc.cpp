#include "nc.hpp"

#include <spdlog/spdlog.h>

Control::NC::NC(std::string url)
{
    ncPtr = this;
    natsOptions *opts = nullptr;
    natsOptions_Create(&opts);
    natsOptions_SetURL(opts, url.c_str());
    auto ncStatus = natsConnection_Connect(&nc, opts);
    if (ncStatus != NATS_OK)
    {
        natsOptions_Destroy(opts);
        throw std::runtime_error("Failed to connect to NATS server");
        return;
    }
    natsOptions_Destroy(opts);

    auto commandSubStatus = natsConnection_Subscribe(&commandSub, nc, "behaviour.command", commandCbWrapper, this);
    if (commandSubStatus != NATS_OK)
    {
        throw std::runtime_error("Failed to subscribe to command topic");
        return;
    }
    auto statusSubStatus = natsConnection_Subscribe(&statusSub, nc, "motion.status", statusCbWrapper, this);
    if (statusSubStatus != NATS_OK)
    {
        throw std::runtime_error("Failed to subscribe to status topic");
        return;
    }

    auto jsStatus = natsConnection_JetStream(&js, nc, NULL);
    if (jsStatus != NATS_OK)
    {
        throw std::runtime_error("Failed to get JetStream context");
        return;
    }
    jsStatus = js_KeyValue(&kv, js, "behaviour");
    if (jsStatus != NATS_OK)
    {
        throw std::runtime_error("Failed to get behaviour key-value store");
        return;
    }
}

Control::NC::~NC()
{
    natsSubscription_Destroy(commandSub);
    natsSubscription_Destroy(statusSub);
    jsCtx_Destroy(js);
    natsConnection_Destroy(nc);
    nats_Close();
}

bool Control::NC::publish(std::string subject, json message)
{
    auto data = message.dump();
    auto status = natsConnection_Publish(nc, subject.c_str(), data.c_str(), data.length());
    if (status != NATS_OK)
    {
        spdlog::error("Failed to publish message");
        return false;
    }
    return true;
}

nlohmann::json Control::NC::getBehaviour(std::string id)
{
    kvEntry *res = nullptr;
    auto jsStatus = kvStore_Get(&res, kv, ("data." + id).c_str());
    if (jsStatus != NATS_OK)
    {
        spdlog::critical("Failed to get JetStream key-value store entries");
        return json();
    }
    auto payload = json::parse(kvEntry_ValueString(res));
    kvEntry_Destroy(res);
    return payload;
}
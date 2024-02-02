#pragma once

#include <nats.h>
#include <nlohmann/json.hpp>
#include <spdlog/spdlog.h>

#include "../model/status.hpp"

namespace Control
{
    using json = nlohmann::json;

    class NC
    {
      public:
        NC(std::string url);
        virtual ~NC();

        bool publish(std::string subject, json message);

        virtual void receiveStatus(json payload) = 0;
        virtual void receiveCommand(json payload) = 0;
        virtual void execute() = 0;
        virtual void sendNodeStatus() = 0;

        bool sendStart();
        bool sendStop();
        bool sendMoveTo(Model::Pose pose);
        json getBehaviour(std::string id);

      private:
        natsConnection *nc = nullptr;
        natsSubscription *commandSub = nullptr;
        natsSubscription *statusSub = nullptr;
        jsCtx *js = nullptr;
        kvStore *kv = nullptr;
    };

    static NC *ncPtr = nullptr;

    inline void commandCbWrapper([[maybe_unused]] natsConnection *nc, [[maybe_unused]] natsSubscription *sub,
                                 natsMsg *msg, [[maybe_unused]] void *closure)
    {
        if (ncPtr != nullptr)
        {
            const json data = json::parse(natsMsg_GetData(msg));
            natsMsg_Destroy(msg);
            ncPtr->receiveCommand(data);
        }
    }

    inline void statusCbWrapper([[maybe_unused]] natsConnection *nc, [[maybe_unused]] natsSubscription *sub,
                                natsMsg *msg, [[maybe_unused]] void *closure)
    {
        if (ncPtr != nullptr)
        {
            const json data = json::parse(natsMsg_GetData(msg));
            natsMsg_Destroy(msg);
            ncPtr->receiveStatus(data);
            ncPtr->execute();
        }
    }
} // namespace Control
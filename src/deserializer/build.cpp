#include "behaviour.hpp"

#include "../behaviours/end.hpp"
#include "../behaviours/moveTo.hpp"
#include "../behaviours/start.hpp"

BT::BehaviorTree Deserializer::Loader::getExecutionTree()
{
    auto nodes = getNodes();
    auto edges = getEdges();

    auto execNodes = std::map<std::string, std::shared_ptr<BT::Node>>();

    BT::BehaviorTree bt;

    for (auto &[id, node] : nodes)
    {
        spdlog::debug("Node: {} type: {} data: {}", node.id, int(node.type), node.data.dump());
        switch (node.type)
        {
        case Deserializer::Type::Start:
            execNodes[id] = std::make_shared<BT::Start>();
            bt.setRoot(execNodes[id]);
            break;
        case Deserializer::Type::End:
            execNodes[id] = std::make_shared<BT::End>(node.data);
            break;
        case Deserializer::Type::Selector:
            execNodes[id] = std::make_shared<BT::Selector>();
            break;
        case Deserializer::Type::Sequence:
            execNodes[id] = std::make_shared<BT::Sequence>();
            break;
        case Deserializer::Type::MoveTo:
            execNodes[id] = std::make_shared<BT::MoveTo>();
            break;
        default:
            spdlog::critical("Unknown node type: {}", int(node.type));
            break;
        }
    }

    for (auto &[id, edge] : edges)
    {
        spdlog::debug("Edge: {} source: {} sourceHandle: {} target: {}", edge.id, edge.source, edge.sourceHandle,
                      edge.target);

        auto &source = nodes[edge.source];
        auto &target = nodes[edge.target];

        switch (source.type)
        {
        case Deserializer::Type::Start: {
            auto n = (BT::Start *)(execNodes[source.id].get());
            n->setChild(execNodes[target.id]);
        }
        break;
        case Deserializer::Type::Selector: {
            auto n = (BT::Selector *)(execNodes[source.id].get());
            n->addChild(execNodes[target.id]);
        }
        break;
        case Deserializer::Type::Sequence: {
            auto n = (BT::Sequence *)(execNodes[source.id].get());
            n->addChild(execNodes[target.id]);
        }
        break;
        default:
            spdlog::critical("Unknown node type: {}", int(source.type));
            break;
        }
    }

    return bt;
}
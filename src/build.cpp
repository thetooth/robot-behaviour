#include "behaviour.hpp"

#include "behaviours/end.hpp"
#include "behaviours/moveto.hpp"
#include "behaviours/pickup.hpp"
#include "behaviours/start.hpp"

BT::BehaviorTree BT::Manager::getExecutionTree()
{
    auto nodes = getNodes();
    auto edges = getEdges();

    execNodes = std::map<std::string, std::shared_ptr<BT::Node>>();

    BT::BehaviorTree bt;

    for (auto &[id, node] : nodes)
    {
        spdlog::debug("Node: {} type: {} data: {}", node.id, int(node.type), node.data.dump());
        switch (node.type)
        {
        case Model::Type::Start:
            execNodes[id] = std::make_shared<BT::Start>(node.data, this);
            bt.setRoot(execNodes[id]);
            break;
        case Model::Type::End:
            execNodes[id] = std::make_shared<BT::End>(node.data, this);
            break;
        case Model::Type::Selector:
            execNodes[id] = std::make_shared<BT::Selector>();
            break;
        case Model::Type::Sequence:
            execNodes[id] = std::make_shared<BT::Sequence>();
            break;
        case Model::Type::MoveTo:
            spdlog::debug("MoveTo {}", node.data.dump());
            execNodes[id] = std::make_shared<BT::MoveTo>(node.data, this);
            break;
        case Model::Type::PickUp:
            spdlog::debug("PickUp {}", node.data.dump());
            execNodes[id] = std::make_shared<BT::PickUp>(node.data, this);
            break;
        default:
            spdlog::critical("Unknown node type: {}", int(node.type));
            break;
        }
        // execNodes[id]->setBlackboard(bt.blackboard);
    }

    for (auto &[id, edge] : edges)
    {
        spdlog::debug("Edge: {} source: {} sourceHandle: {} target: {}", edge.id, edge.source, edge.sourceHandle,
                      edge.target);

        auto &source = nodes[edge.source];
        auto &target = nodes[edge.target];

        switch (source.type)
        {
        case Model::Type::Start: {
            auto n = (BT::Start *)(execNodes[source.id].get());
            n->setChild(execNodes[target.id]);
        }
        break;
        case Model::Type::Selector: {
            auto n = (BT::Selector *)(execNodes[source.id].get());
            n->addChild(execNodes[target.id]);
        }
        break;
        case Model::Type::Sequence: {
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
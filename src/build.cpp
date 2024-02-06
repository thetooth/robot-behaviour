#include "behaviour.hpp"

#include "behaviours/end.hpp"
#include "behaviours/moveto.hpp"
#include "behaviours/pickup.hpp"
#include "behaviours/start.hpp"

//! @brief Gets a braintree behaviour tree ready for execution
//! @return A braintree behaviour tree
BT::BehaviorTree BT::Manager::getExecutionTree(Model::Behaviour b)
{
    auto nodes = b.getNodes();
    auto edges = b.getEdges();

    execNodes = std::map<std::string, std::shared_ptr<BT::Node>>();

    BT::BehaviorTree bt;

    for (auto &[id, node] : nodes)
    {
        spdlog::debug("Node: {} type: {} data: {}", node.id, int(node.type), node.data.dump());
        switch (node.type)
        {
        case Model::Type::Start:
            execNodes[id] = std::make_shared<BT::Start>(node.data, shared_from_this());
            bt.setRoot(execNodes[id]);
            break;
        case Model::Type::End:
            execNodes[id] = std::make_shared<BT::End>(node.data, shared_from_this());
            break;
        case Model::Type::Selector:
            execNodes[id] = std::make_shared<BT::Selector>();
            break;
        case Model::Type::Sequence:
            execNodes[id] = std::make_shared<BT::StatefulSequence>();
            break;
        case Model::Type::Repeater: {
            const auto count = node.data["count"].get<int>();
            execNodes[id] = std::make_shared<BT::Repeater>(count);
        }
        break;
        case Model::Type::Nested: {
            const auto nid = node.data["id"].get<std::string>();
            auto [r, b] = getBehaviour(nid);
            spdlog::debug("Nested: {} {} rev {}", b.name, b.id, r);
            execNodes[id] = std::make_shared<BrainTree::BehaviorTree>(getExecutionTree(b));
        }
        break;
        case Model::Type::MoveTo:
            spdlog::debug("MoveTo {}", node.data.dump());
            execNodes[id] = std::make_shared<BT::MoveTo>(node.data, shared_from_this());
            break;
        case Model::Type::PickUp:
            spdlog::debug("PickUp {}", node.data.dump());
            execNodes[id] = std::make_shared<BT::PickUp>(node.data, shared_from_this());
            break;
        default:
            throw std::invalid_argument(fmt::format("Unknown node type: {}", int(node.type)));
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
        case Model::Type::Repeater: {
            auto n = (BT::Repeater *)(execNodes[source.id].get());
            n->setChild(execNodes[target.id]);
        }
        break;
        default:
            throw std::invalid_argument(fmt::format("Unknown node type: {}", int(source.type)));
            break;
        }
    }

    return bt;
}
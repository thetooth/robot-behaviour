#include "behaviour.hpp"

#include "behaviours/delay.hpp"
#include "behaviours/end.hpp"
#include "behaviours/moveto.hpp"
#include "behaviours/pickup.hpp"
#include "behaviours/start.hpp"

//! @brief Gets a braintree behaviour tree ready for execution
//! @return A braintree behaviour tree
BT::BehaviorTree BT::Manager::getExecutionTree(Model::Behaviour b, int depth, std::string nestedPrefix)
{
    if (depth > 10)
    {
        throw std::runtime_error("Nested depth greater than 10, possible circular reference detected.");
    }

    auto nodes = b.getNodes();
    auto edges = b.getEdges();

    BT::BehaviorTree bt;

    for (auto &[oid, node] : nodes)
    {
        auto id = nestedPrefix + oid;
        spdlog::debug("ExecID: {} Node: {} type: {} data: {}", id, node.id, int(node.type), node.data.dump());
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
        case Model::Type::Delay:
            execNodes[id] = std::make_shared<BT::Delay>(node.data);
            break;
        case Model::Type::Nested: {
            const auto nid = node.data.value("id", std::string());
            if (nid.empty())
            {
                throw std::invalid_argument("Nested node id is empty");
            }
            auto [r, b] = getBehaviour(nid);
            execNodes[id] = std::make_shared<BrainTree::BehaviorTree>(getExecutionTree(b, ++depth, id + "-"));
        }
        break;
        case Model::Type::MoveTo:
            execNodes[id] = std::make_shared<BT::MoveTo>(node.data, shared_from_this());
            break;
        case Model::Type::PickUp:
            execNodes[id] = std::make_shared<BT::PickUp>(node.data, shared_from_this());
            break;
        default:
            throw std::invalid_argument(fmt::format("Unknown node type: {}", int(node.type)));
            break;
        }
    }

    for (auto &[id, edge] : edges)
    {

        auto &source = nodes[edge.source];
        auto &target = nodes[edge.target];

        auto sid = nestedPrefix + source.id;
        auto tid = nestedPrefix + target.id;

        spdlog::debug("Edge: {} source: {} sourceHandle: {} target: {}", nestedPrefix + edge.id, sid, edge.sourceHandle,
                      tid);

        switch (source.type)
        {
        case Model::Type::Start: {
            auto n = (BT::Start *)(execNodes[sid].get());
            n->setChild(execNodes[tid]);
        }
        break;
        case Model::Type::Selector: {
            auto n = (BT::Selector *)(execNodes[sid].get());
            n->addChild(execNodes[tid]);
        }
        break;
        case Model::Type::Sequence: {
            auto n = (BT::Sequence *)(execNodes[sid].get());
            n->addChild(execNodes[tid]);
        }
        break;
        case Model::Type::Repeater: {
            auto n = (BT::Repeater *)(execNodes[sid].get());
            n->setChild(execNodes[tid]);
        }
        break;
        case Model::Type::Delay: {
            auto n = (BT::Delay *)(execNodes[sid].get());
            n->setChild(execNodes[tid]);
        }
        break;
        default:
            throw std::invalid_argument(fmt::format("Unknown node type: {}", int(source.type)));
            break;
        }
    }

    return bt;
}
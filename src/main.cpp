#include <map>

#include "bt.hpp"
#include "nlohmann/json.hpp"
#include "spdlog/spdlog.h"

#include "behaviours/end.hpp"
#include "behaviours/start.hpp"

using namespace BT;
using json = nlohmann::json;

enum class Type
{
    Start,
    End,
    Selector,
    Sequence,
    Action,
    Condition
};

struct StoredNode
{
    std::string id;
    Type type;
    json data;
};

struct StoredEdge
{
    std::string id;
    std::string source;
    std::string sourceHandle;
    std::string target;
};

int main()
{
    auto nodes = std::map<std::string, StoredNode>();
    auto edges = std::vector<StoredEdge>();

    nodes["1"] = StoredNode{"1", Type::Start, {}};
    nodes["2"] = StoredNode{"2", Type::Sequence, {}};
    nodes["3"] = StoredNode{"3", Type::End, {}};
    edges.push_back(StoredEdge{"1", "1", "", "2"});
    edges.push_back(StoredEdge{"2", "2", "1", "3"});
    edges.push_back(StoredEdge{"3", "2", "2", "3"});
    // edges.push_back(StoredEdge{"4", "3", "", ""});

    auto execNodes = std::map<std::string, std::shared_ptr<Node>>();

    BehaviorTree bt;

    for (auto &[id, node] : nodes)
    {
        switch (node.type)
        {
        case Type::Start:
            execNodes[id] = std::make_shared<Start>();
            bt.setRoot(execNodes[id]);
            break;
        case Type::End:
            execNodes[id] = std::make_shared<End>();
            break;
        case Type::Selector:
            execNodes[id] = std::make_shared<Selector>();
            break;
        case Type::Sequence:
            execNodes[id] = std::make_shared<Sequence>();
            break;
        default:
            spdlog::critical("Unknown node type: {}", int(node.type));
            break;
        }
    }

    for (auto &edge : edges)
    {
        auto &source = nodes[edge.source];
        auto &target = nodes[edge.target];

        switch (source.type)
        {
        case Type::Start: {
            auto n = (Start *)(execNodes[source.id].get());
            n->setChild(execNodes[target.id]);
        }
        break;
        case Type::Selector: {
            auto n = (Selector *)(execNodes[source.id].get());
            n->addChild(execNodes[target.id]);
        }
        break;
        case Type::Sequence: {
            auto n = (Sequence *)(execNodes[source.id].get());
            n->addChild(execNodes[target.id]);
        }
        break;

        default:
            break;
        }
    }

    bt.tick();

    return 0;
}
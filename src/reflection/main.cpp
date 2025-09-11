#include "reflection.h"

#include <fmt/ostream.h>
#include <spdlog/spdlog.h>

#include <array>
#include <stacktrace>
#include <vector>

class Node
{
    std::string name;
    std::vector<std::shared_ptr<Node>> children;

public:
    Node() = default;
    virtual ~Node() = default;

    Node(std::string name) : name(std::move(name)) {}
    void AddChild(std::shared_ptr<Node> node)
    {
        children.push_back(std::move(node));
    }

    const std::string &GetName() const { return name; }
    void SetName(std::string name) { this->name = std::move(name); }
};

class MeshNode : public Node
{
    std::vector<std::array<float, 3>> vertices;
    std::vector<std::array<unsigned, 3>> triangles;

public:
    MeshNode() = default;

    MeshNode(std::string name) : Node(std::move(name)) {}
};

int main()
{
    using namespace refl;
    std::set_terminate([]() {
        std::exception_ptr ex = std::current_exception();
        if (ex) {
            try {
                std::rethrow_exception(ex);
            } catch (const std::exception &e) {
                spdlog::critical("exception: {}", e.what());
                spdlog::critical("{}",
                                 fmt::streamed(std::stacktrace::current()));
            }
        }
        spdlog::shutdown();
    });

    auto &reg = *Registry::Get();

    reg.AddClass<Node>("Node") //
        .SupportDefaultConstructor()
        .AddMethod("GetName", &Node::GetName)
        .AddMethod("SetName", &Node::SetName)
        .AddMethod("AddChild", &Node::AddChild)
        /**/;

    reg.AddClass<MeshNode>("MeshNode").SupportDefaultConstructor();

    auto node1 = reg.GetClass("Node").Create<Node>();
    auto node2 = reg.GetClass("MeshNode").Create<MeshNode>();

    spdlog::info("node1: {}", typeid(*node1).name());
    spdlog::info("node2: {}", typeid(*node2).name());

    return 0;
}
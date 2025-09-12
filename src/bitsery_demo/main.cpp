#include "Derived.h"

#include <spdlog/spdlog.h>

#include <bitsery/adapter/stream.h>
#include <bitsery/bitsery.h>
#include <bitsery/brief_syntax.h>
#include <bitsery/ext/inheritance.h>
#include <bitsery/ext/pointer.h>
#include <bitsery/ext/std_smart_ptr.h>
#include <bitsery/traits/vector.h>

#include <sstream>

namespace bitsery
{
namespace ext
{

// for each base class define DIRECTLY derived classes
// e.g. PolymorphicBaseClass<Shape> : PolymorphicDerivedClasses<Circle,
// Rectangle, RoundedRectangle>
//  is incorrect, because RoundedRectangle does not directly derive from Shape
template <>
struct PolymorphicBaseClass<BaseNode> : PolymorphicDerivedClasses<DerivedNode> {
};

} // namespace ext
} // namespace bitsery

void test_shared_ptr()
{
    std::shared_ptr<BaseNode> node1 = std::make_shared<BaseNode>("Node1");
    std::shared_ptr<BaseNode> node2 =
        std::make_shared<DerivedNode>("Node2", std::array<float, 3>{2, 2, 2});
    node1->AddChild(node2);

    SPDLOG_INFO("node1: {}", *node1);

    using MyPolymorphicClassesForRegistering =
        bitsery::ext::PolymorphicClassesList<BaseNode>;
    using TContext = std::tuple<
        bitsery::ext::PointerLinkingContext,
        bitsery::ext::PolymorphicContext<bitsery::ext::StandardRTTI>>;
    using MySerializer =
        bitsery::Serializer<bitsery::OutputBufferedStreamAdapter, TContext>;
    TContext ctx;
    std::get<1>(ctx).registerBasesList<MySerializer>(
        MyPolymorphicClassesForRegistering{});

    std::stringstream ss;
    MySerializer ser{ctx, ss};

    ser.ext(node1, bitsery::ext::StdSmartPtr{});
    ser.adapter().flush();

    spdlog::info("serialized result:\n{}", ss.str());

    // node1.reset();
    // bitsery::quickDeserialization(bitsery::InputStreamAdapter{ss}, node1);
    // SPDLOG_INFO("node1: {}", *node1);
}

namespace fs = std::filesystem;
int main()
{
    spdlog::set_level(spdlog::level::debug);
    constexpr bool is_abstract = std::is_abstract_v<DerivedNode>;

    test_shared_ptr();

    return 0;
}
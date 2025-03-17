#include "../../Libraries/catch/catch_amalgamated.hpp"

#include "../../Libraries/nlohmann/json.hpp"
#include "../../Scene/Components/ComponentIncludes.h"

TEST_CASE("TransformComponent serialization")
{
    std::shared_ptr<TransformComponent> transformComponent = std::make_shared<TransformComponent>();
    nlohmann::json transformComponentJson = std::vector<int>(5, 5);
    REQUIRE(false);
}
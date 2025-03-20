#include "../../Libraries/catch/catch_amalgamated.hpp"
#include "../../Serialization/Serializers.h"

TEST_CASE("Object Serialization")
{
    std::shared_ptr<Object> myObject = std::make_shared<Object>("objectName");

    // Serialization
    nlohmann::json myObjectJson = myObject;
    std::string objectName = myObjectJson.at("m_name");
    REQUIRE(objectName == "objectName");

    // Deserialization
    std::shared_ptr<Object> myObjectPostJson = myObjectJson.get<std::shared_ptr<Object>>();
    REQUIRE(myObjectPostJson->GetName() == "objectName");
}

TEST_CASE("SceneNode Serialization")
{
    std::shared_ptr<SceneNode> mySceneNode = std::make_shared<SceneNode>("sceneNodeName");

    std::shared_ptr<SceneNode> child1 = std::make_shared<SceneNode>("child1Name");
    std::shared_ptr<SceneNode> child2 = std::make_shared<SceneNode>("child2Name");

    mySceneNode->AddChild(child1);
    mySceneNode->AddChild(child2);
    
    // Serialization
    nlohmann::json mySceneNodeJson = mySceneNode;
    std::string sceneNodeName = mySceneNodeJson.at("m_name");
    REQUIRE(sceneNodeName == "sceneNodeName");
    std::vector<std::shared_ptr<SceneNode>> children = mySceneNodeJson.at("m_childSceneNodes");
    REQUIRE(children.size() == 2);
    REQUIRE(children.at(0)->GetName() == "child1Name");
    REQUIRE(children.at(1)->GetName() == "child2Name");

    // Deserialization
    std::shared_ptr<SceneNode> mySceneNodePostJson = mySceneNodeJson.get<std::shared_ptr<SceneNode>>();
    REQUIRE(mySceneNodePostJson->GetName() == "sceneNodeName");
    REQUIRE(mySceneNodePostJson->GetChildren().size() == 2);
}
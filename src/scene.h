#include "Entity.h"

#include <vector>

class SceneNode
{
    std::unique_ptr<Entity> m_Entity;
    std::vector<SceneNode*> m_Children;
public:


};

class Scene
{
    SceneNode m_RootNode;
public:

};


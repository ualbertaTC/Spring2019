#pragma once

#include <bullet/btBulletDynamicsCommon.h>
#include <glm/glm.hpp>

inline btVector3 glm2bt(const glm::vec3& vec)
{
    return btVector3(vec.x, vec.y, vec.z);
}

inline glm::vec3 bt2glm(const btVector3& vec)
{
    return glm::vec3(vec.x(), vec.y(), vec.z());
}

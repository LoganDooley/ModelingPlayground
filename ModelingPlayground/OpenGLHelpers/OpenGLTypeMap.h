#pragma once

#include <typeindex>
#include <unordered_map>
#include <glad/glad.h>

class OpenGLTypeMap
{
public:
    template <typename T>
    static GLenum ToGLEnum()
    {
        if (m_openGLTypeToEnumMap.contains(std::type_index(typeid(T))))
        {
            return m_openGLTypeToEnumMap[std::type_index(typeid(T))];
        }
        return GL_NONE;
    }

private:
    static std::unordered_map<std::type_index, GLenum> m_openGLTypeToEnumMap;
};

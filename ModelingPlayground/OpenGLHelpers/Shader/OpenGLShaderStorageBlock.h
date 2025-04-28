#pragma once
#include <iostream>
#include <string>
#include <unordered_map>

#include "glad/glad.h"

struct BufferProperty
{
    std::string m_name;
    unsigned int m_cumulativeOffset = 0;
    unsigned int m_offset = 0;
    int m_arrayStride = -1;
    std::vector<BufferProperty> m_members;

    BufferProperty operator[](unsigned int index) const
    {
        if (m_arrayStride == -1)
        {
            std::cerr << "BufferProperty|operator[]: Cannot use [] operator on non-array buffer property!\n";
            return {};
        }

        BufferProperty accessedProperty = m_members[index];
        accessedProperty.m_cumulativeOffset = m_cumulativeOffset + index * m_arrayStride;
        return accessedProperty;
    }

    BufferProperty operator()(const std::string& memberName) const
    {
        if (m_arrayStride != -1)
        {
            std::cerr << "BufferProperty|operator(): Cannot use () operator on array buffer property!\n";
            return {};
        }

        int indexOfMember = -1;
        for (int i = 0; i < m_members.size(); i++)
        {
            if (m_members[i].m_name == memberName)
            {
                indexOfMember = i;
            }
        }
        if (indexOfMember == -1)
        {
            std::cerr << "BufferProperty|operator(): Member with name \"" << memberName << "\" not found!\n";
            return {};
        }

        BufferProperty accessedProperty = m_members[indexOfMember];
        accessedProperty.m_cumulativeOffset = m_cumulativeOffset + accessedProperty.m_offset;
        return accessedProperty;
    }
};

class OpenGLShaderStorageBlock
{
public:
    OpenGLShaderStorageBlock(GLuint uniformBlockIndex, GLuint programId);
    ~OpenGLShaderStorageBlock() = default;

private:
    GLuint m_programId;
    std::vector<BufferProperty> m_members;
};

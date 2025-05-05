#pragma once
#include <iostream>
#include <string>
#include <unordered_map>

#include "glad/glad.h"

struct BufferProperty
{
    unsigned int m_cumulativeOffset = 0;
    unsigned int m_offset = 0;
    int m_arrayStride = -1;
    std::unordered_map<std::string, BufferProperty> m_members;

    BufferProperty operator[](unsigned int index) const
    {
        if (m_arrayStride == -1)
        {
            std::cerr << "BufferProperty|operator[]: Cannot use [] operator on non-array buffer property!\n";
            return {};
        }

        return {
            .m_cumulativeOffset = m_cumulativeOffset + index * m_arrayStride,
            .m_offset = m_offset,
            .m_arrayStride = -1,
            .m_members = m_members
        };
    }

    BufferProperty operator()(const std::string& memberName) const
    {
        if (m_arrayStride != -1)
        {
            std::cerr << "BufferProperty|operator(): Cannot use () operator on array buffer property!\n";
            return {};
        }

        if (!m_members.contains(memberName))
        {
            std::cerr << "BufferProperty|operator(): Member with name \"" << memberName << "\" not found!\n";
            return {};
        }

        BufferProperty accessedProperty = m_members.at(memberName);
        accessedProperty.m_cumulativeOffset = m_cumulativeOffset + accessedProperty.m_offset;
        return accessedProperty;
    }

    unsigned int GetCumulativeOffset() const
    {
        return m_cumulativeOffset;
    }

    unsigned int GetArrayStride() const
    {
        return m_arrayStride;
    }
};

class OpenGLShaderStorageBlock
{
public:
    OpenGLShaderStorageBlock(GLuint shaderStorageBlockIndex, GLuint programId);
    ~OpenGLShaderStorageBlock() = default;

    GLint GetShaderStorageBlockBinding() const;

    BufferProperty operator()(const std::string& memberName) const;

private:
    void TryCreateBufferProperties(const std::string& name, unsigned int offset,
                                   int topLevelArrayStride, int arrayStride);
    static bool ShouldAddMember(const std::string& memberName);
    static unsigned int UpdateOffset(BufferProperty& bufferProperty);
    static void SwitchToRelativeOffset(BufferProperty& bufferProperty, unsigned int parentCumulativeOffset);
    static bool MemberNameIsArray(const std::string& memberName);
    static int GetMemberArrayIndex(const std::string& memberName);
    static std::string GetMemberNameFromArrayName(const std::string& arrayName);
    static void UpdateArrayMembers(std::unordered_map<std::string, BufferProperty>* members);

    GLuint m_programId;
    GLint m_dataSize;
    GLint m_binding;
    std::unordered_map<std::string, BufferProperty> m_members;
};

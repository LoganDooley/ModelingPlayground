#include "OpenGLShaderStorageBlock.h"

#include <algorithm>
#include <ranges>

OpenGLShaderStorageBlock::OpenGLShaderStorageBlock(GLuint shaderStorageBlockIndex, GLuint programId):
    m_programId(programId)
{
    GLenum property[1] = {GL_NUM_ACTIVE_VARIABLES};
    GLint numActiveVariables;
    glGetProgramResourceiv(m_programId, GL_SHADER_STORAGE_BLOCK, shaderStorageBlockIndex, 1, property, 1, nullptr,
                           &numActiveVariables);

    std::vector<GLint> activeVariables(numActiveVariables);
    property[0] = GL_ACTIVE_VARIABLES;
    glGetProgramResourceiv(m_programId, GL_SHADER_STORAGE_BLOCK, shaderStorageBlockIndex, 1, property,
                           numActiveVariables,
                           nullptr, activeVariables.data());

    property[0] = GL_BUFFER_DATA_SIZE;
    glGetProgramResourceiv(m_programId, GL_SHADER_STORAGE_BLOCK, shaderStorageBlockIndex, 1, property, 1, nullptr,
                           &m_dataSize);

    property[0] = GL_BUFFER_BINDING;
    glGetProgramResourceiv(m_programId, GL_SHADER_STORAGE_BLOCK, shaderStorageBlockIndex, 1, property, 1, nullptr,
                           &m_binding);

    for (const auto& activeVariableIndex : activeVariables)
    {
        GLint variableNameLength;
        property[0] = GL_NAME_LENGTH;
        glGetProgramResourceiv(m_programId, GL_BUFFER_VARIABLE, activeVariableIndex, 1, property, 1, nullptr,
                               &variableNameLength);
        std::vector<GLchar> variableName(variableNameLength);
        glGetProgramResourceName(m_programId, GL_BUFFER_VARIABLE, activeVariableIndex, variableNameLength, nullptr,
                                 variableName.data());
        property[0] = GL_TOP_LEVEL_ARRAY_STRIDE;
        GLint topLevelStride;
        glGetProgramResourceiv(m_programId, GL_BUFFER_VARIABLE, activeVariableIndex, 1, property, 1, nullptr,
                               &topLevelStride); // Useful for array of structs
        property[0] = GL_ARRAY_STRIDE;
        GLint arrayStride;
        glGetProgramResourceiv(m_programId, GL_BUFFER_VARIABLE, activeVariableIndex, 1, property, 1, nullptr,
                               &arrayStride); // Useful for array of basic types
        property[0] = GL_OFFSET;
        GLint offset;
        glGetProgramResourceiv(m_programId, GL_BUFFER_VARIABLE, activeVariableIndex, 1, property, 1, nullptr,
                               &offset); // Total offset

        TryCreateBufferProperties(variableName.data(), offset, topLevelStride, arrayStride);
    }

    for (auto& bufferProperty : m_members | std::views::values)
    {
        UpdateOffset(bufferProperty);
    }
    for (auto& bufferProperty : m_members | std::views::values)
    {
        SwitchToRelativeOffset(bufferProperty, 0);
    }

    UpdateArrayMembers(&m_members);
}

GLint OpenGLShaderStorageBlock::GetShaderStorageBlockBinding() const
{
    return m_binding;
}

BufferProperty OpenGLShaderStorageBlock::operator()(const std::string& memberName) const
{
    if (!m_members.contains(memberName))
    {
        std::cerr << "OpenGLShaderStorageBlock|operator(): Member with name \"" << memberName << "\" not found!\n";
        return {};
    }

    BufferProperty accessedProperty = m_members.at(memberName);
    accessedProperty.m_cumulativeOffset = accessedProperty.m_offset;
    return accessedProperty;
}

void OpenGLShaderStorageBlock::TryCreateBufferProperties(const std::string& name, unsigned int offset,
                                                         int topLevelArrayStride, int arrayStride)
{
    std::vector<std::string> memberPath;
    std::vector<bool> arrayTypes;
    int lastDotIndex = -1;
    for (int i = 0; i < name.size(); i++)
    {
        if (name[i] == '.')
        {
            std::string memberName = name.substr(lastDotIndex + 1, i);
            if (!ShouldAddMember(memberName))
            {
                return;
            }
            memberPath.push_back(memberName);
            lastDotIndex = i;
        }
    }
    if (!ShouldAddMember(name.substr(lastDotIndex + 1, name.size())))
    {
        return;
    }
    memberPath.push_back(name.substr(lastDotIndex + 1, name.size()));

    std::unordered_map<std::string, BufferProperty>* memberMap = &m_members;
    bool hasSetTopLevelArrayStride = false;
    for (const auto& memberName : memberPath)
    {
        if (!memberMap->contains(memberName))
        {
            memberMap->insert({
                memberName, BufferProperty{
                    .m_offset = offset,
                    .m_members = std::unordered_map<std::string, BufferProperty>(),
                }
            });
            if (MemberNameIsArray(memberName))
            {
                memberMap->at(memberName).m_arrayStride = arrayStride;
                if (!hasSetTopLevelArrayStride && topLevelArrayStride > 0)
                {
                    memberMap->at(memberName).m_arrayStride = topLevelArrayStride;
                    hasSetTopLevelArrayStride = true;
                }
            }
            memberMap = &memberMap->at(memberName).m_members;
        }
        else
        {
            if (MemberNameIsArray(memberName))
            {
                memberMap->at(memberName).m_arrayStride = arrayStride;
                if (!hasSetTopLevelArrayStride && topLevelArrayStride > 0)
                {
                    memberMap->at(memberName).m_arrayStride = topLevelArrayStride;
                    hasSetTopLevelArrayStride = true;
                }
            }
            memberMap = &memberMap->at(memberName).m_members;
        }
    }
}

bool OpenGLShaderStorageBlock::ShouldAddMember(const std::string& memberName)
{
    if (memberName.find('[') != std::string::npos)
    {
        if (memberName.find("[0]") == std::string::npos && memberName.find("[1]") == std::string::npos)
        {
            // Not first or second array element, return false
            return false;
        }
    }
    return true;
}

unsigned int OpenGLShaderStorageBlock::UpdateOffset(BufferProperty& bufferProperty)
{
    unsigned int minOffset = bufferProperty.m_offset;

    for (auto& val : bufferProperty.m_members | std::views::values)
    {
        minOffset = std::min(minOffset, UpdateOffset(val));
    }

    bufferProperty.m_offset = minOffset;

    return minOffset;
}

void OpenGLShaderStorageBlock::SwitchToRelativeOffset(BufferProperty& bufferProperty,
                                                      unsigned int parentCumulativeOffset)
{
    unsigned int cumulativeOffset = bufferProperty.m_offset;
    bufferProperty.m_offset -= parentCumulativeOffset;
    for (auto& member : bufferProperty.m_members | std::views::values)
    {
        SwitchToRelativeOffset(member, cumulativeOffset);
    }
}

bool OpenGLShaderStorageBlock::MemberNameIsArray(const std::string& memberName)
{
    return memberName.find('[') != std::string::npos;
}

int OpenGLShaderStorageBlock::GetMemberArrayIndex(const std::string& memberName)
{
    return memberName.find("[0]") != std::string::npos ? 0 : 1;
}

std::string OpenGLShaderStorageBlock::GetMemberNameFromArrayName(const std::string& arrayName)
{
    return arrayName.substr(0, arrayName.find('['));
}

void OpenGLShaderStorageBlock::UpdateArrayMembers(std::unordered_map<std::string, BufferProperty>* members)
{
    // get keys for array members
    std::vector<std::string> arrayMemberNames;
    for (const auto& member : *members)
    {
        if (MemberNameIsArray(member.first))
        {
            arrayMemberNames.push_back(member.first);
        }
    }

    for (const auto& arrayMemberName : arrayMemberNames)
    {
        if (!members->contains(arrayMemberName))
        {
            continue;
        }

        int arrayIndex = GetMemberArrayIndex(arrayMemberName);
        std::string memberName = GetMemberNameFromArrayName(arrayMemberName);
        if (members->contains(memberName + "[1]"))
        {
            if (members->contains(memberName + "[0]"))
            {
                members->at(memberName + "[0]").m_arrayStride = members->at(memberName + "[1]").m_offset - members->at(
                    memberName + "[0]").m_offset;
            }

            members->erase(memberName + "[1]");
        }

        auto pair = members->extract(arrayMemberName);
        pair.key() = std::move(memberName);
        members->insert(std::move(pair));
    }

    for (auto& member : *members | std::views::values)
    {
        UpdateArrayMembers(&member.m_members);
    }
}

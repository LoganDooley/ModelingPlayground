#include "OpenGLShaderStorageBlock.h"

#include <algorithm>

OpenGLShaderStorageBlock::OpenGLShaderStorageBlock(GLuint uniformBlockIndex, GLuint programId):
    m_programId(programId)
{
    GLenum property[1] = {GL_NUM_ACTIVE_VARIABLES};
    GLint numActiveVariables;
    glGetProgramResourceiv(m_programId, GL_SHADER_STORAGE_BLOCK, uniformBlockIndex, 1, property, 1, nullptr,
                           &numActiveVariables);

    std::vector<GLint> activeVariables(numActiveVariables);
    property[0] = GL_ACTIVE_VARIABLES;
    glGetProgramResourceiv(m_programId, GL_SHADER_STORAGE_BLOCK, uniformBlockIndex, 1, property, numActiveVariables,
                           nullptr, activeVariables.data());

    std::vector<MemberOffset> memberOffsets;

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
        GLint structStride;
        glGetProgramResourceiv(m_programId, GL_BUFFER_VARIABLE, activeVariableIndex, 1, property, 1, nullptr,
                               &structStride); // Useful for array of structs
        property[0] = GL_ARRAY_STRIDE;
        GLint valueStride;
        glGetProgramResourceiv(m_programId, GL_BUFFER_VARIABLE, activeVariableIndex, 1, property, 1, nullptr,
                               &valueStride); // Useful for array of typed
        property[0] = GL_OFFSET;
        GLint offset;
        glGetProgramResourceiv(m_programId, GL_BUFFER_VARIABLE, activeVariableIndex, 1, property, 1, nullptr,
                               &offset); // Total offset

        MemberOffset memberOffset;
        if (TryCreateMemberOffset(memberOffset, variableName.data(), offset, std::max(structStride, valueStride)))
        {
            memberOffsets.push_back(memberOffset);
        }
    }

    /*
     * Parse out all variables except for those with [n] in them where n != 0
     * if ends in [0], it is an array of a simple type, use GL_ARRAY_STRIDE
     * otherwise it is a member
     *
     * if it is a member prefixed by [0]., get the stride of that array with GL_TOP_LEVEL_ARRAY_STRIDE
     * Get their local offset by subtracting their offset from the first member's offset in that struct (all variables
     * with same prefix and that have no suffix, [0] is ok but another . is not)
     */

    std::ranges::sort(memberOffsets, [](MemberOffset a, MemberOffset b)
    {
        return a.m_name.size() < b.m_name.size();
    });
}

bool OpenGLShaderStorageBlock::TryCreateMemberOffset(MemberOffset& memberOffset, const std::string& name,
                                                     unsigned int offset, unsigned int arrayStride)
{
    int lastDotIndex = -1;
    for (int i = 0; i < name.size(); i++)
    {
        if (name[i] == '.')
        {
            std::string nameFragment = name.substr(lastDotIndex + 1, i);
            if (!TryAddNameFragment(memberOffset, nameFragment))
            {
                return false;
            }
            lastDotIndex = i;
        }
    }
    std::string nameFragment = name.substr(lastDotIndex + 1, name.size());
    if (!TryAddNameFragment(memberOffset, nameFragment))
    {
        return false;
    }
    memberOffset.m_offset = offset;
    memberOffset.m_arrayStride = arrayStride;
    return true;
}

bool OpenGLShaderStorageBlock::TryAddNameFragment(MemberOffset& memberOffset, const std::string& nameFragment)
{
    if (nameFragment.find('[') != std::string::npos)
    {
        if (nameFragment.find("[0]") == std::string::npos)
        {
            // Not first array element, return false
            return false;
        }
        memberOffset.m_arrayTypes.push_back(true);
    }
    else
    {
        memberOffset.m_arrayTypes.push_back(false);
    }
    memberOffset.m_name.push_back(nameFragment);
    return true;
}

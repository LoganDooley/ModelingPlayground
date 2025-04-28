#include "OpenGLShaderStorageBlock.h"

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

    for (const auto& activeVariableIndex : activeVariables)
    {
        GLint variableNameLength;
        property[0] = GL_NAME_LENGTH;
        glGetProgramResourceiv(m_programId, GL_BUFFER_VARIABLE, activeVariableIndex, 1, property, 1, nullptr,
                               &variableNameLength);
        std::vector<GLchar> variableName(variableNameLength);
        glGetProgramResourceName(m_programId, GL_BUFFER_VARIABLE, activeVariableIndex, variableNameLength, nullptr,
                                 variableName.data());
        property[0] = GL_TYPE;
        GLint variableType;
        glGetProgramResourceiv(m_programId, GL_BUFFER_VARIABLE, activeVariableIndex, 1, property, 1, nullptr,
                               &variableType);
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
}

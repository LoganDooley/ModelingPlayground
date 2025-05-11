#pragma once

#include <string>
#include <unordered_map>
#include "BlockMember.h"

#include "glad/glad.h"

class OpenGLShaderBlock
{
public:
    OpenGLShaderBlock(GLuint shaderStorageBlockIndex, GLuint programId, GLenum blockType, GLenum variableType);
    ~OpenGLShaderBlock() = default;

    GLint GetShaderBlockBinding() const;
    GLint GetDataSize() const;

    BlockMember operator()(const std::string& memberName) const;

private:
    void TryCreateBlockMembers(const std::string& name, unsigned int offset,
                               int topLevelArrayStride, int arrayStride);
    static bool ShouldAddMember(const std::string& memberName);
    static unsigned int UpdateOffset(BlockMember& blockMember);
    static void SwitchToRelativeOffset(BlockMember& blockMember, unsigned int parentCumulativeOffset);
    static bool MemberNameIsArray(const std::string& memberName);
    static int GetMemberArrayIndex(const std::string& memberName);
    static std::string GetMemberNameFromArrayName(const std::string& arrayName);
    static void UpdateArrayMembers(std::unordered_map<std::string, BlockMember>* members);

    GLuint m_programId;
    GLint m_dataSize;
    GLint m_binding;
    std::unordered_map<std::string, BlockMember> m_members;
};

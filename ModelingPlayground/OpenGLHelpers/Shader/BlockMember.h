#pragma once

#include <iostream>
#include <string>
#include <unordered_map>

struct BlockMember
{
    unsigned int m_cumulativeOffset = 0;
    unsigned int m_offset = 0;
    int m_arrayStride = -1;
    std::unordered_map<std::string, BlockMember> m_members;

    BlockMember operator[](unsigned int index) const
    {
        if (m_arrayStride == -1)
        {
            std::cerr << "BlockMember|operator[]: Cannot use [] operator on non-array block member!\n";
            return {};
        }

        return {
            .m_cumulativeOffset = m_cumulativeOffset + index * m_arrayStride,
            .m_offset = m_offset,
            .m_arrayStride = -1,
            .m_members = m_members
        };
    }

    BlockMember operator()(const std::string& memberName) const
    {
        if (m_arrayStride != -1)
        {
            std::cerr << "BlockMember|operator(): Cannot use () operator on array block member!\n";
            return {};
        }

        if (!m_members.contains(memberName))
        {
            std::cerr << "BlockMember|operator(): Member with name \"" << memberName << "\" not found!\n";
            return {};
        }

        BlockMember accessedProperty = m_members.at(memberName);
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

#pragma once
#include <iostream>
#include <string>
#include <unordered_map>

template <typename E>
class EnumLabel
{
public:
    EnumLabel(std::unordered_map<E, std::string> enumToStringMap)
    {
        m_enumToString = enumToStringMap;
        for (auto& enumString : enumToStringMap)
        {
            if (m_stringToEnum.count(enumString.second) != 0)
            {
                std::cerr << "Two enums map to the same string: "<<enumString.second<<"!\n";
            }
            m_stringToEnum[enumString.second] = enumString.first;
            m_enums.emplace_back(enumString.first);
            m_strings.emplace_back(enumString.second);
        }
        for (const auto& m_string : m_strings)
        {
            m_cStrings.emplace_back(m_string.c_str());
        }
    }

    ~EnumLabel() = default;

    std::string ToString(E enumValue)
    {
        if (!m_enumToString.contains(enumValue))
        {
            std::cerr << "String not mapped for enum valued!\n";
        }
        return m_enumToString.at(enumValue);
    }

    E ToEnum(std::string enumString)
    {
        if (!m_stringToEnum.contains(enumString))
        {
            std::cerr << "Enum not mapped for string: "<<enumString<<"!\n";
        }
        return m_stringToEnum.at(enumString);
    }

    std::vector<E> GetEnums()
    {
        return m_enums;
    }

    std::vector<std::string> GetStrings()
    {
        return m_strings;
    }

    std::vector<const char*> GetCStrings()
    {
        return m_cStrings;
    }

    int Count()
    {
        return m_enums.size();
    }

    int GetEnumIndex(E enumValue)
    {
        for (int i = 0; i<m_enums.size(); i++)
        {
            if (m_enums.at(i) == enumValue)
            {
                return i;
            }
        }
        std::cerr << "EnumLabel|GetEnumIndex: Enum not found!\n";
        return 0;
    }

private:
    std::unordered_map<E, std::string> m_enumToString;
    std::unordered_map<std::string, E> m_stringToEnum;
    std::vector<E> m_enums;
    std::vector<std::string> m_strings;
    std::vector<const char*> m_cStrings;
};

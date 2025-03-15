#pragma once

template <typename T>
class Dirtyable
{
public:
    Dirtyable(T data)
    {
        m_data = data;
        m_isDirty = false;
    }

    ~Dirtyable() = default;

    void MarkDirty()
    {
        m_isDirty = true;
    }

    void SetCleanData(T data)
    {
        m_data = data;
        m_isDirty = false;
    }

    const T& GetData() const
    {
        return m_data;
    }

    const bool& IsDirty() const
    {
        return m_isDirty;
    }

private:
    T m_data;
    bool m_isDirty;
};

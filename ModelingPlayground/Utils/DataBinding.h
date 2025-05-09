#pragma once
#include <iostream>
#include <unordered_map>
#include <functional>

template <typename T>
class DataBinding
{
public:
    DataBinding() = default;

    DataBinding(T data)
    {
        m_data = data;
    }

    ~DataBinding() = default;

    const T& GetData() const
    {
        return m_data;
    }

    bool SetAndNotify(T newData, bool forceNotify = false)
    {
        if (!forceNotify && newData == m_data)
        {
            return false;
        }

        T oldData = m_data;
        m_data = newData;
        for (const auto& subscriber : m_subscribers)
        {
            subscriber.second(m_data, oldData);
        }
        return true;
    }

    bool SetWithoutNotify(T newData)
    {
        if (newData == m_data)
        {
            return false;
        }
        m_data = newData;
        return true;
    }

    void Subscribe(const void* subscriber, std::function<void(const T&, T)> callback, bool initialCall = false,
                   bool logOnDoubleSubscription = true)
    {
        if (m_subscribers.contains(subscriber))
        {
            if (logOnDoubleSubscription)
            {
                std::cout << "DataBinding|Subscribe: Attempting to subscribe twice with the same object!\n";
            }
            return;
        }
        m_subscribers[subscriber] = callback;
        if (initialCall)
        {
            //T defaultValue;
            m_subscribers[subscriber](m_data, m_data);
        }
    }

    void Unsubscribe(const void* subscriber)
    {
        if (!m_subscribers.contains(subscriber))
        {
            std::cout << "DataBinding|Unsubscribe: Attempting to unsubscribe but wasn't subscribed!\n";
            return;
        }
        m_subscribers.erase(subscriber);
    }

private:
    T m_data;
    std::unordered_map<const void*, std::function<void(const T&, T)>> m_subscribers;
};

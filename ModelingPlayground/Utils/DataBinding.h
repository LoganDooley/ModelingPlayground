#pragma once

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
    
    const T& GetData() const{
        return m_data;
    }
    
    bool SetAndNotify(T newData){
        if (newData == m_data)
        {
            return false;
        }
        
        T oldData = m_data;
        m_data = newData;
        for(const auto& subscriber : m_subscribers){
            subscriber(m_data, oldData);
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
    
    void Subscribe(std::function<void(const T&, T)> callback){
        m_subscribers.push_back(callback);
    }

private:
    T m_data;
    std::vector<std::function<void(const T&, T)>> m_subscribers;
};
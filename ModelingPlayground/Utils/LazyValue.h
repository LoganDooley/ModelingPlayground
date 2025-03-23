#pragma once

template <typename T>
class LazyValue
{
public:
	LazyValue(T initialValue, std::function<T()> updateValueFunction)
	{
		m_value = initialValue;
		m_isDirty = false;
		m_updateValueFunction = updateValueFunction;
	}

	~LazyValue() = default;

	void MarkDirty()
	{
		m_isDirty = true;
	}

	const T& GetValue()
	{
		if (m_isDirty)
		{
			UpdateValueInternal();
		}

		return m_value;
	}

private:
	void UpdateValueInternal()
	{
		m_value = m_updateValueFunction();
		m_isDirty = false;
	}

	T m_value;
	bool m_isDirty;
	std::function<T()> m_updateValueFunction;
};

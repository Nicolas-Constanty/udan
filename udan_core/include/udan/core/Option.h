#pragma once
#include <utility>

namespace udan::core
{
	enum class OptionValue
	{
		None,
		Some
	};
	template <class T>
	class Option
	{
	public:
		OptionValue OptionSet() const
		{
			return  m_optionValue;
		}
		
		[[deprecated("This function could lead to UB.")]]
		T unwrap() const
		{
			return m_value;
		}

		T& operator=(const T& other)
		{
			if (m_value != other)
			{
				m_value = other;
				m_optionValue = OptionValue::Some;
			}
			return m_value;
		}


		T& operator=(T&& other)
		{
			m_value = std::move(other);
			m_optionValue = OptionValue::Some;
			return m_value;
		}
		
	private:
		OptionValue m_optionValue = OptionValue::None;
		T m_value;
	};
}

#define LET(x) x.OptionSet() == udan::core::OptionValue::Some ? x.unwrap() : 0
#define MATCH(x, a, b) x.OptionSet() == udan::core::OptionValue::Some ? a(x.unwrap()) : b()

namespace udan::core
{
	template<class T>
	void PrintOption(const udan::core::Option<T>& opt)
	{
		if (T val = LET(opt))
		{
			std::cout << "Option Value : " << val << std::endl;
		}
		else
		{
			std::cout << "Option as no value" << std::endl;
		}
	}
}

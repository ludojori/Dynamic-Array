#ifndef DYNAMIC_ARRAY_H
#define DYNAMIC_ARRAY_H

#include <iostream>
#include <exception>
#include <stdexcept>

namespace dynamic
{
	template<class T>
	class Array
	{
	private:
		T* m_data;
		size_t m_size;
		size_t m_cap;

		void expand();

	public:

		// CONSTRUCTORS

		Array();
		Array(const Array<T>& other);
		Array(Array<T>&& other);

		// OPERATORS

		Array<T>& operator=(const Array<T>& other);
		Array<T>& operator=(Array<T>&& other);
		constexpr T& operator[](size_t index);
		constexpr const T& operator[](size_t index) const;
		bool operator<(const Array<T>& other) const;
		bool operator>(const Array<T>& other) const;
		bool operator==(const Array<T>& other) const;
		bool operator!=(const Array<T>& other) const;
		friend std::ostream& operator<<(std::ostream& os, const Array<T>& arr);

		// USEFUL FUNCTIONS

		constexpr T* data() noexcept;
		constexpr const T* data() const noexcept;
		constexpr const T& at(size_t index) const;
		constexpr void push_back(const T& value);
		constexpr void push_back(T&& value);
		template<typename... Args>
		void emplace_back(Args&&... args);
		constexpr void pop_back() noexcept;
		constexpr void pop_front() noexcept;
		constexpr size_t size() const noexcept;
		constexpr size_t capacity() const noexcept;
		constexpr void clear() noexcept;
		[[nodiscard]] constexpr bool empty() const;
		constexpr void print() const noexcept;
		
		constexpr T& front();
		constexpr const T& front() const;
		constexpr T& back();
		constexpr const T& back() const;
		T* begin() const;
		T* end() const;

		// DESTRUCTOR

		~Array();
	};

	template<class T>
	inline void Array<T>::expand()
	{
		m_cap *= 2;
		T* temp = (T*)realloc(m_data, m_cap * sizeof(T));

		if (temp)
		{
			m_data = temp;
		}
		else
		{
			throw std::bad_alloc();
		}
	}

	template<class T>
	inline Array<T>::Array() : m_size(0), m_cap(1)
	{
		m_data = (T*)malloc(m_cap * sizeof(T));
	}

	template<class T>
	inline Array<T>::Array(const Array<T>& other) : m_size(other.m_size), m_cap(other.m_cap)
	{
		m_data = (T*)malloc(m_cap * sizeof(T));
		std::memcpy(m_data, other.m_data, m_cap * sizeof(T));
	}

	template<class T>
	inline Array<T>::Array(Array<T>&& other) : m_size(other.m_size), m_cap(other.m_cap)
	{
		m_data = other.m_data;
		other.m_data = nullptr;
		other.m_size = 0;
		other.m_cap = 1;
	}

	template<class T>
	inline Array<T>& Array<T>::operator=(const Array<T>& other)
	{
		if (this == &other)
		{
			return *this;
		}

		if (other.m_size == 0)
		{
			for (size_t i = 0; i < m_cap; i++)
			{
				m_data[i].~T();
			}

			free(m_data);
			m_size = 0;
			m_cap = 1;

			return *this;
		}

		if (m_data != nullptr)
		{
			for (size_t i = 0; i < m_cap; i++)
			{
				m_data[i].~T();
			}

			free(m_data);
		}

		m_size = other.m_size;
		m_cap = other.m_cap;
		m_data = (T*)malloc(m_cap * sizeof(T));
		std::memcpy(m_data, other.m_data, m_cap * sizeof(T));

		return *this;
	}

	template<class T>
	inline Array<T>& Array<T>::operator=(Array<T>&& other)
	{
		if (this == &other)
		{
			return *this;
		}

		if (other.m_size == 0)
		{
			free(m_data);
			m_size = 0;
			m_cap = 1;
			
			return *this;
		}

		if (m_data != nullptr)
		{
			for (size_t i = 0; i < m_cap; i++)
			{
				m_data[i].~T();
			}

			free(m_data);
		}

		m_data = other.m_data;
		m_size = other.m_size;
		m_cap = other.m_cap;

		other.m_data = nullptr;
		other.m_size = 0;
		other.m_cap = 1;

		return *this;
	}

	template<class T>
	inline constexpr T& Array<T>::operator[](size_t index)
	{
		return m_data[index];
	}
	
	template<class T>
	inline constexpr const T& Array<T>::operator[](size_t index) const
	{
		if (index < 0 || index >= m_size)
		{
			throw std::out_of_range;
		}
		return m_data[index];
	}

	template<class T>
	inline bool Array<T>::operator<(const Array<T>& other) const
	{
		T* this_begin = this->begin();
		T* this_end = this->end();
		T* other_begin = other.begin();
		T* other_end = other.end();

		for (; (this_begin != this_end) && (other_begin != other_end); ++this_begin, ++this_end)
		{
			if (*this_begin < *other_begin) return true;
			if (*this_begin > *other_begin) return false;
		}

		return (this_begin == this_end) && (other_begin != other_end);
	}

	template<class T>
	inline bool Array<T>::operator>(const Array<T>& other) const
	{
		return !(this->operator<(other));
	}

	template<class T>
	inline bool Array<T>::operator==(const Array<T>& other) const
	{
		if (this->m_size != other.m_size)
		{
			return false;
		}

		for (size_t i = 0; i < m_size; i++)
		{
			if (m_data[i] != other[i])
			{
				return false;
			}
		}

		return true;
	}

	template<class T>
	inline bool Array<T>::operator!=(const Array<T>& other) const
	{
		return !(this->operator==(other));
	}

	template<class T>
	inline std::ostream& operator<<(std::ostream& os, Array<T>& other)
	{
		if (other.empty())
		{
			os << "!--- ERROR: Cannot Print - Array Is Empty ---!\n";
			return os;
		}
		os << "<<< Printing Dynamic Array... >>>" << std::endl << std::endl;
		for (size_t i = 0; i < other.size(); i++)
		{
			os << " " << other[i];
		}
		return os;
	}

	template<class T>
	inline constexpr T* Array<T>::data() noexcept
	{
		return m_data;
	}

	template<class T>
	inline constexpr const T* Array<T>::data() const noexcept
	{
		return m_data;
	}

	template<class T>
	inline constexpr const T& Array<T>::at(size_t index) const
	{
		if (index >= m_size)
		{
			throw std::out_of_range;
		}
		return m_data[index];
	}

	template<class T>
	inline constexpr void Array<T>::push_back(const T& value)
	{
		if (m_size == m_cap)
		{
			expand();
		}
		m_data[m_size] = value;
		m_size++;
	}

	template<class T>
	inline constexpr void Array<T>::push_back(T&& value)
	{
		if (m_size == m_cap)
		{
			expand();
		}
		m_data[m_size] = std::move(value);
		m_size++;
	}

	template<class T>
	inline constexpr void Array<T>::pop_back() noexcept
	{
		if (m_size > 0)
		{
			m_size--;
		}
	}

	template<class T>
	inline constexpr void Array<T>::pop_front() noexcept
	{
		if (m_size > 0)
		{
			for (size_t i = 1; i < m_size; i++)
			{
				m_data[i - 1] = m_data[i];
			}
			m_size--;
		}
	}

	template<class T>
	inline constexpr size_t Array<T>::size() const noexcept
	{
		return m_size;
	}

	template<class T>
	inline constexpr size_t Array<T>::capacity() const noexcept
	{
		return m_cap;
	}

	template<class T>
	inline constexpr void Array<T>::clear() noexcept
	{
		for (size_t i = 0; i < m_size; i++)
		{
			m_data[i].~T();
		}
		m_size = 0;
	}

	template<class T>
	inline constexpr bool Array<T>::empty() const
	{
		return m_size == 0 ? true : false;
	}

	template<class T>
	inline constexpr void Array<T>::print() const noexcept
	{
		if (empty())
		{
			std::cout << "!--- ERROR: Cannot Print - Array Is Empty ---!" << std::endl;
			return;
		}
		std::cout << "<<< Printing Dynamic Array... >>>" << std::endl << std::endl;
		for (size_t i = 0; i < m_size; i++)
		{
			std::cout << " " << m_data[i];
		}
	}

	template<class T>
	inline constexpr T& Array<T>::front()
	{
		return m_data[0];
	}

	template<class T>
	inline constexpr const T& Array<T>::front() const
	{
		return m_data[0];
	}

	template<class T>
	inline constexpr T& Array<T>::back()
	{
		return m_data[m_size - 1];
	}

	template<class T>
	inline constexpr const T& Array<T>::back() const
	{
		return m_data[m_size - 1];
	}

	template<class T>
	inline T* Array<T>::begin() const
	{
		return m_data;
	}
	
	template<class T>
	inline T* Array<T>::end() const
	{
		return m_data + m_size;
	}

	template<class T>
	inline Array<T>::~Array()
	{
		for (size_t i = 0; i < m_cap; i++)
		{
			m_data[i].~T();
		}

		free(m_data);
		m_size = 0;
		m_cap = 0;
	}
	template<class T>
	template<typename ...Args>
	inline void Array<T>::emplace_back(Args && ...args)
	{
		if (m_size == m_cap)
		{
			expand();
		}

		m_data[m_size] = T(std::forward<Args>(arg)...);
		m_size++;
	}
}

#endif

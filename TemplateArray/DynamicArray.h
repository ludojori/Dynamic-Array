#pragma once
#include <iostream>
#include <exception>

namespace dynamic
{
	template<class T>
	class array
	{
	private:
		T* m_data;
		size_t m_size;
		size_t m_cap;

		void expand();

	public:

		// CONSTRUCTORS

		array();
		array(const array<T>&);
		array(array<T>&&);

		// OPERATORS

		array<T>& operator=(const array<T>&);
		array<T>& operator=(array<T>&&);
		T& operator[](size_t) const;
		bool operator<(const array<T>&) const;
		bool operator>(const array<T>&) const;
		bool operator==(const array<T>&) const;
		friend std::ostream& operator<<(std::ostream&, const array<T>&);

		// USEFUL FUNCTIONS

		void add(T);
		void generate(T, size_t);
		void remove();
		void remove(size_t);
		size_t size() const;
		bool empty() const;
		void print() const;

		// DESTRUCTOR

		~array();
	};

	template<class T>
	inline void array<T>::expand()
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
	inline array<T>::array() : m_size(0), m_cap(1)
	{
		m_data = (T*)malloc(m_cap * sizeof(T));
	}

	template<class T>
	inline array<T>::array(const array<T>& dynArray) : m_size(dynArray.m_size), m_cap(dynArray.m_cap)
	{
		m_data = (T*)malloc(m_cap * sizeof(T));
		std::memcpy(m_data, dynArray.m_data, m_cap * sizeof(T));
	}

	template<class T>
	inline array<T>::array(array<T>&& dynArray) : m_size(dynArray.m_size), m_cap(dynArray.m_cap)
	{
		m_data = dynArray.m_data;
		dynArray.m_data = nullptr;
		dynArray.m_size = 0;
		dynArray.m_cap = 1;
	}

	template<class T>
	inline array<T>& array<T>::operator=(const array<T>& other)
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
	inline array<T>& array<T>::operator=(array<T>&& other)
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
	inline T& array<T>::operator[](size_t index) const
	{
		if (index < 0 || index > m_size)
		{
			std::cout << "!--- ERROR: Out Of Bounds ---!" << std::endl;
		}
		else
		{
			return m_data[index];
		}
	}

	template<class T>
	inline bool array<T>::operator<(const array<T>& dynArray) const
	{
		m_size < dynArray.m_size ? true : false;
	}

	template<class T>
	inline bool array<T>::operator>(const array<T>& dynArray) const
	{
		m_size > dynArray.m_size ? true : false;
	}

	template<class T>
	inline bool array<T>::operator==(const array<T>& dynArray) const
	{
		if (this < dynArray || this > dynArray)
		{
			return false;
		}
		for (size_t i = 0; i < m_size; i++)
		{
			if (m_data[i] != dynArray[i])
			{
				return false;
			}
		}
		return true;
	}

	template<class T>
	inline std::ostream& operator<<(std::ostream& os, array<T>& dynArray)
	{
		if (dynArray.empty())
		{
			os << "!--- ERROR: Cannot Print - Array Is Empty ---!\n";
			return os;
		}
		os << "<<< Printing Dynamic Array... >>>" << std::endl << std::endl;
		for (size_t i = 0; i < dynArray.size(); i++)
		{
			os << " " << dynArray[i];
		}
		return os;
	}

	template<class T>
	inline void array<T>::add(T value)
	{
		if (m_size == m_cap)
		{
			expand();
		}
		m_size++;
		m_data[m_size - 1] = value;
	}

	template<class T>
	inline void array<T>::generate(T value, size_t length)
	{
		if (length < 0 || length > m_cap)
		{
			std::cout << "!--- ERROR: Out Of Bounds ---!" << std::endl;
			return;
		}
		for (size_t i = 0; i < length; i++)
		{
			expand();
		}
		std::cout << m_cap << std::endl;
		m_size = m_size + length;
		for (size_t i = m_size - length; i < m_size; i++)
		{
			m_data[i] = value;
		}
	}

	template<class T>
	inline void array<T>::remove()
	{
		m_size--;
		T* tempArr = new T[m_size];
		for (size_t i = 0; i < m_size; i++)
		{
			tempArr[i] = m_data[i];
		}
		delete[] m_data;
		m_data = tempArr;
	}

	template<class T>
	inline void array<T>::remove(size_t index)
	{
		if (index < 0 || index > m_size)
		{
			std::cout << "!--- ERROR: Out Of Bounds! ---!" << std::endl;
			return;
		}
		T* tempArr = new T[m_size];
		for (size_t i = index - 1; i < m_size; i++)
		{
			m_data[i] = m_data[i + 1];
		}
		remove();
	}

	template<class T>
	inline size_t array<T>::size() const
	{
		return m_size;
	}

	template<class T>
	inline bool array<T>::empty() const
	{
		return m_size == 0 ? true : false;
	}

	template<class T>
	inline void array<T>::print() const
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
	inline array<T>::~array()
	{
		for (size_t i = 0; i < m_cap; i++)
		{
			m_data[i].~T();
		}

		free(m_data);
		m_size = 0;
		m_cap = 0;
	}

}


#pragma once
#include <iostream>
#include <exception>

namespace dynamic
{
	template<class T>
	class array
	{
	private:
		T* _arr;
		size_t _size;
		size_t _cap;

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
		_cap++;
		T* tempArr = new T[_cap];
		for (size_t i = 0; i < _size; i++)
		{
			tempArr[i] = _arr[i];
		}
		delete[] _arr;
		_arr = tempArr;
	}

	template<class T>
	inline array<T>::array()
	{
		_size = 0;
		_cap = 1;
		_arr = new T[_cap];
	}

	template<class T>
	inline array<T>::array(const array<T>& dynArray)
	{
		if (this != &dynArray)
		{
			delete[] _arr;
			_size = dynArray._size;
			_cap = dynArray._cap;
			_arr = new T[_cap];
			for (size_t i = 0; i < _size; i++)
			{
				_arr[i] = dynArray[i];
			}
		}
	}

	template<class T>
	inline array<T>::array(array<T>&& dynArray)
	{
		if (this == &dynArray)
		{
			return *this;
		}

		if (dynArray._size == 0)
		{
			delete[] _arr;
			_size = 0;
			_cap = 1;
			return *this;
		}

		if (_arr != nullptr)
		{
			for (size_t i = 0; i < _cap; i++)
			{
				_arr[i].~T();
			}
			delete[] _arr;
		}

		_arr = dynArray._arr;
		_size = dynArray._size;
		_cap = dynArray._cap;

		dynArray._arr = nullptr;
		dynArray._size = 0;
		dynArray._cap = 1;

		return *this;
	}

	template<class T>
	inline array<T>& array<T>::operator=(const array<T>& dynArray)
	{
		if (this != &dynArray)
		{
			delete[] _arr;
			_size = dynArray._size;
			_cap = dynArray._cap;
			_arr = new T[_cap];
			for (size_t i = 0; i < _size; i++)
			{
				_arr[i] = dynArray[i];
			}
		}
		return *this;
	}

	template<class T>
	inline array<T>& array<T>::operator=(array<T>&& dynArray)
	{
		if (this == &dynArray)
		{
			return *this;
		}

		if (dynArray._size == 0)
		{
			delete[] _arr;
			_size = 0;
			_cap = 1;
			return *this;
		}

		if (_arr != nullptr)
		{
			for (size_t i = 0; i < _cap; i++)
			{
				_arr[i].~T();
			}
			delete[] _arr;
		}

		_arr = dynArray._arr;
		_size = dynArray._size;
		_cap = dynArray._cap;

		dynArray._arr = nullptr;
		dynArray._size = 0;
		dynArray._cap = 1;

		return *this;
	}

	template<class T>
	inline T& array<T>::operator[](size_t index) const
	{
		if (index < 0 || index > _size)
		{
			std::cout << "!--- ERROR: Out Of Bounds ---!" << std::endl;
		}
		else
		{
			return _arr[index];
		}
	}

	template<class T>
	inline bool array<T>::operator<(const array<T>& dynArray) const
	{
		_size < dynArray._size ? true : false;
	}

	template<class T>
	inline bool array<T>::operator>(const array<T>& dynArray) const
	{
		_size > dynArray._size ? true : false;
	}

	template<class T>
	inline bool array<T>::operator==(const array<T>& dynArray) const
	{
		if (this < dynArray || this > dynArray)
		{
			return false;
		}
		for (size_t i = 0; i < _size; i++)
		{
			if (_arr[i] != dynArray[i])
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
		if (_size == _cap)
		{
			expand();
		}
		_size++;
		_arr[_size - 1] = value;
	}

	template<class T>
	inline void array<T>::generate(T value, size_t length)
	{
		if (length < 0 || length > _cap)
		{
			std::cout << "!--- ERROR: Out Of Bounds ---!" << std::endl;
			return;
		}
		for (size_t i = 0; i < length; i++)
		{
			expand();
		}
		std::cout << _cap << std::endl;
		_size = _size + length;
		for (size_t i = _size - length; i < _size; i++)
		{
			_arr[i] = value;
		}
	}

	template<class T>
	inline void array<T>::remove()
	{
		_size--;
		T* tempArr = new T[_size];
		for (size_t i = 0; i < _size; i++)
		{
			tempArr[i] = _arr[i];
		}
		delete[] _arr;
		_arr = tempArr;
	}

	template<class T>
	inline void array<T>::remove(size_t index)
	{
		if (index < 0 || index > _size)
		{
			std::cout << "!--- ERROR: Out Of Bounds! ---!" << std::endl;
			return;
		}
		T* tempArr = new T[_size];
		for (size_t i = index - 1; i < _size; i++)
		{
			_arr[i] = _arr[i + 1];
		}
		remove();
	}

	template<class T>
	inline size_t array<T>::size() const
	{
		return _size;
	}

	template<class T>
	inline bool array<T>::empty() const
	{
		return _size == 0 ? true : false;
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
		for (size_t i = 0; i < _size; i++)
		{
			std::cout << " " << _arr[i];
		}
	}

	template<class T>
	inline array<T>::~array()
	{
		delete[] _arr;
	}

}


#pragma once
#include <vector>
#include <stdexcept>

template <typename T, int _maxSize>
class MaxSizeVector : public std::vector<T>
{
protected:
	//int _maxSize;
public:
	//void setMaxSize(int max) { this->_maxSize = max; }
	void push_back(const T& element)
	{
		int listSize = std::vector<T>::size();
		if (listSize < _maxSize)
		{
			std::vector<T>::push_back(element);
		}
		else
		{
			throw std::out_of_range("Element cannot be added");
		}
	}

};
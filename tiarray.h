#pragma once
#include <iostream>
#include <exception>
#include <limits>
#include <random>
#include <string>
//--------------------------------------------------------------------------------------------------------------------
template<typename Y> class CTypeArray{
public:
	CTypeArray() :_size(0), _data(nullptr) {};
	CTypeArray(int);
	CTypeArray(const CTypeArray&);
	~CTypeArray();


	auto size() -> int const { return _size;}
	auto operator=(const CTypeArray&)->CTypeArray&;
	auto operator[](const int&) ->Y&;

	friend std::ostream& operator<<(std::ostream& out, const CTypeArray<Y>& arr)
	{
		if (arr._data) {
			for (int i = 0; i < arr._size; i++) {
				out << "Item #" << i << ": " << arr._data[i] << std::endl;
			}
		}
		else out << "The array is empty" << std::endl;
		return out;
	}

	auto insert(Y, int = 0)->int;
	auto add(Y)-> int;
	auto remove(int)->int;
	auto clear()->int;
	auto resize(int)->int;
	auto index_of(Y)->int;
	void random_fill();
	void Show();


private:
	int _size;
	Y* _data;
};
//--------------------------------------------------------------------------------------------------------------------
class CBadSizeException :public std::exception {
public:
	CBadSizeException(int v) {
		_info = "Invalid array size passed to constructor:";
		_info += std::to_string(v);
	};
	virtual const char* what() const override { return _info.c_str(); };
private:
	std::string _info;
};
//--------------------------------------------------------------------------------------------------------------------
class CBadRangeException :public std::exception {
public:
	CBadRangeException(int v) {
		_info = "Invalid array item index:";
		_info += std::to_string(v);
	};
	virtual const char* what() const override { return	_info.c_str(); }
private:
	std::string _info;
};
//--------------------------------------------------------------------------------------------------------------------

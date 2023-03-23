#include "tiarray.h"
//====================================================================================================
template<typename Y> CTypeArray<Y>::CTypeArray(int sz):_size(sz) {
	if (sz < 1 || sz >= INT_MAX)
		throw CBadSizeException(sz);

	try {
		_data = new Y[sz] {};
	}
	catch (const std::bad_alloc& e) {
		std::cout << e.what() << std::endl;
		throw;
	}
}
//-----------------------------------------------------------------------------------------------------
template<typename Y>
CTypeArray<Y>::CTypeArray(const CTypeArray& other):_size(other._size) {
	try {
		_data = new Y[other._size];
	}
	catch (const std::bad_alloc& e) {
		std::cout << e.what() << std::endl;
		throw;
	}
	for (int i = 0; i < other._size; i++)
		_data[i] = other._data[i];
}
//-----------------------------------------------------------------------------------------------------
template<typename Y>
CTypeArray<Y>::~CTypeArray(){
	if (_data) {
		for (int i = 0; i < _size; i++)
			delete _data[i];
		delete[] _data;
	}	
}
//-----------------------------------------------------------------------------------------------------
template<typename Y>
auto CTypeArray<Y>::operator=(const CTypeArray& rhs)->CTypeArray& {
	if (this == &rhs)
		return *this;
	clear();

	try {
		_data = { new Y[rhs._size] };
	}
	catch (const std::bad_alloc& e) {
		std::cout << e.what() << std::endl;
		throw;
	}

	for (int i = 0; i < rhs._size; i++)
		_data[i] = rhs._data[i];

	_size = rhs._size;
	return *this;
}
//-----------------------------------------------------------------------------------------------------
template<typename Y>
auto CTypeArray<Y>::operator[](const int& idx)->Y& {
	if (idx < 0 || idx >= _size)
		throw CBadRangeException(idx);
	return _data[idx];
}
//-----------------------------------------------------------------------------------------------------
template<typename Y>
auto CTypeArray<Y>::add(Y val) -> int {
	if (!_data) {
		_data = new Y[1] {val};
		_size = 1;
	}
	else {
		Y* tmp_arr = nullptr;
		try {
			tmp_arr = new Y[_size + 1];
		}
		catch (const std::bad_alloc& e) {
			std::cout << e.what() << std::endl;
			throw;
		}
		for (int i = 0; i < _size; i++)
			tmp_arr[i] = _data[i];

		tmp_arr[_size++] = val;
		delete[] _data;
		_data = tmp_arr;
	}
	return _size;
}
//-----------------------------------------------------------------------------------------------------
//if idx 0 - adding to begining of array
template<typename Y>
auto CTypeArray<Y>::insert(Y val, int idx) -> int {
	if (idx < 0)
		throw CBadRangeException(idx);

	if (!_data) {
		if (!idx) {
			_data = new Y[1] {val};
			_size = 1;
		}
		else {
			try {
				_data = new Y[idx] {};
			}
			catch (const std::bad_alloc& e) {
				std::cout << e.what() << std::endl;
				throw;
			}
			_data[idx - 1] = val;
			_size = idx;
		}
	}
	else {
		Y* tmp_arr = nullptr;

		int new_size{ _size + 1 + (idx > _size ? idx - _size : 0) };

		try {
			tmp_arr = new Y[new_size]{};
		}
		catch (const std::bad_alloc& e) {
			std::cout << e.what() << std::endl;
			throw;
		}

		if (!idx) {
			tmp_arr[0] = val;
			for (int i = 0, j = 1; i < _size; i++, j++)
				tmp_arr[j] = _data[i];
		}
		else {
			for (int i = 0; i < (idx <= _size ? idx : _size); i++)
				tmp_arr[i] = _data[i];

			tmp_arr[idx] = val;

			for (int i = idx, j = idx + 1; j < new_size; i++, j++)
				tmp_arr[j] = _data[i];
		}
		delete[] _data;
		_data = tmp_arr;
		_size = new_size;
	}
	return _size;
}
//-----------------------------------------------------------------------------------------------------
template<typename Y>
auto CTypeArray<Y>::remove(int idx) -> int {
	int res(-1);

	if (idx < 0 || idx >= _size)
		return res;
	if (_data) {
		Y* tmp_data = new Y[_size - 1] {};
		for (int i = 0; i < idx; i++)
			tmp_data[i] = _data[i];

		for (int i = idx, j = ++idx; j <= (_size - 1); i++, j++)
			tmp_data[i] = _data[j];
		delete[] _data;
		_data = tmp_data;
		_size--;
		res = _size;
	}
	return res;
}
//-----------------------------------------------------------------------------------------------------
template<typename Y>
auto CTypeArray<Y>::clear() -> int {
	if (_data) {
		for (int i = 0; i < _size; i++)
			delete _data[i];
		delete[] _data;
		_data = nullptr;
	}
	_size = 0;
	return _size;
}
//-----------------------------------------------------------------------------------------------------
template<typename Y>
auto CTypeArray<Y>::resize(int new_size) -> int {
	if (_size == new_size)
		return _size;

	if (new_size <= 1 || new_size >= INT_MAX)
		throw CBadSizeException(new_size);

	Y* tmp_arr = nullptr;

	try {
		tmp_arr = new Y[new_size] {};
	}
	catch (const std::bad_alloc& e) {
		std::cout << e.what() << std::endl;
		throw;
	}

	for (int i = 0; i < (new_size >= _size ? _size : new_size); i++) {
		tmp_arr[i] = _data[i];
	}

	delete[] _data;
	_data = tmp_arr;
	_size = new_size;

	return _size;
}
//-----------------------------------------------------------------------------------------------------
template<typename Y>
auto CTypeArray<Y>::index_of(Y val) -> int {
	int res(-1);
	if (_data) {
		for (int i = 0; i < _size; i++) {
			if (val == _data[i]) {
				res = i;
				break;
			}
		}
	}
	return res;
}
//-----------------------------------------------------------------------------------------------------
template<typename Y>
void CTypeArray<Y>::random_fill(){
	std::random_device rd;
	std::uniform_int_distribution<int> dist(0, _size - 1);

	for (int i = 0; i < _size; i++) {
		if (typeid(_data[i]) == typeid(float))
			_data[i] = dist(rd) * 2.169f;
		else if (typeid(_data[i]) == typeid(char))
			_data[i] = static_cast<char>(dist(rd));
		else if (typeid(_data[i]) == typeid(std::string))
			_data[i] = dist(rd);//??
		else
			_data[i] = dist(rd);
	}
}
//-----------------------------------------------------------------------------------------------------

template<typename Y>
void CTypeArray<Y>::Show() {
	if (_data) {
		for (int i = 0; i < _size; i++) {
			std::cout << "Item #" << i << ": " << _data[i] << std::endl;
		}
	}
	else std::cout << "The array is empty" << std::endl;
}
//-----------------------------------------------------------------------------------------------------

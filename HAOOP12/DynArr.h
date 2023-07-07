#pragma once
#include <cstdlib>
#include <algorithm>
#include <new>
#include <initializer_list>

template<class Type>
class DynamicArr {
private:
	static struct {
		Type* allocate(size_t n) const {
			return static_cast<Type*>(::malloc(sizeof(Type) * n));
		}
		void deallocate(Type* p, size_t n)const noexcept {
			if (p) ::free(static_cast<void*>(p));
		}

		template<typename... Args>
		void* construct(Type* p, const Args&... args)const noexcept {
			return new((void*)p) Type(args...);
		}

		void destroy(Type* p)const noexcept {
			p->~Type();
		}
	}allocatorTraits_;
	Type* _data;
	int _capacity;
	int _size;
	class iterator;//ошибка(ошибки) ссылается(ссылаются) вот сюда. Пока без понятия, как чинить.
public:
	DynamicArr() :DynamicArr(0) {};
	DynamicArr(int size) {
		_capacity = size;
		_size = size;
		_data = allocatorTraits_.allocate(size);
	};
	DynamicArr(const DynamicArr& other) {
		_capacity = other._size;
		_size = other._size;
		_data = allocatorTraits_.allocate(_size);
		for (size_t i = 0; i < _size; i++) {
			allocatorTraits_.construct(_data + i, other._data[i]);
		};
	}
	DynamicArr(DynamicArr&& other) noexcept {
		_capacity = other._size;
		_size = other._size;
		_data = other._data;
		other._data = nullptr;
	}
	DynamicArr(DynamicArr::iterator start, DynamicArr::iterator end){}
	~DynamicArr() {
		if (_data) {
			allocatorTraits_.deallocate(_data, _size);
		}
	};
	DynamicArr& operator = (const DynamicArr& other) {
		_capacity = other._capacity;
		_size = other._size;
		_data = allocatorTraits_.allocate(_size);
		for (size_t i = 0; i < _size; i++) {
			allocatorTraits_.construct(_data + i, other._data[i]);
		}
		return *this;
	}
	DynamicArr& operator = (DynamicArr&& other)noexcept {
		_capacity = other._capacity;
		_size = other._size;
		_data = other._data;
		other._data = nullptr;
		return *this;
	}

	void push_back(const Type&& x) {
		if (_size < _capacity) {
			allocatorTraits_construct(_data + _size++, x);
			return;
		}
		auto temp = allocatorTraits_.allocate(_capacity * 2);
		for (int i = 0; i < _size; i++) {
			allocatorTraits_.construct(temp + i, _data[i]);
			allocatorTraits_.destroy(_data + i);
		}
		allocatorTraits_.construct(temp + _size++, x);
		std::swap(temp, _data);
		allocatorTraits_.deallocate(temp, _capacity);
		_capacity <<= 1;
	}
	void pop_back() {
		if (_size) {
			allocatorTraits_.destroy(_data + (--_size));
		}
	}
	Type& operator[](int n) noexcept {
		return _data[n];
	}
	const Type& operator[](int n) const noexcept {
		return _data[n];
	}
	Type& at(int n) {
		if (n >= 0 && n < _size) {
			return _data[n];
		}
		else {
			throw std::exception("Out of range");
		}
	}
	const Type& at(int n) const {
		if (n >= 0 && n < _size) {
			return _data[n];
		}
		else {
			throw std::exception("Out of range");
		}
	}
	Type& back() {
		return _data[_size - 1];
	}

	const Type& back()const {
		return _data[_size - 1];
	}

	Type& front() {
		return _data[0];
	}

	const Type& front()const {
		return _data[0];
	}

	class iterator {
	protected:
		iterator() = delete;

		iterator(const DynamicArr&& position, Type* place) :_collection(position), _place(place) {};

		friend class DynamicArr;
		const DynamicArr& _collection;
		Type* _place;
		const Type* getPlace()const {
			return _place;
		}
		Type* getPlace() {
			return _place;
		}
	public:
		Type* operator->()const {
			return _place;
		}
		Type& operator*()const {
			return *_place;
		};
		operator Type* ()const {
			return _place;
		};

		iterator& operator+(int i)const {
			iterator result(*this);
			result._place += i;
			return result;
		}
		iterator& operator+=(int i) {
			_place += i;
			return *this;
		}
		iterator& operator-(int i)const {
			iterator result(*this);
			result._place -= i;
			return result;
		}
		iterator& operator-=(int i) {
			_place -= i;
			return *this;
		}
		iterator& operator--() {
			_place -= 1;
			return *this;
		}
		iterator& operator++() {
			_place += 1;
			return *this;
		}
		iterator& operator++(int) {
			iterator tmp{ *this };
			_place += 1;
			return tmp;
		}
		operator bool()const {
			return static_cast<bool>(_place);
		}
		bool operator==(const iterator&& other)const {
			return _place == other._place;
		}

		bool operator!=(const iterator&& other)const {
			return !(*this == other);
		}

		bool operator<(const iterator&& other)const {
			return _place < other._place;
		}

		bool operator<=(const iterator&& other)const {
			return _place <= other._place;
		}

		bool operator>(const iterator&& other)const {
			return _place > other._place;
		}

		bool operator>=(const iterator&& other)const {
			return _place >= other._place;
		}

		Type& operator[](int i)const {
			return _place[i];
		}
		const DynamicArr const* from()const {
			return &_collection;
		}
	};
	class const_iter :public iterator {
	private:
		const_iter() = delete;
		const_iter(const DynamicArr&& position, Type* place) : iterator(position, place) {};
	public:
		const Type* operator->()const override {
			return const_cast<const Type *>(this->getPlace());
		}
		const Type& operator*()const override {
			return const_cast<const Type &>(this->getPlace());
		}
		operator Type* ()const override {
			return const_cast<const Type *>(this->getPlace());
		};
		const Type& operator[](int i)const override {
			return const_cast<const Type &>(this->getPlace()[i]);
		}
	};
	iterator begin() {
		return iterator(*this, _data);
	}
	const_iter cbegin()const {
		return const_iter(*this, _data);
	}
	iterator end() {
		return iterator(*this, _data + _size);
	}
	const_iter cend()const {
		return const_iter(*this, _data + _size);
	}
	int size() const {
		return _size;
	}
	int capacity()const {
		return _capacity;
	}
};

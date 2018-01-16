
#define _SCL_SECURE_NO_WARNINGS

#include <iostream>
#include <stdlib.h>
#include <string>
#include <mutex>

using namespace std;

template <typename T>
class Stack
{
private:
	T * array_;
	size_t array_size_;
	size_t count_;
	mutable mutex mutex_;

public:
	Stack();
	Stack(const Stack<T>& Object);
	size_t count() const;
	size_t array_size() const;
	void push(T const &);
	auto pop()->std::shared_ptr<T>;
	void print();
	void swap(Stack<T>&);
	bool empty() const



};



template <typename T>
Stack<T>::Stack() {
	array_ = nullptr;
	array_size_ = 0;
	count_ = 0;
}


template <typename T>
Stack<T>::Stack(const Stack<T>& Object)
{

	lock_guard<mutex> lock(Object.mutex_);
	T* temp = new T[Object.array_size_];
	array_size_ = Object.array_size_;
	count_ = Object.count_;
	array_ = temp;

	try
	{
		copy(Object.array_, Object.array_ + count_, array_);
	}

	catch (...)
	{
		cerr << "ERROR" << endl;
		delete[] array_;
		throw;
	}
}


template <typename T>
size_t Stack<T>::count() const {
	lock_guard<mutex> lock(mutex_);
	return count_;

}


template <typename T>
size_t Stack<T>::array_size() const {
	lock_guard<mutex> lock(mutex_);
	return array_size_;

}

template <typename T>
void Stack<T>::push(T const& value) {


	std::lock_guard<mutex> lock(mutex_);
	if (array_size_ == 0)
	{
		array_size_ = 1;
		array_ = new T[array_size_]();
	}
	else if (array_size_ == count_)
	{

		try {
			array_size_ *= 2;
			T * array_new = new T[array_size_]();
			copy(array_, array_ + count_, array_new);
			delete[] array_;
			array_ = array_new;
		}

		catch (std::bad_alloc)
		{
			cerr << "bad_alloc caught" << endl;
		}

	}
	array_[count_++] = value;
}

template <typename T>
auto Stack<T>::pop() -> std::shared_ptr<T>
{
	lock_guard<mutex> lock(mutex_);

	if (empty())
	{
		throw "Stack is empty";
	}

	else
	{
		--count_;
		return std::make_shared<T>(array_[count_]);
	}
}



template <typename T>
void Stack<T>::print()
{
	lock_guard<std::mutex> lock(mutex_);
	for (unsigned int i = 0; i < count_; ++i)
		cout << array_[i] << " ";
	cout << endl;
}




template <typename T>
void Stack<T>::swap(Stack<T>& Object)
{
	Object.mutex_.lock();
	swap(array_, Object.array_);
	swap(array_size_, Object.array_size_);
	swap(count_, Object.count_);
	Object.mutex_.unlock();
}

template <typename T>
bool Stack<T>::empty() const {
	return (count_ == 0);
}



int main() {

	Stack<int> test;
	test.push(1);
	test.push(2);
	test.push(3);

	test.print();




	system("pause");
	return 0;
}
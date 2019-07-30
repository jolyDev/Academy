#pragma once
#include <functional>

template<typename T_FunctionalClassType>
class ScopedCaller
{
	bool isFunctorValid = false;
	T_FunctionalClassType m_caller;

public:

	ScopedCaller();

	ScopedCaller(T_FunctionalClassType i_scoped_caller);

	void Reset(T_FunctionalClassType i_other);

	void Release();

	~ScopedCaller();

};


template<typename T_FunctionalClassType>
ScopedCaller<T_FunctionalClassType>::ScopedCaller()
{

}

template<typename T_FunctionalClassType>
ScopedCaller<T_FunctionalClassType>::ScopedCaller(T_FunctionalClassType i_scoped_caller) : m_caller(i_scoped_caller), isFunctorValid(true)
{

}

template<typename T_FunctionalClassType>
void ScopedCaller<T_FunctionalClassType>::Reset(T_FunctionalClassType i_other)
{
	m_caller = i_other;
}

template<typename T_FunctionalClassType>
void ScopedCaller<T_FunctionalClassType>::Release()
{
	isFunctorValid = false;
}

template<typename T_FunctionalClassType>
ScopedCaller<T_FunctionalClassType>::~ScopedCaller()
{
	if (isFunctorValid)
		m_caller();
}

void f()
{
	std::cout << "Hello";
}
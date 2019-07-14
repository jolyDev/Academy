#pragma once

#include <functional>
#include <iostream>

template <class TClass>
class SmartPtr
{
public:

	TClass * mp_ref;
	const unsigned int m_ID;
	void( *m_copy_callback )( SmartPtr<TClass> *, unsigned int );
	void( *m_destruct_callback )( SmartPtr<TClass> *, unsigned int );
public:

	SmartPtr( TClass * reference, unsigned int ID)
			  //void( *copy_callback )( SmartPtr<TClass> *, unsigned int ),
			  //void( *destruct_callback )( SmartPtr<TClass> *, unsigned int ))
		: mp_ref( reference ), m_ID( ID )//, m_copy_callback(copy_callback), m_destruct_callback( destruct_callback )
	{
	}

	TClass * get()
	{
		return mp_ref;
	}

	SmartPtr<TClass> &operator=( const SmartPtr<TClass>  & other )
	{
		m_copy_callback( this, m_ID );
	}

	~SmartPtr()
	{
		m_destruct_callback( this, m_ID );
	}

};
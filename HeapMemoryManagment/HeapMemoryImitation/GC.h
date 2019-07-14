#pragma once

#include <unordered_map>

#include "SmartPointer.h"

class GC {

	template <class TClass>
	void UpdateRefCount( SmartPtr<TClass> * obj, unsigned int ID );

	template <class TClass>
	void DeleteManager( SmartPtr<TClass> * obj, unsigned int ID );

public:
	static GC & getInstance();

	template <class TClass>
	SmartPtr<TClass> create( TClass * object )
	{
		m_ID_map.insert( { m_ID, 0 } );

		SmartPtr<TClass> smart_ptr = SmartPtr<TClass>( object, m_ID++ );

		smart_ptr.m_copy_callback = &GC::UpdateRefCount;
		smart_ptr.m_destruct_callback = &GC::DeleteManager;

		return static_cast<SmartPtr<TClass>>( smart_ptr );
	}

	~GC();

private:
	GC();

private:
	static unsigned int m_ID;
	static std::unordered_map<unsigned int, unsigned int> m_ID_map;
};


#include "GC.h"

#include "SmartPointer.h"

unsigned int GC::m_ID = 0;

GC::GC()
{
}


GC & GC::getInstance()
{
	static GC instance = GC();

	return instance;
}

//
//template <typename TClass>
//inline SmartPtr<TClass> GC::create( TClass * object )
//{
//	m_ID_map.insert( { m_ID, 0 } );
//
//	return SmartPtr<TClass>(object, m_ID++, std::bind(&GC::UpdateRefCount) ,std::bind(&GC::DeleteManager));
//}


template <class TClass>
void GC::UpdateRefCount( SmartPtr<TClass> * obj, unsigned int ID )
{
	auto it = m_ID_map.find( ID );

	if ( it != m_ID_map.end() || it->second == 0)
		it->second++;
	else
		throw new std::exception( "System ERROR: ID not found" );
}


template <typename TClass>
void GC::DeleteManager( SmartPtr<TClass> * obj, unsigned int ID )
{
	auto it = m_ID_map.find( ID );

	if ( it != m_ID_map.end() )
	{
		it->second--;

		if ( it->second == 0 )
		{
			delete obj;
			m_ID_map.erase(ID);
		}
	}
	else
		throw new std::exception( "System ERROR: ID not found" );
}

GC::~GC() {
}

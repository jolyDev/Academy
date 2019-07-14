#pragma once

class Allocator
{
public:

	virtual void* Allocate( size_t i_size ) = 0;

	virtual void Deallocate( void* ip_pointer ) = 0;
};
#pragma once

#include <iostream>

#include "AllocatorInterface.h"

typedef struct allocHeader {
	size_t next;
	size_t prev;
	size_t isFree;
} allocHeader;

struct MyAllocator : public Allocator
{
public:
	MyAllocator(size_t i_size);

	~MyAllocator();

	virtual void* Allocate( size_t i_size ) override final;

	virtual void Deallocate( void* ip_pointer ) override final;

private:
	void* mp_start_buffer;
	void* mp_end_buffer;

private:
	bool isValidPointer(void * point_to_block);

	void print();

	void isOK();

	int nodesLeft();
};

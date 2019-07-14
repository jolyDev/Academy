#pragma once

#include "AllocatorInterface.h"

class SegmentAllocator : public Allocator
{
public:
	
	SegmentAllocator( size_t i_size );

	virtual void* Allocate( size_t i_size ) override final;

	virtual void Deallocate( void* ip_pointer ) override final;

	void showRawMemory();

	~SegmentAllocator();

private:
	unsigned int m_segregated_memory;
	Allocator * mp_sequential_allocator;

	// const options
	static const int mg_BLOCKS_COUNT = 4;
	static const int mg_BASIC_BLOCKS_SIZE [ mg_BLOCKS_COUNT ];

	const float m_BLOCKS_SIZE_KOEF [ mg_BLOCKS_COUNT ] = { 0.2f, 0.2f, 0.2f, 0.2f };

	const float m_INFINITE_BLOCK_KOEF = 0.2f; // 20% takes to blocks that have size more than 64 bytes


	// inf about segrageted memory
	typedef struct nodes_info
	{
		unsigned int service_nodes_count;
		
		unsigned int overall_nodes_count ;

		unsigned int frontier;

		const char * data_begin_ptr;

		const char * data_end_ptr;

		char * data;

	} nodes_info;

	nodes_info m_memory_sectors [ mg_BLOCKS_COUNT ];

};


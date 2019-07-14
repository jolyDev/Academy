#include "SegmentAllocator.h"

#include "Allocator.h"
#include <math.h>       /* ceil */

/*

	const int BASIC_BLOCK_SIZE = 8;
	const int BLOCK_SIZES_KOEF [ 4 ] = { 10, 10, 10, 10 };
	const float INFINITE_BLOCK_KOEF = 0.2f; // 20% takes to blocks that have size more than 64 bytes
	
	*/


const int SegmentAllocator::mg_BASIC_BLOCKS_SIZE [ SegmentAllocator::mg_BLOCKS_COUNT ] = { 8, 16, 32, 64 };

SegmentAllocator::SegmentAllocator( size_t i_size )
{

	for ( int i = 1; true; i++ )
	{
		int memory_sum = 0;

		for ( int j = 0; j < mg_BLOCKS_COUNT; j++ )
		{
			memory_sum += i * m_BLOCKS_SIZE_KOEF [ j ] * mg_BASIC_BLOCKS_SIZE[ 0];
		}

		if ( memory_sum > ( 1 - m_INFINITE_BLOCK_KOEF ) * i_size )
			break;

		m_segregated_memory = memory_sum;
	}

	mp_sequential_allocator = new MyAllocator( i_size - m_segregated_memory );

	if ( m_segregated_memory )
	{
		for ( int i = 0; i < mg_BLOCKS_COUNT; i++ )
		{
			// this value guarantied to be power of 2
			int overall_capacity = (int)ceil( i_size * m_BLOCKS_SIZE_KOEF [ i ] / mg_BASIC_BLOCKS_SIZE [ i ]) * mg_BASIC_BLOCKS_SIZE [ i ];
			unsigned int service_nodes_count = 0;

			for ( int j = 0; true; j++ )
			{
				service_nodes_count++;

				int service_memory_size = service_nodes_count * mg_BASIC_BLOCKS_SIZE [ 0 ] * mg_BASIC_BLOCKS_SIZE[0];

				if ( service_memory_size >= ( overall_capacity - service_memory_size ) / mg_BASIC_BLOCKS_SIZE [ j ] )
					break;
			}

			unsigned int overall_nodes_count = ceil(i_size * m_BLOCKS_SIZE_KOEF [ i ] / mg_BASIC_BLOCKS_SIZE [ i ]);

			char * raw_memory = (new char [ overall_capacity ]);

			for ( int j = 0; j < overall_capacity ; j++)
			{
				raw_memory [ j ] = -1;
			}

			m_memory_sectors [ i ] = { service_nodes_count, overall_nodes_count, 0, raw_memory, raw_memory + overall_capacity, (char *)raw_memory };
		}
	}
}


void SegmentAllocator::showRawMemory()
{
    #define LOG( x ) std::cout << x << std::endl
    #define LOG_INLINE( x ) std::cout << x 

	for ( int i = 0; i < mg_BLOCKS_COUNT; i++ )
	{
		LOG( "-={ " << mg_BASIC_BLOCKS_SIZE[ i] << " }=-" );

		/// >>> ??? 
		int nodes_count = m_memory_sectors[i].overall_nodes_count;
		char * data_ptr = m_memory_sectors[i].data;

		for ( int j = 0; j < nodes_count; j++ )
		{
			for ( int k = 0; k < mg_BASIC_BLOCKS_SIZE [ i ]; k++ )
			{
				if ( k % 4 == 0 && k != 0)
					LOG_INLINE("; ");

				int num = data_ptr[ j * mg_BASIC_BLOCKS_SIZE[ i ] + k ];

				// print every single bit
				for ( int bit_n = 0; bit_n < 8; bit_n++ )
				{
					if ( bit_n == 4)
						LOG_INLINE( " " );

					int bit = (num >> bit_n) & 1;
					LOG_INLINE( bit );
				}
				LOG_INLINE( "." );
			}

			LOG_INLINE( " | " );
		}

	}
}


void* SegmentAllocator::Allocate( size_t i_size )
{

	for (int i = 0; i < mg_BLOCKS_COUNT; i++ )
		if ( i_size <= mg_BASIC_BLOCKS_SIZE [ i ] )
		{
			// iterating through nodes
			for ( int j = 0; j < m_memory_sectors[ i ].service_nodes_count; j++ )
			{
				// iterating through every bit of each node
				for ( int k = 0; k <= mg_BASIC_BLOCKS_SIZE[0]; k++ )
				{
					// check in service data if there is empty place
					char * curr_ptr_data = &m_memory_sectors[ i ].data[ j * mg_BASIC_BLOCKS_SIZE[ 0 ]];
					int bit = ( *curr_ptr_data >> k ) & 1;

					if ( bit )
					{
						// set certain bit to zero 
						// (mark as used memory flag)
						*( int * ) curr_ptr_data ^= ( 0 ^ *( int * ) curr_ptr_data ) & ( 1 << k );
						
						// return shifted value that correspond to that bit
						return curr_ptr_data + m_memory_sectors[ i ].service_nodes_count * mg_BASIC_BLOCKS_SIZE[ 0 ] + k * mg_BASIC_BLOCKS_SIZE[ i ];
					}
				}
			}
		}

		return mp_sequential_allocator->Allocate( i_size );
}

void SegmentAllocator::Deallocate( void* ip_pointer )
{
	for ( int i = 0; i < mg_BLOCKS_COUNT; i++ )
	{
		if ( ( char* ) ip_pointer > m_memory_sectors[ i ].data_begin_ptr
			 && ( char* ) ip_pointer < m_memory_sectors[ i ].data_end_ptr )
		{
			int diff = (char *)ip_pointer - m_memory_sectors[ i ].data_begin_ptr - m_memory_sectors[i].service_nodes_count * mg_BASIC_BLOCKS_SIZE[0];
			diff /= mg_BASIC_BLOCKS_SIZE[ 0 ];

			char * data_ptr = m_memory_sectors[ i ].data;

			while ( diff > 8 )
			{
				diff -= 8;
				data_ptr += 1;
			}

			*( int * ) data_ptr |= 1 << diff;

			return;
		}

	}

	mp_sequential_allocator->Deallocate( ip_pointer );
}


SegmentAllocator::~SegmentAllocator()
{
}

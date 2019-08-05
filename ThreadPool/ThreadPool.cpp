#pragma once

#include "ThreadPool.h"

#include <iostream>


ThreadPool::ThreadPool() : ThreadPool( std::thread::hardware_concurrency() )
{

}

ThreadPool::ThreadPool( int i_thread_count )
{
	m_threads.reserve( i_thread_count );

	for (int i = 0; i < i_thread_count; i++ )
		m_threads.emplace_back( [&] () -> void
		{
			while ( m_is_still_running || m_task_count )
			{
				if ( !m_task_count )
				{
					std::unique_lock < std::mutex > unique_lock( m_mtx );

					while ( !m_task_count )
					{
						if ( !m_is_still_running )
							return;
					
						m_task_availability_condition.wait( unique_lock );
					}
				}

				std::function<void( void )> procces;

				{
					std::lock_guard<std::mutex> lock( m_mtx );

					if ( !m_task_queue.size() )
						continue;
					
					procces = m_task_queue.front();
					m_task_queue.pop_front();
					m_task_count--;
				}

				procces();
			}
		} );
}

void ThreadPool::add( std::function<void( void )> i_func )
{
	{
		std::lock_guard<std::mutex> lock( m_mtx );
		m_task_queue.emplace_back( i_func );
		m_task_count++;
		m_task_availability_condition.notify_all();
	}
}

ThreadPool::~ThreadPool()
{
	m_is_still_running = false;

	for ( auto & th : m_threads )
		th.join();
}

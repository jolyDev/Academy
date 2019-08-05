#pragma once

#include <iostream>
#include <thread>
#include <vector>
#include <deque>
#include <functional>
#include <atomic>
#include <condition_variable>
#include <mutex>


#define STR(x) #x
#define LOG(x) std::cout << x << std::endl

class ThreadPool
{
public:

	ThreadPool();

	ThreadPool( int i_thread_count );

	void add( std::function<void( void )> i_func );

	~ThreadPool();

private:
	std::mutex m_mtx;
	std::condition_variable m_task_availability_condition;

	std::atomic<int> m_task_count = 0;
	std::atomic<bool> m_is_still_running = true;

	std::vector < std::thread > m_threads;
	std::deque < std::function<void( void )> > m_task_queue;
};
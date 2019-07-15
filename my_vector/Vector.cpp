#include "Vector.h"

#include <iostream>

#define LOG_INLINE(x ) std::cout << x

Vector::Vector()
	: Vector(DEFAULT_SIZE)
{
}

Vector::Vector(size_t i_size)
	: m_max_count(i_size), m_frontier(0), mp_data(new int[i_size])
{

}

int * Vector::at(int i_index) const
{
	if (i_index < 0 || i_index >= m_frontier)
		return nullptr;

	return &mp_data[i_index];
}

int * Vector::front() const
{
	return this->at(0);
}

int * Vector::back() const
{
	return this->at(m_frontier - 1);
}

int * Vector::operator[](size_t i_index) const
{
	return &mp_data[i_index];
}

bool Vector::empty() const
{
	return m_frontier == 0;
}

unsigned int Vector::size() const
{
	return m_frontier;
}


void Vector::resize()
{
	int * new_data = new int[m_max_count *= RESIZE_SCALE];

	for (int i = 0; i < m_frontier; i++)
		new_data[i] = mp_data[i];

	for (int i = m_frontier; i < m_max_count; i++)
		new_data[i] = -1; // for debugging

	delete[] mp_data;

	mp_data = new_data;
}

void Vector::reserve(size_t i_size)
{
	if (i_size < m_max_count)
		return;

	int * new_data = new int[i_size];

	for (int i = 0; i < m_frontier; i++)
		new_data[i] = mp_data[i];

	delete[] mp_data;

	mp_data = new_data;
}

void Vector::push_back(int i_element)
{
	if (m_frontier == m_max_count)
		this->resize();
	
	mp_data[m_frontier++] = i_element;
}

int * Vector::pop_back()
{
	if (m_frontier == 0)
		return nullptr;

	return &mp_data[--m_frontier];
}

void Vector::showRawMemory()
{
	for (int i = 0; i < m_max_count; i++)
	{
		if (i % 16 == 0)
			LOG_INLINE("\n");
		LOG_INLINE(mp_data[i] << " ");
	}

	LOG_INLINE("\n");
}

Vector::~Vector()
{
	delete[] mp_data;
}


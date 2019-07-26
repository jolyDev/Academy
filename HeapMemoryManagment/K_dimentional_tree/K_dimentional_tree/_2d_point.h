#pragma once

#include <cmath>

template <typename T>
struct _2d_point
{
	T m_x;
	T m_y;

	_2d_point<T> operator+(const _2d_point<T> & i_other)
	{
		return { this->m_x - i_other.m_x, this->m_y - i_other.m_y };
	}

	_2d_point<T> operator-(const _2d_point<T> & i_other)
	{
		return { this->m_x - i_other.m_x, this->m_y - i_other.m_y };
	}

	bool operator ==(const _2d_point<T> & i_other) const;

	double distanceTo(const _2d_point<T> & i_other);
	
	friend std::ostream& operator<< (std::ostream& i_stream, const _2d_point& i_entity)
	{
		i_stream << i_entity.m_x << ' ' << i_entity.m_y;
		return i_stream;
	}
};

template<typename T>
inline bool _2d_point<T>::operator==(const _2d_point<T> & i_other) const
{
	return this->m_x == i_other.m_x && this->m_y == i_other.m_y;
}

template<typename T>
inline double _2d_point<T>::distanceTo(const _2d_point<T> & i_other)
{
	_2d_point<T> dir_vec = *this - i_other;
	return sqrt(pow(dir_vec.m_x, 2) + pow(dir_vec.m_y, 2));
}

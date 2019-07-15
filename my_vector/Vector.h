#pragma once
class Vector
{

public:

	Vector();

	Vector(size_t i_size);
	
	int * at(int i_index) const;

	int * front() const;

	int * back() const;

	int * operator[](size_t i_index) const;

	bool empty() const;

	unsigned int size() const;

	void reserve(size_t i_size);

	void push_back(int i_element);

	int * pop_back();

	~Vector();

	/* Used for debugging purpose */

	void showRawMemory();

private:

	#define DEFAULT_SIZE 16
	#define RESIZE_SCALE 4

	int * mp_data;

	unsigned int m_max_count;

	int m_frontier;

private:

	void resize();

};


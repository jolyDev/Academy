#include <iostream>

#include "Allocator.h"

#define BASIC_SECTOR_SIZE 8


MyAllocator::MyAllocator(size_t i_size)
	: mp_start_buffer(new char[i_size])
	, mp_end_buffer((char*)mp_start_buffer + i_size) {

	// for debugging purpose
	for (size_t * p = (size_t *)mp_start_buffer; p < mp_end_buffer; p++)
		*p = 0;

	// initial header
	((allocHeader *)mp_start_buffer)[0] = { i_size, 0, true };
}

MyAllocator::~MyAllocator() {
	delete[] mp_start_buffer;
}

void* MyAllocator::Allocate(size_t i_size) {

	allocHeader * head = (allocHeader *)mp_start_buffer;

	while (head != mp_end_buffer) {
		if (head->isFree && i_size + 2 * sizeof(*head) + 1 < head->next) {

			// setup new header
			size_t new_block_size = i_size + sizeof(*head);
			allocHeader * new_block = (allocHeader *)((char *)head + sizeof(*head) + i_size);

			*new_block = { head->next - new_block_size, new_block_size, true };

			// fill allocated node
			head->isFree = false;
			head->next = i_size + sizeof(*head);

			// next block should know that previous header has changed
			allocHeader * ptr = (allocHeader *)((char *)new_block + new_block->next);
			
			if (ptr < mp_end_buffer)
				ptr->prev -= new_block_size;
		
			return (char *)head + sizeof(*head);
		}

		// switch to the next node
		head = (allocHeader *)((char *)head + head->next);
	}

	return nullptr;
}


void MyAllocator::Deallocate(void* ip_pointer) {

	allocHeader * block_pointer = (allocHeader *)ip_pointer - 1;
	
	if (!isValidPointer(block_pointer))
		throw new std::exception("Can`t deallocate -> invalid pointer input");

	block_pointer->isFree = true;

	allocHeader * prev_block = block_pointer->prev != 0 ? (allocHeader *)((char *)block_pointer - block_pointer->prev) : nullptr;
	allocHeader * next_block = (((char *)block_pointer + block_pointer->next) < (char *)mp_end_buffer) ? (allocHeader *)((char *)block_pointer + block_pointer->next) : nullptr;
	

		
	// handling prev node if dealocating node isn`t first
	if (prev_block) 
	{

		if (prev_block->isFree) 
		{
			// deleting reference to this block
			prev_block->next += block_pointer->next;

			// setting prev.next to this.next
			allocHeader * next_next_block = (allocHeader *)((char *)block_pointer + block_pointer->next);

			if ((char *)next_next_block < (char *)mp_end_buffer)
				next_next_block->prev += block_pointer->prev;
			
			// now previos header is main
			block_pointer = prev_block;
		}

	}	

	if (next_block && next_block->isFree) 
	{
		block_pointer->next += next_block->next;

		allocHeader * next_next_block = (allocHeader *)((char *)block_pointer + block_pointer->next);

		if ((char *)next_next_block < (char *)mp_end_buffer)
			next_next_block->prev = block_pointer->next;
	}

}

void MyAllocator::print() {
	for (size_t * p = (size_t *)mp_start_buffer; p < mp_end_buffer; p++)
		std::cout << " " << *p << " ";
	std::cout << std::endl;
}

void MyAllocator::isOK() {
	allocHeader * head = (allocHeader *)mp_start_buffer;

	int c = 0;
	while (head < mp_end_buffer) {
		if (head->isFree)
			c++;
		else c = 0;

		if (c > 1)
			nodesLeft();
		
		head = (allocHeader *)((char *)head + head->next);
	}
}

int MyAllocator::nodesLeft() {
	allocHeader * head = (allocHeader *)mp_start_buffer;

	int count = 0;
	int active = 0;
	int nonactive = 0;
	while (head < mp_end_buffer) {
		if (head->isFree)
			active++;
		else nonactive++;

		count++;

		std::cout << "[ " << head->next << " ] " << " | " << head->prev << " | " << head->isFree << std::endl;
		head = (allocHeader *)((char *)head + head->next);
	}
	std::cout << "active: " << active << " |nonactive: " << nonactive << " overall [ " << nonactive + active << "] " << std::endl;

	return count;
}

bool MyAllocator::isValidPointer(void * point_to_block) {

	if (!point_to_block)
		return false;

	allocHeader * head = (allocHeader *)mp_start_buffer;
	allocHeader * prev = nullptr;

	while (head != mp_end_buffer && head->next != 0) {
		if (head == point_to_block)
			return true;

		prev = head;
		head = (allocHeader *)((char *)head + head->next);

		// user can write over our headers, 
		// so we check is structure of headers is not destruct
		if ((head < mp_end_buffer && prev->next != head->prev)
			|| 
			(head->isFree != 0 && head->isFree != 1))
			throw new std::exception(" Åhe structure of the heap was broken (writing on memory which is out of bounds of allocated memory) ");

	}
	return false;
}
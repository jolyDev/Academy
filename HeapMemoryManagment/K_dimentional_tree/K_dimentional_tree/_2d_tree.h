#pragma once

#include <vector>
#include <algorithm>    // std::sort
#include <queue>
#include <limits>

#include "_2d_point.h"

#define LOG(x) std::cout << x << std::endl
#define LOG_INLINE(x) std::cout << x

template <typename T>
class _2d_tree
{
public:
	class Node;
private:
	
	const int m_levels_count = 2;

	Node * root = nullptr;

	double m_current_best_distance = std::numeric_limits<double>::max();

	class Node
	{
	public:
		const _2d_point<T> data;
		Node * left = nullptr;
		Node * right = nullptr;
	
		Node(_2d_point<T> i_point) : data(i_point)
		{

		}
	
	};

	void buildTree(std::vector<T> i_container_X, std::vector<T> i_container_Y, int i_l, int i_r, bool i_lookingBy_X);

	Node * findClosest(_2d_point<T> i_element, Node * ip_curr, int i_level)
		{
			double potential_best_distance;
			double distance_between_this_two_dots = i_element.distanceTo(ip_curr->data);

			if (!i_level)
				potential_best_distance = abs(i_element.m_x - ip_curr->data.m_x);
			else
				potential_best_distance = abs(i_element.m_y - ip_curr->data.m_y);


			bool shouldCheckOtherSubTree = false;
			if (potential_best_distance < m_current_best_distance)
			{
				m_current_best_distance = potential_best_distance;
				shouldCheckOtherSubTree = true;
			}

			Node * result = nullptr;
			bool isNextStepLeft = false;

			if (!i_level)
				isNextStepLeft = ip_curr->data.m_x < i_element.m_x;
			else
				isNextStepLeft = ip_curr->data.m_y < i_element.m_y;

			if (isNextStepLeft)
			{
				if (ip_curr->left == nullptr)
					result = ip_curr;
				else
					result = findClosest(i_element, ip_curr->left, ++i_level % 2);

				Node * temp = nullptr;
				if (shouldCheckOtherSubTree)
					if (ip_curr->right != nullptr)
						temp = findClosest(i_element, ip_curr->right, ++i_level % 2);

				if (temp && i_element.distanceTo(temp->data) < i_element.distanceTo(result->data))
					result = temp;

			}
			else
			{
				if (ip_curr->right == nullptr)
					result = ip_curr;
				else
					result = findClosest(i_element, ip_curr->right, ++i_level % 2);

				Node * temp = nullptr;
				if (shouldCheckOtherSubTree)
					if (ip_curr->left != nullptr)
						temp = findClosest(i_element, ip_curr->left, ++i_level % 2);

				if (temp && i_element.distanceTo(temp->data) < i_element.distanceTo(result->data))
					result = temp;

			}
			return result;
	}

public:
	_2d_tree(std::vector<_2d_point<T>> i_points);

	void add(_2d_point<T> i_child, Node * ip_curr = nullptr, int i_level = 0);

	bool isInside(_2d_point<T> i_element);

	_2d_point<T> findClosest(_2d_point<T> i_element);

	std::vector<_2d_point<T>> find(T i_distance);

	void levelOrderPrint();

	~_2d_tree();
};

template<typename T>
inline void _2d_tree<T>::buildTree(std::vector<T> i_container_X, std::vector<T> i_container_Y, int i_l, int i_r, bool i_lookingBy_X)
{
 // @TODO
}




/*
There are several unnecessary 
actions in this implementation,
but this is done 
to perfectly balance the tree
*/
template <typename T>
_2d_tree<T>::_2d_tree(std::vector<_2d_point<T>> i_points)
{
	/// Yeah, deep copy!!!
	std::vector<_2d_point<T>> sorted_by_X = i_points;
	std::vector<_2d_point<T>> sorted_by_Y = i_points;

	std::vector<bool> is_Touched(i_points.size());
	
	auto it = sorted_by_X.begin();


	std::sort(sorted_by_X.begin(), sorted_by_X.end(), [](_2d_point<T> i_el1, _2d_point<T> i_el2) -> bool { return i_el1.m_x < i_el2.m_x; });
	std::sort(sorted_by_Y.begin(), sorted_by_Y.end(), [](_2d_point<T> i_el1, _2d_point<T> i_el2) -> bool { return i_el1.m_y < i_el2.m_y; });

	for (int i = 0; i < i_points.size(); i++)
		this->add(i_points[i]);

}

template<typename T>
inline void _2d_tree<T>::add(_2d_point<T> i_child, Node * ip_curr, int i_level)
{
	if (!ip_curr)
		ip_curr = root;

	if (!root)
	{
		root = new Node(i_child);
		return;
	}


	// if it is an X level 
	if (!i_level)
		if (ip_curr->data.m_x < i_child.m_x)
			if (!ip_curr->left)
				ip_curr->left = new Node(i_child);
			else this->add(i_child, ip_curr->left, 1);
		else 
			if (!ip_curr->right)
				ip_curr->right = new Node(i_child);
			else this->add(i_child, ip_curr->right, 1);
	else
		if (ip_curr->data.m_y < i_child.m_y)
			if (!ip_curr->left)
				ip_curr->left = new Node(i_child);
			else this->add(i_child, ip_curr->left, 0);
		else
			if (!ip_curr->right)
				ip_curr->right = new Node(i_child);
			else this->add(i_child, ip_curr->right, 0);
}

template<typename T>
inline bool _2d_tree<T>::isInside(_2d_point<T> i_element)
{
	if (!root)
		return false;

	Node * curr = root;
	
	int i_level = 0;

	while (curr != nullptr)
	{
		// if it is an X level 
		if (!i_level)
			if (curr->data.m_x < i_element.m_x)
				curr = curr->left;
			else 
				if (curr->data.m_x > i_element.m_x)
					curr = curr->right;
				else 
					if (curr->data == i_element)
						return true;
					else continue;

		// if it is an Y level 
		else 
			if (curr->data.m_y < i_element.m_y)
				curr = curr->left;
			else
				if (curr->data.m_y > i_element.m_y)
					curr = curr->right;
				else 
					if (curr->data == i_element)
						return true;
					else continue;
		i_level = (i_level + 1) % 2;
	}

	return false;
}

template<typename T>
inline _2d_point<T> _2d_tree<T>::findClosest(_2d_point<T> i_element)
{

	if (!root)
		throw std::exception("Empty tree");

	m_current_best_distance = std::numeric_limits<T>::max();

	return findClosest(i_element, root, 0)->data;
}

template <typename T>
void _2d_tree<T>::levelOrderPrint()
{
	if (!root)
		return;

	std::queue<Node*> q;  // using stl
	Node* temp;
	q.push(root);
	while (!q.empty())
	{
		temp = q.front();
		q.pop();
		LOG_INLINE(temp->data << ' ' );  //process node
		if (temp->left)
			q.push(temp->left); //EnQueue
		if (temp->right)
			q.push(temp->right); //EnQueue
	}
}

template<typename T>
inline _2d_tree<T>::~_2d_tree()
{
}

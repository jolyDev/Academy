#include <iostream>

#include "_2d_tree.h"

int main()
{

	_2d_point<float> a{ 3,4 };
	_2d_point<float> b{ 9,8 };
	_2d_point<float> c = a + b;

	std::vector<_2d_point<float>> points;
	points.push_back({ 1, 4 });
	points.push_back({ 9, 1 });
	points.push_back({ 8, 2 });
	points.push_back({ 11, 6 });
	points.push_back({ 4, 12 });
	points.push_back({ 2, -7 });
	points.push_back({ -19, 12 });
	points.push_back({ 13, 14 });

	_2d_tree<float> map(points);

	map.levelOrderPrint();
	//c = map.findClosest({ 4.1, 11.9 });
	LOG(' ');
	LOG(map.findClosest({4.1, 11.9}));
	LOG(map.findClosest({ 4.1, 11.9 }));
	LOG(map.findClosest({ 5.2, -1.9 }));
	LOG(map.findClosest({ 4.1, 11.9 }));
	LOG(map.findClosest({ 4.1, 11.9 }));
	LOG(map.findClosest({ -10000, 200 }));
	LOG(map.findClosest({ 100, 200 }));

	std::cin.get();

}
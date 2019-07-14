#include "Allocator.h"
#include "GC.h"

#include "SegmentAllocator.h"

#include <vector>
#include <iostream>

#include <windows.h>

#if 1 
//int main() {
//	SegmentAllocator my(1024 * 1024);
//	std::vector<void*> ps;
//	for (int i = 0; i < 10000; i++) {
//		if (rand() % 100 < 40) {
//			if (ps.size() == 0)continue;
//			auto index = rand() % ps.size();
//
//
//			allocHeader * x = (allocHeader *)ps[index];
//
//
//			my.Deallocate(ps[index]);
//			std::cout << "Deallocated " << ps[index] << std::endl;
//			ps[index] = ps.back();
//			ps.pop_back();
//
//		} else {
//			size_t sz = rand() % 512 + 512;
//			auto p = my.Allocate(sz);
//			if (p) {
//				ps.push_back(p);
//				std::cout << "allocated " << ps.back() << std::endl;
//			} else {
//				std::cout << "can't allocate" << std::endl;
//			}
//		}
//	}
//
//	while (!ps.empty()) {
//		auto p = ps.back();
//		my.Deallocate(p);
//		ps.pop_back();
//		std::cout << "Deallocated " << p << std::endl;
//	}
//
//	std::cin.get();
//	return 0;
//}

int main()
{

	/*set window to certain size, so we can have better lookup*/
	HWND console = GetConsoleWindow();
	RECT r;
	GetWindowRect( console, &r ); //stores the console's current dimensions

	MoveWindow( console, r.left, r.top, 660, 1200, true ); // 800 width, 100 height
	/**/
	SegmentAllocator seg_alloc = SegmentAllocator(1024);

	int * ptr = (int *)seg_alloc.Allocate(14);
	*ptr = 0;
	*( int * ) seg_alloc.Allocate( 14 ) = 3;
	*( int * ) seg_alloc.Allocate( 14 ) = 15;
	*( int * ) seg_alloc.Allocate( 14 ) = 123213;
	*( int * ) seg_alloc.Allocate( 14 ) = 94932;
	seg_alloc.Deallocate( ptr );
	*( int * ) seg_alloc.Allocate( 14 ) = 12351;


	//int * ptr3 = ( int * ) seg_alloc.Allocate( 4 );

	//*ptr3 = 16;

	//seg_alloc.Deallocate( ptr3 );

	// ptr3 = ( int * ) seg_alloc.Allocate( 6 );
	// *ptr3 = 244123123;
	// *( ptr + 1 ) = 172138;

	// /* alloc more than 8 bytes */

	seg_alloc.showRawMemory();

	std::cin.get();
}

#else
	class A
	{
	public:

		A()
		{
			LOG( "A" );
		}

		~A()
		{
			LOG( "~A" );
		}
	};

	void func()
	{
		LOG( "1" );
		GC::getInstance().create<A>( new A() );
		LOG( "2" );
	}

	int main()
	{
		LOG( "3" );
	}

#endif
//
//#if 1 {
//int main() {
//	MyAllocator my(1024 * 1024);
//	std::vector<void*> ps;
//	for (int i = 0; i < 10000; i++) {
//		if (rand() % 100 < 40) {
//			if (ps.size() == 0)continue;
//			auto index = rand() % ps.size();
//
//
//			my.isOK();
//			allocHeader * x = (allocHeader *)ps[index];
//			if (((allocHeader *)ps[index])->next == 746) 
//			{
//				my.nodesLeft();
//				x = nullptr;
//			}
//
//			my.Deallocate(ps[index]);
//			std::cout << "Deallocated " << ps[index] << std::endl;
//			ps[index] = ps.back();
//			ps.pop_back();
//
//		} else {
//			size_t sz = rand() % 512 + 512;
//			auto p = my.Allocate(sz);
//			if (p) {
//				ps.push_back(p);
//				std::cout << "allocated " << ps.back() << std::endl;
//			} else {
//				std::cout << "can't allocate" << std::endl;
//			}
//		}
//	}
//
//	while (!ps.empty()) {
//		auto p = ps.back();
//		my.Deallocate(p);
//		ps.pop_back();
//		std::cout << "Deallocated " << p << std::endl;
//	}
//
//	my.nodesLeft();
//	std::cin.get();
//	return 0;
//}
//
//#else
//
//int main() {
//	const size_t M = 4;
//	MyAllocator alloc(60 * M);
//	alloc.print();
//	std::cout << "-===-" << std::endl;
//
//	void* p1 = alloc.Allocate(15 * M);
//
//
//	alloc.print();
//	std::cout << "-===-" << std::endl;
//	void* p2 = alloc.Allocate(1 * M);
//	void* p3 = alloc.Allocate(8 * M);
//
//	alloc.print();
//	std::cout << "-===-" << std::endl;
//	alloc.Deallocate(p1);
//
//	alloc.print();
//	std::cout << "-===-" << std::endl;
//	void* p4 = alloc.Allocate(8 * M);
//
//	alloc.print();
//	std::cout << "-===-" << std::endl;
//	alloc.Deallocate(p3);
//
//	alloc.print();
//	std::cout << "-===-" << std::endl;
//	alloc.Deallocate(p4);
//	alloc.Deallocate(p2);
//
//	alloc.print();
//	std::cout << "-===-" << std::endl;
//	alloc.nodesLeft();
//	std::cin.get();
//}
////
////int main() {
////	const size_t M = 4;
////	MyAllocator alloc(30 * M);
////	void* p1 = alloc.Allocate(5 * M);
////	alloc.print();
////	std::cout << "-===-" << std::endl;
////	// alloc.print();
////	//void* p2 = alloc.Allocate(M);
////	void* p2 = alloc.Allocate(2 * M);
////
////	alloc.print();
////	std::cout << "-===-" << std::endl;
////
////	void* p3 = alloc.Allocate(3 * M);
////	alloc.Deallocate(p1);
////
////	alloc.print();
////	std::cout << "-===-" << std::endl;
////
////	alloc.print();
////	std::cout << "-===-" << std::endl;
////
////	std::cout << "-===-" << std::endl;
////	alloc.print();
////	alloc.Deallocate(p3);
////
////	alloc.Deallocate(p2);
////	alloc.print();
////
////	//std::cout << "-=-" << std::endl;
////	//p2 = alloc.Allocate(1 * M);
////
////	alloc.nodesLeft();
////	std::cin.get();
////
////	return 0;
////}
//#endif
#pragma once
#include <chrono>

namespace SEVIAN {
	class ElapsedTimer
	{

		std::chrono::high_resolution_clock::time_point _start;
		std::chrono::high_resolution_clock::time_point end;
		std::chrono::microseconds duration;
	public:
		void start ();
		void mark ();



	};
}
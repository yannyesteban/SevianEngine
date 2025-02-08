#include "ElapsedTimer.h"
#include <iostream>


namespace SEVIAN {
	void ElapsedTimer::start () {
		_start = std::chrono::high_resolution_clock::now ();
	}

	void ElapsedTimer::mark () {
		end = std::chrono::high_resolution_clock::now ();
		duration = std::chrono::duration_cast<std::chrono::microseconds>(end - _start);
		std::cout << " ---- Tiempo transcurrido: " << duration.count () << " microsegundos\n";
		start ();
	}

}
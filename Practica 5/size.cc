// size.cc

#include <algorithm>    // nth_element
#include <chrono>       // high_resolution_clock
#include <iomanip>      // setw
#include <iostream>     // cout
#include <vector>       // vector

int main()
{
	const unsigned STEPS = 1000000;
	const unsigned LINE_SIZE = 1 << 6;
	const unsigned GAP = 12;
	const unsigned REP = 10;

	std::cout << "#"
	          << std::setw(GAP - 1) << "size"
	          << std::setw(GAP    ) << "time"
	          << std::endl;

	for (unsigned size = 1 << 10; size <= 1 << 26; size <<= 1)
	{
		using namespace std::chrono;

		std::vector<unsigned char> bits(size);
		unsigned tamanio = size - 1;
		std::vector<double> sec(REP);
		for (auto &s: sec)
		{
			auto start = high_resolution_clock::now();

			for (unsigned j = 0; j < STEPS; j++)
			{
				bits.at(j * LINE_SIZE & tamanio)++;
			}

			auto stop = high_resolution_clock::now();
			s = duration_cast<nanoseconds>(stop - start).count() / double(STEPS);
		}

		nth_element(sec.begin(), sec.begin() + sec.size() / 2, sec.end());
		std::cout << std::setw(GAP) << size
		          << std::setw(GAP) << std::fixed << std::setprecision(1)
		          << sec[sec.size() / 2]
		          << std::endl;
	}
}

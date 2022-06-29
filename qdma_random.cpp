#include <QDMAController.h>
#include "mmio.hpp"
#include "dma.hpp"
using namespace std;

int main(){

	init(0x1a);
	// h2c_benchmark_random();
	c2h_benchmark_random();
	// concurrent_random();
	return 0;
}
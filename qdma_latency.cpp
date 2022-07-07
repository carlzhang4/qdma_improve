#include <QDMAController.h>
#include "mmio.hpp"
#include "dma.hpp"
using namespace std;

int main(){

	init(0x1a,128*1024*1024);
	// init(0x3d,128*1024*1024);
	h2c_benchmark_latency(0x1a);
	// h2c_benchmark_latency(0x3d);
	c2h_benchmark_latency(0x1a);
	// c2h_benchmark_latency(0x3d);
	// concurrent_latency();
	return 0;
}
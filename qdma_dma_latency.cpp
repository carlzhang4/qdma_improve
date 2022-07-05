#include <QDMAController.h>
#include "mmio.hpp"
#include "dma.hpp"
using namespace std;

int main(){

	init(0x1a);
	h2c_benchmark_latency();
	// c2h_benchmark_latency();
	// concurrent_latency();
	return 0;
}
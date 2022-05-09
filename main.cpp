#include <QDMAController.h>
#include "mmio.hpp"
#include "dma.hpp"
using namespace std;

int main(){

	init(0x1a);
	h2c_benchmark();
	// c2h_benchmark();
	// benchmark_bridge_write();
	return 0;
}
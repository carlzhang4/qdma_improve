#include <QDMAController.h>
#include "mmio.hpp"
#include "dma.hpp"
using namespace std;



int main(){

	init(0x1a);
	h2c_benchmark();
	// benchmark_bridge_write();

	//bridge reg 512bit
	// uint64_t *value = NULL;
	// uint64_t *res = NULL;
	// posix_memalign((void **)&value, 64 /*alignment */ , 64);
	// posix_memalign((void **)&res, 64 /*alignment */ , 64);
	// for(int i=0;i<8;i++){
	// 	value[i] = 1;
	// 	res[i] = 0;
	// }
	// writeBridge(0, value);
	// writeBridge(1, value);
	// writeBridge(2, value);
	// cout<<endl;
	// readBridge(0,res);
	// for(int i=0;i<8;i++){
	// 	cout<<res[i]<<" ";
	// }
	// cout<<endl;

	// write multiple time,  qdma 55 cycles interval
	// {
	// 	volatile __m512i* bridge = get_bridge_addr();
	// 	bridge_global = bridge;
	// 	__m512i data;
	// 	for(int i=0;i<8;i++){
	// 		data[i] = 1;
	// 	}
	// 	for(int i=0;i<10;i++){
	// 		bridge[i] = data;
	// 	}
	// }

	//read multiple time, qdma 250 cycles interval
	{
		// volatile __m512i* bridge = get_bridge_addr();
		// __m512i data[10];
		// for(int i=0;i<10;i++){
		// 	data[i] = bridge[i];
		// }
		// for(int i=0;i<10;i++){
		// 	for(int j=0;j<8;j++){
		// 		cout<<data[i][j]<<" ";
		// 	}
		// 	cout<<endl;
		// }
	}

	// read benchmark
	{	
		// size_t size = 1l*1024*1024*1024;//byte

		// __m512i* cpu_mem;
		// if(posix_memalign((void **)&cpu_mem, 64 /*alignment */ , size) != 0){
		// 	cout<<"Error on alloc\n";
		// }
		// for(int i=0;i<size/64;i++){
		// 	for(int j=0;j<8;j++){
		// 		cpu_mem[i][j] = 0;
		// 	}
		// }

		// __m512i data[8];
		// for(int i=0;i<8;i++){
		// 	data[i][7] = i+1;
		// 	data[i][6] = i+1;
		// 	data[i][5] = i+1;
		// 	data[i][4] = i+1;
		// 	data[i][3] = i+1;
		// 	data[i][2] = i+1;
		// 	data[i][1] = i+1;
		// 	data[i][0] = i+1;
		// }
		
		// volatile __m512i* bridge = get_bridge_addr();
		// bridge_global = bridge;
		// int sum=0;

		// // if (mlock((void*)bridge, size)) {
		// // 	printf("mlock error\n");
		// // }

		// //start timer
		// struct timespec start_timer,end_timer;
		// clock_gettime(CLOCK_MONOTONIC, &start_timer); 

		// int num_threads = 64;
		// pthread_t tids[num_threads];
		// for(int i = 0; i < num_threads; ++i)
		// {
		// 	int ret = pthread_create(&tids[i], NULL, write_bridge, NULL);
		// 	if (ret != 0)
		// 	{
		// 	cout << "pthread_create error: error_code=" << ret << endl;
		// 	}
		// }
		// for(int i=0;i<num_threads;i++){
		// 	pthread_join(tids[i],NULL);
		// }

		// //end timer
		// clock_gettime(CLOCK_MONOTONIC, &end_timer); 
		// double time = (end_timer.tv_sec-start_timer.tv_sec)+1.0*(end_timer.tv_nsec-start_timer.tv_nsec)/1e9;
		// printf("time:%f s\n",time);
		// printf("speed:%f GB/s\n",size*num_threads/time/1024/1024/1024);
	}

	//write benchmark
	{
		// size_t size = 1l*1024*1024*1024;//byte
		// volatile __m512i* bridge = get_bridge_addr();

		// __m512i data;
		// for(int i=0;i<8;i++){
		// 	data[i] = i;
		// }

		// struct timespec start_timer,end_timer;
		// clock_gettime(CLOCK_MONOTONIC, &start_timer); 

		// for(int i=0;i<size/64;i++){
		// 	bridge[i%1024] = data;
		// }

		// clock_gettime(CLOCK_MONOTONIC, &end_timer); 
		// double time = (end_timer.tv_sec-start_timer.tv_sec)+1.0*(end_timer.tv_nsec-start_timer.tv_nsec)/1e9;
		// printf("time:%f s\n",time);
		// printf("speed:%f GB/s\n",size/time/1024/1024/1024);
	}
	return 0;
}
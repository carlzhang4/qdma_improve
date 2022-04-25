#include "dma.hpp"
#include <stdio.h>
#include <unistd.h>

void h2c_benchmark(){
	void * dmaBuffer = qdma_alloc(1L*1024*1024*1024);
	cout<<hex<<dmaBuffer<<endl;
	uint32_t * bar = (uint32_t*)getLiteAddr();

	uint32_t is_seq = 1;
	uint32_t length = 4*1024;
	uint32_t offset = 0;
	uint32_t total_qs = 1;
	uint32_t total_cmds = 256*1024;
	uint32_t total_words = length/64 * total_cmds;
	uint32_t range = 16*1024*1024;
	uint32_t range_words = range/64;

	unsigned int * p = (unsigned int *) dmaBuffer;

	for(int i=0;i<total_words;i++){//initial
		for(int j=0;j<16;j++){
			p[i*16+j] = i;
		}
	}

	bar[100] = (uint32_t)((unsigned long)p>>32);
	bar[101] = (uint32_t)((unsigned long)p);
	bar[102] = length;
	bar[103] = offset;
	bar[104] = 1;//sop
	bar[105] = 1;//eop
	bar[107] = total_words;
	bar[108] = total_qs;
	bar[109] = total_cmds;
	bar[110] = range;
	bar[111] = range_words;
	bar[112] = is_seq;

	//start
	bar[106] = 0;
	bar[106] = 1;

	sleep(1);
	unsigned int cycles = bar[512+101];

	printf("Number of errors: %d\n",bar[512+100]);
	printf("Cycles: %d\n",cycles);
	double speed = 1.0*length*total_cmds/(1.0*cycles*4/1000/1000/1000)/1024/1024/1024;

	printf("Total length: %d\n\n",total_words*64);
	printf("Speed: %.2f\n",speed);
	printf("\nExpected words per q: %d\n",total_words/total_qs);
	cout<<"Real word per q: ";
	for(int i=0;i<16;i++){
		cout<<bar[512+102+i]<<" ";
	}
	cout<<endl;
}
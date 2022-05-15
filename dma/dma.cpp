#include "dma.hpp"
#include <stdio.h>
#include <unistd.h>

void h2c_benchmark(){
	void * dmaBuffer = qdma_alloc(1L*1024*1024*1024);
	unsigned int * p = (unsigned int *) dmaBuffer;
	volatile uint32_t * bar = (volatile uint32_t*)getLiteAddr();

	uint32_t is_seq = 1;
	uint32_t length = 1*1024;
	uint32_t offset = 0;
	uint32_t total_qs = 4;
	uint32_t total_cmds = 256*1024;
	uint32_t total_words = length/64 * total_cmds;
	uint32_t range = 16*1024*1024;
	uint32_t range_words = range/64;

	

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
	printf("\nExpected words per q: %x\n",total_words/total_qs);
	cout<<"Real word per q: ";
	for(int i=0;i<16;i++){
		cout<<bar[512+102+i]<<" ";
	}
	cout<<endl;
	printCounters();
}

void c2h_benchmark(){
	size_t size = 1L*1024*1024*1024;
	void * dmaBuffer = qdma_alloc(size);
	unsigned int * p = (unsigned int *) dmaBuffer;
	volatile uint32_t * bar = (volatile uint32_t*)getLiteAddr();
	
	for(size_t i=0;i<size/4;i++){
		p[i]=0;
	}
	
	uint32_t length = 32*1024;
	uint32_t offset = 3;
	uint32_t total_cmds = 32*1024;
	uint32_t total_qs = 4;
	uint32_t total_words = length/64 * total_cmds;
	
	bar[200] = (uint32_t)((unsigned long)p>>32);
	bar[201] = (uint32_t)((unsigned long)p);
	bar[202] = length;
	bar[203] = offset;
	bar[205] = total_words;
	bar[206] = total_qs;
	bar[207] = total_cmds;
	resetCounters();

	for(int i=0;i<total_qs;i++){
		writeConfig(0x1408/4,i);
		uint32_t tag = readConfig(0x140c/4);
		bar[209] = tag;
		bar[210] = i+1;
		printf("%d\n",tag&0x7f);
	}
	bar[210] = 0;//reset tag_index

	bar[204] = 0;//start
	bar[204] = 1;

	sleep(2);
	uint32_t count_cmd = bar[512+200];
	uint32_t count_word = bar[512+201];
	uint32_t count_time = bar[512+202];

	printf("count cmd: %d,right: %d\n",count_cmd,total_cmds);
	printf("count word: %d,right: %d\n",count_word,total_words);
	printf("count time: %d\n",count_time);
	
	double speed = 1.0*length*total_cmds/(1.0*count_time*4/1000/1000/1000)/1024/1024/1024;
	printf("Speed: %.2f\n",speed);

	uint32_t right_count=0;
	uint32_t wrong_count=0;
	uint32_t right_val = offset;
	for(int i=0;i<total_words;i++){
		uint32_t val = p[i*16];
		if(right_val != val){
			// printf("%d ",val);
			wrong_count++;
		}else{
			right_count++;
		}
		right_val++;
		for(int j=1;j<16;j++){
			if(p[i*16+j]!=val){
				// printf("Error\n");
			}
		}
	}
	printf("right data count: %d, wrong data count: %d\n",right_count,wrong_count);
	printCounters();
}

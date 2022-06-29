#include "dma.hpp"
#include <stdio.h>
#include <unistd.h>
#include <cmath>

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

void h2c_benchmark_random(){
	size_t size = 1L*1024*1024*1024;
	void * dmaBuffer = qdma_alloc(size);
	size_t * p = (size_t *) dmaBuffer;
	volatile uint32_t * bar = (volatile uint32_t*)getLiteAddr();

	uint32_t burst_length = 256;
	uint32_t busrt_length_shift = (uint32_t)log2(burst_length);
	uint32_t total_qs = 16;
	uint32_t total_cmds = 256*1024;
	uint32_t total_words = burst_length/64 * total_cmds;

	for(int i=0;i<size/64;i++){//initial
		p[i*8] = i*64;//sizeof(size_t)*8=512
	}

	bar[100] = (uint32_t)((unsigned long)p>>32);
	bar[101] = (uint32_t)((unsigned long)p);
	bar[102] = burst_length;
	bar[103] = busrt_length_shift;
	bar[105] = total_words;
	bar[106] = total_qs;
	bar[107] = total_cmds;

	//start
	bar[104] = 0;
	bar[104] = 1;

	sleep(1);
	unsigned int cycles = bar[512+104];

	printf("\n");
	printf("count_err_data:    0x[%x], should be zero\n",bar[512+100]);
	printf("count_right_data:  0x[%x]\n",bar[512+101]);
	printf("count_total_words: 0x[%x]\n",bar[512+102]);
	printf("count_send_cmd:    0x[%x]\n",bar[512+103]);

	printf("Cycles: %d\n",cycles);
	double speed = 1.0*burst_length*total_cmds/(1.0*cycles*4/1000/1000/1000)/1024/1024/1024;

	printf("Total length: %d\n\n",total_words*64);
	printf("Speed: [%.2f] GB/s\n",speed);
	printf("\nExpected words per q: 0x%x\n",total_words/total_qs);
	cout<<"Real words per q: ";
	for(int i=0;i<16;i++){
		cout<<hex<<"0x"<<bar[512+105+i]<<" ";
	}
	cout<<endl;
	printCounters();
}

void c2h_benchmark_random(){
	size_t size = 1L*1024*1024*1024;
	void * dmaBuffer = qdma_alloc(size);
	size_t * p = (size_t *) dmaBuffer;
	volatile uint32_t * bar = (volatile uint32_t*)getLiteAddr();
	
	for(size_t i=0;i<size/sizeof(size_t);i++){
		p[i]=0;
	}
	
	uint32_t burst_length = 1024;//32K has ever triggered the horrible bug
	uint32_t busrt_length_shift = (uint32_t)log2(burst_length);
	uint32_t total_cmds = 32*1024;
	uint32_t total_qs = 4;
	uint32_t total_words = burst_length/64 * total_cmds;
	
	bar[200] = (uint32_t)((unsigned long)p>>32);
	bar[201] = (uint32_t)((unsigned long)p);
	bar[202] = burst_length;
	bar[203] = busrt_length_shift;
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
	uint32_t count_cmds = bar[512+200];
	uint32_t count_words = bar[512+201];
	uint32_t count_time = bar[512+202];

	printf("count_cmds: %d,total: %d\n",count_cmds,total_cmds);
	printf("count_words: %d,total: %d\n",count_words,total_words);
	printf("count time: %d\n",count_time);
	
	double speed = 1.0*burst_length*total_cmds/(1.0*count_time*4/1000/1000/1000)/1024/1024/1024;
	printf("Speed: [%.2f] GB/s\n",speed);

	int count_written_right_word = 0;
	int count_written_word = 0;
	for(int i=0;i<size/64;i++){
		if(p[i*8] != 0){
			count_written_word++;
		}
		if(p[i*8] == i*64){
			count_written_right_word++;
		}
	}
	printf("count_written_word:%d, count_written_right_word:%d, total words:%d\n",count_written_word,count_written_right_word,total_words);
	if(count_time > 2 * 603700){
		printCounters();
	}
}

void concurrent_random(){
	size_t size = 1L*1024*1024*1024;
	void * dmaBuffer = qdma_alloc(size);
	size_t * p = (size_t *) dmaBuffer;
	volatile uint32_t * bar = (volatile uint32_t*)getLiteAddr();

	for(int i=0;i<size/64;i++){//initial
		p[i*8] = i*64;//sizeof(size_t)*8=512
	}

	uint32_t burst_length = 256;
	uint32_t busrt_length_shift = (uint32_t)log2(burst_length);
	uint32_t total_qs = 8;
	uint32_t total_cmds = 256*1024;
	uint32_t total_words = burst_length/64 * total_cmds;

	uint32_t h2c_factor = 1;//smaller one is reliable
	uint32_t c2h_factor = 2;
	
	//h2c
	bar[100] = (uint32_t)((unsigned long)p>>32);
	bar[101] = (uint32_t)((unsigned long)p);
	bar[102] = burst_length;
	bar[103] = busrt_length_shift;
	bar[105] = total_words*h2c_factor;
	bar[106] = total_qs;
	bar[107] = total_cmds*h2c_factor;

	//c2h
	bar[200] = (uint32_t)((unsigned long)p>>32);
	bar[201] = (uint32_t)((unsigned long)p);
	bar[202] = burst_length;
	bar[203] = busrt_length_shift;
	bar[205] = total_words*c2h_factor;
	bar[206] = total_qs;
	bar[207] = total_cmds*c2h_factor;
	resetCounters();

	for(int i=0;i<total_qs;i++){
		writeConfig(0x1408/4,i);
		uint32_t tag = readConfig(0x140c/4);
		bar[209] = tag;
		bar[210] = i+1;
		printf("%d\n",tag&0x7f);
	}
	bar[210] = 0;//reset tag_index


	//start
	bar[104] = 0;
	bar[204] = 0;//start
	bar[104] = 1;
	bar[204] = 1;
	sleep(2);

	printf("H2C statistics:\n");
	unsigned int h2c_cycles = bar[512+104];
	printf("count_err_data:    0x[%x], should be zero\n",bar[512+100]);
	printf("count_right_data:  0x[%x]\n",bar[512+101]);
	printf("count_total_words: 0x[%x]\n",bar[512+102]);
	printf("count_send_cmd:    0x[%x]\n",bar[512+103]);

	printf("H2C Cycles: %d\n",h2c_cycles);
	double h2c_speed = 1.0*h2c_factor*burst_length*total_cmds/(1.0*h2c_cycles*4/1000/1000/1000)/1024/1024/1024;
	printf("Total length: %d\n",total_words*64);
	printf("Expected words per q: 0x%x\n",total_words/total_qs);
	cout<<"Real words per q: ";
	for(int i=0;i<16;i++){
		cout<<hex<<"0x"<<bar[512+105+i]<<" ";
	}
	cout<<endl;

	printf("\nC2H statistics:\n");
	uint32_t count_cmds = bar[512+200];
	uint32_t count_words = bar[512+201];
	uint32_t c2h_cycles = bar[512+202];
	printf("C2H Cycles: %d\n",c2h_cycles);

	printf("count_cmds: %d,total: %d\n",count_cmds,total_cmds);
	printf("count_words: %d,total: %d\n",count_words,total_words);
	
	double c2h_speed = 1.0*c2h_factor*burst_length*total_cmds/(1.0*c2h_cycles*4/1000/1000/1000)/1024/1024/1024;
	
	cout<<endl;
	if(h2c_cycles>c2h_cycles){//c2h is reliable
		printf("H2C Speed: [%.2f] GB/s [X]\n",h2c_speed);
		printf("C2H Speed: [%.2f] GB/s [√]\n",c2h_speed);
	}else{
		printf("H2C Speed: [%.2f] GB/s [√]\n",h2c_speed);
		printf("C2H Speed: [%.2f] GB/s [X]\n",c2h_speed);
	}
	// printCounters();
}
#ifndef dma_hpp
#define dma_hpp

#include <QDMAController.h>
#include <immintrin.h>
#include <iostream>
#include <pthread.h>
using namespace std;

void h2c_benchmark();
void c2h_benchmark();

void h2c_benchmark_random();
void c2h_benchmark_random();
void concurrent_random();

void h2c_benchmark_latency(unsigned char pci_bus);
void c2h_benchmark_latency(unsigned char pci_bus);
void concurrent_latency();
#endif
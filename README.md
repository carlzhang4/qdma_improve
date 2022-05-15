# QDMA Software Guide
Kernel 4.15.0-20-generic has been tested.

## Driver Installation
```
1. prepare 
clone these repo to your home dir or anywhere you like.
$ git clone https://github.com/carlzhang4/qdma_driver.git
$ git clone https://github.com/carlzhang4/lib_qdma.git
$ sudo apt-get install libaio1 libaio-dev

2. compile
$ cd ~/qdma_driver
$ make
$ make apps

3. install apps and header files
$ sudo make install-apps

4. install kernel mod
$ sudo insmod src/qdma-pf.ko
```

## QDMA Lib Installation
```
$ cd ~/lib_qdma
$ mkdir build
$ cd build
$ cmake ..
$ sudo make install 
```

## Software Test
```bash
$ cd ~/qdma_improve
$ mkdir build 
$ cd build
$ cmake ..
$ make 
```
The binary file qdma_throughput is the file that you can test QDMATop, you can write your own software.
- `h2c_benchmark()` will test host to card channel
- `c2h_benchmark()` will test card to host channel
- `benchmark_bridge_write()` will test axi bridge channel

Besides, you can use the binary file qdma_latency to test AXILBenchmarkTop, which aims to benchmark the AXIL read latency in various situations under different workloads.
- `startRead(...)` will initialize the host to card channel with a simple throughput benchmark
- `startWrite(...)` will initialize the card to host channel with a simple throughput benchmark
- `axilReadBenchmark(...)` will test the axi lite read latency

Before you run qdma_throughput or qdma_latency, you must program FPGA and reboot the host.
Each time you reboot you need to redo the insmod step (i.e., sudo insmod src/qdma-pf.ko)

And following instructions needs to be executed.
```bash
$ su root 	# I don't know whether sudo will work.
$ echo 1024 > /sys/bus/pci/devices/0000:1a:00.0/qdma/qmax
$ dma-ctl qdma1a000 q add idx 0 mode st dir bi
$ dma-ctl qdma1a000 q start idx 0 dir bi desc_bypass_en pfetch_bypass_en
$ dma-ctl qdma1a000 q add idx 1 mode st dir bi
$ dma-ctl qdma1a000 q start idx 1 dir bi desc_bypass_en pfetch_bypass_en
$ dma-ctl qdma1a000 q add idx 2 mode st dir bi
$ dma-ctl qdma1a000 q start idx 2 dir bi desc_bypass_en pfetch_bypass_en
$ dma-ctl qdma1a000 q add idx 3 mode st dir bi
$ dma-ctl qdma1a000 q start idx 3 dir bi desc_bypass_en pfetch_bypass_en

$ sudo ./qdma_throughput
$ sudo ./qdma_latency 1>/dev/null # debug information will write to stdout, thus being ignored with the redirection to /dev/null
```
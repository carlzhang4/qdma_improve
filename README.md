# QDMA Software Guide
Ubuntu 18.04 LTS (Kernel 4.15.0-20-generic) has been tested.

## Driver Installation
```
1. prepare 
clone these repo to your home dir or anywhere you like.
$ git clone git@github.com:carlzhang4/qdma_driver.git
$ git clone git@github.com:carlzhang4/lib_qdma.git
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
There are three binary files you can use:

### `qdma_throughput`
Coressponed to `QDMATop.scala`.
- `h2c_benchmark()` will test host to card channel
- `c2h_benchmark()` will test card to host channel
- `benchmark_bridge_write()` will test axi bridge channel

### `qdma_latency`
Coressponed to `AXILBenchmarkTop.scala`, which aims to benchmark the AXIL read latency in various situations under different workloads.
- `startFpgaH2C(...)` will initialize the host to card channel with a simple throughput benchmark
- `startFpgaC2H(...)` will initialize the card to host channel with a simple throughput benchmark
- `axilReadBenchmark(...)` will test the axi lite read latency

### `qdma_random`
Coressponed to `QDMARandomTop.scala`, which aims to benchmark random access performance. (1 GB memory)
- `h2c_benchmark_random()` will test host to card channel
- `c2h_benchmark_random()` will test card to host channel
- `concurrent_random()` will test concurrent performance, if you want to get one direction performance (such as h2c), you need to set another's factor (c2h_factor) to 2 ensuring that c2h is always running when h2c is performed. 

### `qdma_dma_latency`
Coressponed to `QDMALatencyTop.scala`, which aims to benchmark dma channel's host to card and card to host latency. (1 GB memory)
- `h2c_benchmark_latency()` will test host to card channel
- `c2h_benchmark_latency()` will test card to host channel
- `concurrent_latency()` will test concurrent performance, this is not fully implemented and latency would increase around 1us when fully loaded.

Before you run these binaries, you must program FPGA and reboot the host.
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
$ sudo ./qdma_random
$ sudo ./qdma_latency 1>/dev/null # debug information will write to stdout, thus being ignored with the redirection to /dev/null
```
There are some useful commands  (provided by Xilinx QDMA Linux Kernel Driver) in cmd.txt.

## QDMA C2H bug
1. When running with more than 8 qs, it will always fail. QDMA C2H data port's ready would be down after receiving several data. 

2. Even running with less than or equal to 8 qs, it can sometimes fail, try reprogram the FPGA. I guess only one q has the most chance to pass.

3. Tested situation:

	packet size: 1K/32K(which would be splited into multiple packets)

4. I have tried fetching tag index for each queue using dma-ctl, it's useless.

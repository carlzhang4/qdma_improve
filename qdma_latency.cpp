#include <QDMAController.h>
#include "load.hpp"
#include <unistd.h>
#include <cstdio>
int main(void)
{
    using namespace std;
    init(0x1a);
    void* qdmaBuffer = qdma_alloc(1024L * 1024 * 1024);
    uint32_t* readBuffer = (uint32_t*)(qdmaBuffer);
    volatile uint32_t* writeBuffer = (volatile uint32_t*)(readBuffer + 128L * 1024 * 1024);
    volatile uint32_t* axilReg = (volatile uint32_t*)(getLiteAddr());
    
    if (!startRead(readBuffer, axilReg) || !startWrite(writeBuffer, axilReg)) {
        printCounters();
        resetCounters();
        cerr << "Aborted!" << endl; 
        sleep(1);
        return 1;
    }
    printCounters();
    resetCounters();
    sleep(1);
    
    // AXIL Benchmark
    cerr << "Axil Benchmark Begin!" << endl;

    uint32_t readTimeBegin, readTimeEnd, writeTimeBegin, writeTimeEnd, readTimeCount, writeTimeCount;
    uint32_t readCommands, writeCommands;
    double readBandwidth, writeBandwidth;
    // read delay with no load
    readTimeBegin = axilReg[H2C_TIME_COUNT];
    writeTimeBegin = axilReg[C2H_TIME_COUNT];
    auto readDelayWithNoLoad = axilReadBenchmark(axilReg);
    readTimeEnd = axilReg[H2C_TIME_COUNT];
    writeTimeEnd = axilReg[C2H_TIME_COUNT];
    readCommands = axilReg[H2C_COMMANDS_CHECK];
    writeCommands = axilReg[C2H_COMMANDS_CHECK];
    readTimeCount = readTimeEnd - readTimeBegin;
    writeTimeCount = writeTimeEnd - writeTimeBegin;
    readBandwidth = readCommands == 0 ? 0 : 1.0*DEFAULT_DATA_BYTE_LENGTH*readCommands/(1.0*readTimeCount*4/1000/1000/1000)/1024/1024/1024;
    writeBandwidth = writeCommands == 0 ? 0 : 1.0*DEFAULT_DATA_BYTE_LENGTH*writeCommands/(1.0*writeTimeCount*4/1000/1000/1000)/1024/1024/1024;
    fprintf(stderr, "Average Read Delay With No Load: %.2lfns ==> [QDMA]: Read: %.2lfGBps  Write: %.2lfGBps\n", readDelayWithNoLoad / 512.0, readBandwidth, writeBandwidth);
    printCounters();
    resetCounters();
    sleep(1);
    

    // read with qdma read load
    readTimeBegin = axilReg[H2C_TIME_COUNT];
    writeTimeBegin = axilReg[C2H_TIME_COUNT];
    resumeRead(axilReg);
    auto readDelayWithReadLoad = axilReadBenchmark(axilReg);
    pauseRead(axilReg);
    sleep(2);
    readTimeEnd = axilReg[H2C_TIME_COUNT];
    writeTimeEnd = axilReg[C2H_TIME_COUNT];
    readCommands = axilReg[H2C_COMMANDS_CHECK];
    writeCommands = axilReg[C2H_COMMANDS_CHECK];
    readTimeCount = readTimeEnd - readTimeBegin;
    writeTimeCount = writeTimeEnd - writeTimeBegin;
    readBandwidth = readCommands == 0 ? 0 : 1.0*DEFAULT_DATA_BYTE_LENGTH*readCommands/(1.0*readTimeCount*4/1000/1000/1000)/1024/1024/1024;
    writeBandwidth = writeCommands == 0 ? 0 : 1.0*DEFAULT_DATA_BYTE_LENGTH*writeCommands/(1.0*writeTimeCount*4/1000/1000/1000)/1024/1024/1024;
    fprintf(stderr, "Average Read Delay With Read Load: %.2lfns ==> [QDMA]: Read: %.2lfGBps  Write: %.2lfGBps\n", readDelayWithReadLoad / 512.0, readBandwidth, writeBandwidth);
    printCounters();
    resetCounters();
    sleep(1);



    
    // read with qdma write load
    readTimeBegin = axilReg[H2C_TIME_COUNT];
    writeTimeBegin = axilReg[C2H_TIME_COUNT];
    resumeWrite(axilReg);
    auto readDelayWithWriteLoad = axilReadBenchmark(axilReg);
    pauseWrite(axilReg);
    sleep(2);
    readTimeEnd = axilReg[H2C_TIME_COUNT];
    writeTimeEnd = axilReg[C2H_TIME_COUNT];
    readCommands = axilReg[H2C_COMMANDS_CHECK];
    writeCommands = axilReg[C2H_COMMANDS_CHECK];
    readTimeCount = readTimeEnd - readTimeBegin;
    writeTimeCount = writeTimeEnd - writeTimeBegin;
    readBandwidth = readCommands == 0 ? 0 : 1.0*DEFAULT_DATA_BYTE_LENGTH*readCommands/(1.0*readTimeCount*4/1000/1000/1000)/1024/1024/1024;
    writeBandwidth = writeCommands == 0 ? 0 : 1.0*DEFAULT_DATA_BYTE_LENGTH*writeCommands/(1.0*writeTimeCount*4/1000/1000/1000)/1024/1024/1024;
    fprintf(stderr, "Average Read Delay With Write Load: %.2lfns ==> [QDMA]: Read: %.2lfGBps  Write: %.2lfGBps\n", readDelayWithWriteLoad / 512.0, readBandwidth, writeBandwidth);
    printCounters();
    resetCounters();
    sleep(1);

    

    // read with qdma read & write load
    readTimeBegin = axilReg[H2C_TIME_COUNT];
    writeTimeBegin = axilReg[C2H_TIME_COUNT];
    resumeRead(axilReg);
    resumeWrite(axilReg);
    auto readDelayWithReadAndWriteLoad = axilReadBenchmark(axilReg);
    pauseWrite(axilReg);
    pauseRead(axilReg);
    sleep(2);
    readTimeEnd = axilReg[H2C_TIME_COUNT];
    writeTimeEnd = axilReg[C2H_TIME_COUNT];
    readCommands = axilReg[H2C_COMMANDS_CHECK];
    writeCommands = axilReg[C2H_COMMANDS_CHECK];
    readTimeCount = readTimeEnd - readTimeBegin;
    writeTimeCount = writeTimeEnd - writeTimeBegin;
    readBandwidth = readCommands == 0 ? 0 : 1.0*DEFAULT_DATA_BYTE_LENGTH*readCommands/(1.0*readTimeCount*4/1000/1000/1000)/1024/1024/1024;
    writeBandwidth = writeCommands == 0 ? 0 : 1.0*DEFAULT_DATA_BYTE_LENGTH*writeCommands/(1.0*writeTimeCount*4/1000/1000/1000)/1024/1024/1024;
    fprintf(stderr, "Average Read Delay With Read And Write Load: %.2lfns ==> [QDMA]: Read: %.2lfGBps  Write: %.2lfGBps\n", readDelayWithReadAndWriteLoad / 512.0, readBandwidth, writeBandwidth);
    printCounters();
    resetCounters();
    sleep(1);

    cerr << "Axil Benchmark Complete!" << endl;
}
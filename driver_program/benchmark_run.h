#ifndef BENCHMARKHEADER_H
#define BENCHMARKHEADER_H

#define STACK_TRACE 1
#define MULTI 1

#define PIN_PATH "../pin-3.20-98437-gf02b61307-gcc-linux/pin"

#ifdef STACK_TRACE
#define PIN_TOOL "../pin-3.20-98437-gf02b61307-gcc-linux/source/tools/MyPinTool/obj-intel64/multireadwrite.so"
#else
#define PIN_TOOL "../pin-3.20-98437-gf02b61307-gcc-linux/source/tools/MyPinTool/obj-intel64/vanillareadwrite.so"
#endif


#define BENCH_PATH "../example/pthread_merge_sort"
#define BENCH_NAME "pthread_merge_sort"
#define BENCH_ARGS ""

#endif

        _____       _ _____  
       / ____|     (_)  __ \
      | (___  _ __  _| |__) |
       \___ \| '_ \| |  ___/
       ____) | | | | | |     
      |_____/|_| |_|_|_|     
SniP: An Efficient Stack Tracing Framework for Multi-threaded Programs
=======================================================================
    SniP is a stack tracing framework for multi-threaded programs built around Intel's binary instrumentation tool Pin.

## What package/platform dependencies do I need to have to run the code?

  This code is tested on a Ubuntu 20.04 machine with Linux kernel version 4.19 and g++ version 9.3.

  Following system settings need a modification whenever a system seeks reboot/restart.

  1. Disable systems ASLR ==> Use command mentioned below
      > **echo 0 | sudo tee /proc/sys/kernel/randomize_va_space**

  2. Enable tracing of one process from another one (Pin requirement when OS configuration prevents injection mode). ==> Use command mentioned
      > **echo 0 | sudo tee /proc/sys/kernel/yama/ptrace_scope**

## How do I use it?

  Following steps shows the usage of SniP

  1. Insert monitoring kernel module available in monitor_module. ==> ==> Use command mentioned
      > **sudo insmode monitor_module/stackmonitor.ko

  2. Update the required paths and settings in benchmark_run.h file available in driver_program folder. The description of all the parameter/settings presents in benchmark.h file as #define are as follows:

      2.1 **STACK_TRACE:** Set to 1 if you want to trace only the stack area otherwise 0

      2.2 **MULTI:** Set to 1 if your benchmark or program which you want to trace is multi-threded else 0.

      2.3 **PIN_PATH:** Path to the pin binary file. For your reference, we have placed the tarball Pin on the root of this SniP. Please untar it on the same location or at your convineint location. But do not forgot to modify the this PATH.

      2.4 **PIN_TOOL:** This holds the path of your Pin tool .so file which you have created. We are also providing a Pin tool you your are using our provided pin tarball. On ontaring it, you can find pin tool at location **'pin-3.20-98437-gf02b61307-gcc-linux/source/tools/MyPinTool'**. You required to generate .so binary by invoking **make** command.

      2.5 **BENCH_PATH:** Path to your benchmark or program you want to trace. We are providing and example benchmark program inside is example folder of SniP project. To use it, jume generate its binary file by executing following command after going inside the example folder.
          > **gcc -o pthread_merge_sort pthread_merge_sort.c -lpthread**

      2.6 **BENCH_NAME:** Provide the name of benchmark. In our example benchmark case, set it to **pthread_merge_sort**.

      2.7 **BENCH_ARGS:** If your benchmark program required some command line arhuments, ples add them in the form of space separated single string. If bechmark program does not need any argument. Set is bith empty string, which is the case with our supplied example bechmark program.
      

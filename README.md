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

  1. Insert monitoring kernel module available in monitor_module.==> Use command mentioned,

      Note: If you are facing any difficulty to insert **stackmonitor.ko**, please **rm stackmonitor.ko** and **run make** inside **monitor_module** folder and try to insert
      > **sudo insmode monitor_module/stackmonitor.ko

  2. Update the required paths and settings in **benchmark_run.h** file available in **driver_program** folder. The description of all the parameter/settings presents in benchmark_run.h file as #define are as follows:

      2.1 **STACK_TRACE:** Set to 1 if you want to perform targetted trace of stack area, otherwise set to 0

      2.2 **MULTI:** Set to 1 if your benchmark or program which you want to trace is multi-threaded, otherwise set to 0.

      2.3 **PIN_PATH:** Path to the pin binary file. For your reference, we have placed the tarball of Intel's Pin in the SniP root directory. Please untar it on the same location or at your convenient location. But do not forgot to modify the **PIN_PATH** accordingly.

      2.4 **PIN_TOOL:** This holds the path of your Pin tool **.so** file which you have created. We are also providing the pin tool (**multireadwrite.cpp**) that we have used in pin tarball. On untaring it, you can find pin tool at location **'pin-3.20-98437-gf02b61307-gcc-linux/source/tools/MyPinTool'**. You required to generate **.so** binary by invoking **make** command.

      2.5 **BENCH_PATH:** Path to your benchmark or program that you want to trace. We are providing an example benchmark program inside **example** folder in the SniP root directory. To use it, just generate its binary file by executing following command after going inside the example folder.
      > **cd example**

      > **gcc -o pthread_merge_sort pthread_merge_sort.c -lpthread**

      2.6 **BENCH_NAME:** Provide the name of benchmark. In our example benchmark case, set it to **pthread_merge_sort**.

      2.7 **BENCH_ARGS:** If your benchmark program requires some command line arguments, please add them in the form of space separated single string. If bechmark program does not need any argument, you may set it with empty string. **pthread_merge_sort** does not take any arguments.
      
      You may refer to man page of **execlp** in Linux to gain more information on **BENCH PATH, NAME, ARGS** arguments.

  3. Generate the driver program binary. To generate it, first go inside the **driver_program** folder, use make command as follows:
      > **cd driver_program**

      > **make**

  4. Execute the benchmar_run binary to get the trace of your benchmark program as follows from the inside of driver_program folder:
      > **./benchmark_run**

  On Successfull execution of driver program, you will get trace file **pinatrace.out** as provided inside our Pin tool. This file name may differe on the basis of your supplied output file name in the Pin tool. You can also use **dmesg** to see the stack ranges indentified by the monitor module.

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

  This code is tested on a Linux machine with Linux kernel version 4.19.

  Following system settings need a modification whenever a system seeks reboot/restart.

  1. Disable systems ASLR ==> Use command mentioned below in Ubuntu machine
  > **echo 0 | sudo tee /proc/sys/kernel/randomize_va_space**

  2. Enable tracing of one process from another one (Pin requirement when OS configuration prevents injection mode). ==> Use command mentioned below in Ubuntu machine
  > **echo 0 | sudo tee /proc/sys/kernel/yama/ptrace_scope**

## How do I use it?

#ifndef BENCHMARKHEADER_H
#define BENCHMARKHEADER_H

#define STACK_TRACE 1
//#define MULTI 1

#define PIN_PATH "/home/kparun/Documents/stack_analysis/pin-3.20-98437-gf02b61307-gcc-linux/pin"

#ifdef STACK_TRACE
#define PIN_TOOL "/home/kparun/Documents/stack_analysis/pin-3.20-98437-gf02b61307-gcc-linux/source/tools/MyPinTool/obj-intel64/multireadwrite.so"
#else
#define PIN_TOOL "/home/kparun/Documents/stack_analysis/pin-3.20-98437-gf02b61307-gcc-linux/source/tools/MyPinTool/obj-intel64/vanillareadwrite.so"
#endif

//#define BENCH_PATH "/usr/bin/mpiexec"
//#define BENCH_NAME "mpiexec"
//#define BENCH_ARGS "/home/kparun/Documents/stack_analysis/graph500-newreference/src/graph500_reference_bfs 16"

//#define BENCH_PATH "/usr/sbin/mysqld"
//#define BENCH_NAME "mysqld"
//#define BENCH_ARGS "--basedir=/usr --datadir=/var/lib/mysql --plugin-dir=/usr/lib/mysql/plugin --user=mysql --log-error=/var/log/mysql/error.log --pid-file=kparun-PC.pid"

//#define BENCH_PATH "/home/kparun/Documents/stack_analysis/graph500-newreference/src/graph500_reference_bfs"
//#define BENCH_NAME "graph500_reference_bfs"
//#define BENCH_ARGS "10"


//#define BENCH_PATH "/usr/bin/memcached"
//#define BENCH_NAME "memcached"
//#define BENCH_ARGS "-m 64 -p 11211 -u memcache -l 127.0.0.1 -P /var/run/memcached/memcached.pid"

//#define BENCH_PATH "/home/kparun/Documents/stack_analysis/micro-benchmarks/vulnerability1"
//#define BENCH_NAME "vulnerability1"
//#define BENCH_ARGS ""

//#define BENCH_PATH "/home/kparun/Documents/tkrzw-0.9.1/example/babydbm_ex1"
//#define BENCH_NAME "babydbm_ex1"
//#define BENCH_ARGS ""

//#define BENCH_PATH "/home/kparun/Documents/tkrzw-0.9.1/example/cachedbm_ex1"
//#define BENCH_NAME "cachedbm_ex1"
//#define BENCH_ARGS ""

#define BENCH_PATH "/home/kparun/Documents/tkrzw-0.9.1/example/tinydbm_ex1"
#define BENCH_NAME "tinydbm_ex1"
#define BENCH_ARGS ""

//#define BENCH_PATH "/usr/bin/python3.8"
//#define BENCH_NAME "python3.8"
//#define BENCH_ARG1 "-m"
//#define BENCH_ARG2 "http.server"

//#define BENCH_PATH "/home/kparun/Documents/myvenv/bin/python3"
//#define BENCH_NAME "python3"
//#define BENCH_ARGS "ML_Scripts/dt.py"
//#define BENCH_ARGS "ML_Scripts/et.py"
//#define BENCH_ARGS "ML_Scripts/GB.py"
//#define BENCH_ARGS "ML_Scripts/NB.py"
#endif

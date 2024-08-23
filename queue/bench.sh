#!/bin/sh
./build/benchmark_queue_without_memory --benchmark_out=queue_without_memory --benchmark_out_format=csv
./build/benchmark_queue_without_aling --benchmark_out=queue_without_aling --benchmark_out_format=csv
./build/benchmark_queue_no_opt --benchmark_out=queue_no_opt --benchmark_out_format=csv
./build/benchmark_queue --benchmark_out=queue --benchmark_out_format=csv
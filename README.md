## Intro
This is a simple benchmarker, that can be used as a way to compare the speed of different code.
It is planned on to add a higher precision benchmarker class to the existing set.

## Build

`mkdir build && cd build && cmake ..`

To make the demo binary: 

`make mini-bench_demo`

To make the **shared** library:

`make mini-bench`

To install the lib: 

`sudo make install mini-bench` 
## Usage

### demo 

Use `mini-bench_demo <bench-type> <complexity>` to see the demo. For example, 
`mini-bench_demo scoped 10000` to see the scoped benchmark results on operation with complexity 10000 (inserts to a vector with reserve vs without)

See `mini-bench_demo --help` for more info on demo usage.


## TODO
- add a higher precision benchmarker (less overhead)

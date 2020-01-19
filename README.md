# afl-libprotobuf-mutator

Example/skeleton for using
[libprotobuf-mutator](https://github.com/google/libprotobuf-mutator)
together with AFL.

## Usage

0. Download and compile [AFLplusplus](https://github.com/vanhauser-thc/AFLplusplus)
1. Put your protobuffer in `gen/out.proto`
2. Write your own protobuffer-message-to-raw-data methods
3. `export AFL_CUSTOM_MUTATOR_ONLY=1`
4. `export AFL_CUSTOM_MUTATOR_LIBRARY=./path/to/library.so`

The current implementation turns enum values into bytes.
It was an experiment in encoding regexps as protobuffers.
Unfortunately, PBs are not powerful enough to do that.

## Install

```sh
./build.sh
make
```

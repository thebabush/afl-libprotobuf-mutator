# afl-libprotobuf-mutator

Example/skeleton for using
[libprotobuf-mutator](https://github.com/google/libprotobuf-mutator)
together with AFL.

## Usage

0. Download and compile [AFLplusplus](https://github.com/vanhauser-thc/AFLplusplus)
1. Put your protobuffer in `gen/out.proto`
2. Write your own protobuffer-message-to-raw-data methods
3. `export AFL_CUSTOM_MUTATOR_ONLY=1`
4. `export AFL_CUSTOM_MUTATOR_LIBRARY=./mutator.so`

The current implementation turns enum values into bytes.
It was an experiment in encoding regexps as protobuffers.
Unfortunately, PBs are not powerful enough to do that.

### Full Example

```bash
export AFL_CUSTOM_MUTATOR_ONLY=1
export AFL_CUSTOM_MUTATOR_LIBRARY=./mutator.so
afl-fuzz -i /tmp/in -o /tmp/out -Q -- ./dumper @@
```

In order to dump/verify the content of the protobuffers:

```bash
for f in /tmp/out/queue/id*src*; do echo "== $f =="; ./dumper $f; done
```

## Install

```sh
./build.sh
make
```

## Missing Features

- AFLplusplus doesn't yet provide a custom splicing hook, so we can't mix two
  protobuffers
  - I have a custom version on my PC but I'm not sure it's bug-free so I won't
    push it for the time being
- honggfuzz has support for external mutators/postprocessors, so it should be
  trivial to add support (maybe it'll be a little bit slower do to I/O)


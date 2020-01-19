# Did I say that I can't write makefiles?

PROTOC=./external/bin/protoc
CXXFLAGS=-O3 -fPIC -std=c++11 -I. -I./external/include -I./external/include/libprotobuf-mutator
CXX=clang++
CC=clang
LD=$(CXX)

.PHONY: clean

all: mutator libmutator.so dumper

libmutator.so: gen/out.pb.o src/mutator.o
	$(CXX) -pthread -shared $(CXXFLAGS) -o $@ $^ ./external/lib/libprotobuf-mutator-libfuzzer.a ./external/lib/libprotobuf-mutator.a ./build/external.protobuf/lib/libprotobuf.a

mutator: gen/out.pb.o src/mutator.o
	$(LD) -pthread $(CXXFLAGS) -fsanitize=fuzzer -o $@ $^ ./external/lib/libprotobuf-mutator-libfuzzer.a ./external/lib/libprotobuf-mutator.a ./build/external.protobuf/lib/libprotobuf.a

dumper: gen/out.pb.o src/dumper.o src/mutator.o
	$(LD) -pthread $(CXXFLAGS) -o $@ $^ ./external/lib/libprotobuf-mutator-libfuzzer.a ./external/lib/libprotobuf-mutator.a ./build/external.protobuf/lib/libprotobuf.a

gen/out.pb.cc gen/out.pb.h: gen/out.proto
	$(PROTOC) --cpp_out=. $<

gen/out.pb.o: gen/out.pb.cc
	$(CXX) $(CXXFLAGS) -c -o $@ $<

%.o: %.cc
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	rm src/mutator.o gen/*.pb.*

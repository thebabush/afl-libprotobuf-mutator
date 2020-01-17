
#include <iostream>
#include <fstream>
#include <sstream>
#include <unistd.h>

#include "libprotobuf-mutator/src/libfuzzer/libfuzzer_macro.h"

#include "gen/out.pb.h"

#include "mutator.h"

inline std::string slurp(const std::string& path) {
  std::ostringstream buf; 
  std::ifstream input (path.c_str()); 
  buf << input.rdbuf(); 
  return buf.str();
}

int main(int argc, char *argv[]) {
  Root msg;

  std::string data = slurp(argv[1]);

  protobuf_mutator::libfuzzer::LoadProtoInput(true, (const uint8_t *)data.c_str(), data.size(), &msg);
  //std::cout << msg.DebugString() << std::endl;

  std::stringstream stream;
  ProtoToDataHelper(stream, msg);
  std::cout << stream.str() << std::endl;

  return 0;
}

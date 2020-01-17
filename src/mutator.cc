#include "mutator.h"

#include <iostream>
#include <unistd.h>

#include "google/protobuf/descriptor.h"
#include "google/protobuf/descriptor.pb.h"
#include "google/protobuf/reflection.h"

#include "libprotobuf-mutator/src/libfuzzer/libfuzzer_macro.h"

#include "gen/out.pb.h"

#include <algorithm>


void ProtoToDataHelper(std::stringstream &out, const google::protobuf::Message &msg) {
  const google::protobuf::Descriptor *desc = msg.GetDescriptor();
  const google::protobuf::Reflection *refl = msg.GetReflection();

  const unsigned fields = desc->field_count();
  for (unsigned i = 0; i < fields; ++i) {
    const google::protobuf::FieldDescriptor *field = desc->field(i);

    if (field->cpp_type() == google::protobuf::FieldDescriptor::CPPTYPE_MESSAGE) {
      if (field->is_repeated()) {
        const google::protobuf::RepeatedFieldRef<google::protobuf::Message> &ptr = refl->GetRepeatedFieldRef<google::protobuf::Message>(msg, field);
        for (const auto &child : ptr) {
          ProtoToDataHelper(out, child);
        }
      } else if (refl->HasField(msg, field)) {
        const google::protobuf::Message &child = refl->GetMessage(msg, field);
        ProtoToDataHelper(out, child);
      }
    } else if (field->cpp_type() ==
              google::protobuf::FieldDescriptor::CPPTYPE_ENUM) {
      out << (char)refl->GetEnumValue(msg, field);
    } else {
      abort();
    }

  }
}

// Apparently the textual generator kinda breaks?
DEFINE_BINARY_PROTO_FUZZER(const Root &root) {
  std::stringstream stream;
  ProtoToDataHelper(stream, root);
  std::string data = stream.str();
  puts("====================================================================================================");
  // std::string dbg = root.DebugString();
  // std::replace(dbg.begin(), dbg.end(), '\n', ' ');
}

// AFLPlusPlus interface
extern "C" {
  size_t afl_custom_mutator(uint8_t *data, size_t size, uint8_t *mutated_out, size_t max_size, unsigned int seed) {
    // This will reuse the data buffer...
    // AFLpp currently performs a memcopy from mutated_out
    // Unfortunately it's not **mutated_out so we end up doing two memcpy
    // should be fixed upstream after I finish this
    assert(size <= max_size);
    memcpy(mutated_out, data, size);

    size_t new_size = LLVMFuzzerCustomMutator(
      mutated_out,
      size,
      max_size,
      seed
    );
    return new_size;
  }

  size_t pre_save_handler(uint8_t *data, size_t size, uint8_t **new_data) {
    // new_data is never free'd by pre_save_handler
    // I prefer a slow but clearer implementation for now
    static uint8_t *saved_buf = NULL;

    Root msg;
    std::stringstream stream;
    ProtoToDataHelper(stream, msg);
    const std::string str = stream.str();

    uint8_t *new_buf = (uint8_t *) realloc((void *)saved_buf, str.size());
    if (!new_buf) {
      *new_data = data;
      return size;
    }
    saved_buf = new_buf;

    memcpy((void *)new_buf, str.c_str(), str.size());

    return str.size();
  }
}

#ifndef MUTATOR_H
#define MUTATOR_H

#include <sstream>

#include "google/protobuf/message.h"

void ProtoToDataHelper(std::stringstream &out, const google::protobuf::Message &msg);

#endif
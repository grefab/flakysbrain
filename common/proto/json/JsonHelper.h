//
// Created by grefab on 9/22/20.
//

#pragma once

#include <google/protobuf/message.h>
#include <string>

std::string protoToJsonString(google::protobuf::Message const& message, bool compact = true);

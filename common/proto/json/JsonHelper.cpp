//
// Created by grefab on 9/22/20.
//

#include "common/proto/json/JsonHelper.h"
#include <fmt/core.h>
#include <google/protobuf/util/json_util.h>
#include <spdlog/spdlog.h>

std::string protoToJsonString(google::protobuf::Message const& message, bool compact) {
    google::protobuf::util::JsonOptions options;
    options.add_whitespace = !compact;
    options.always_print_primitive_fields = true;
    std::string output;
    auto status = google::protobuf::util::MessageToJsonString(message, &output, options);
    if (!status.ok()) {
        spdlog::error("serializing proto to json failed: {}", status.ToString());
    }
    return output;
}

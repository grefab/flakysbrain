//
// Created by grefab on 9/27/20.
//

#include "common/proto/time/FromProto.h"
#include <google/protobuf/util/time_util.h>

std::chrono::system_clock::time_point asTimePoint(::google::protobuf::Timestamp const& ts) {
    auto nanoseconds = google::protobuf::util::TimeUtil::TimestampToNanoseconds(ts);
#ifdef __linux__
    return std::chrono::system_clock::time_point(std::chrono::nanoseconds(nanoseconds));
#else
    return std::chrono::system_clock::time_point(std::chrono::microseconds(nanoseconds / 1000));
#endif
}

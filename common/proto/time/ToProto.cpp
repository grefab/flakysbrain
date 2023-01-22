//
// Created by grefab on 9/27/20.
//

#include "common/proto/time/ToProto.h"

::google::protobuf::Timestamp asProtoTimestamp(std::chrono::system_clock::time_point tp) {
    ::google::protobuf::Timestamp time_stamp;
    long long nanos = std::chrono::duration_cast<std::chrono::nanoseconds>(tp.time_since_epoch()).count();
    time_stamp.set_seconds(nanos / 1000000000);
    time_stamp.set_nanos(nanos % 1000000000);
    return time_stamp;
}

::google::protobuf::Timestamp asProtoTimestamp(long long nanoseconds_since_epoch) {
    ::google::protobuf::Timestamp time_stamp;
    time_stamp.set_seconds(nanoseconds_since_epoch / 1000000000);
    time_stamp.set_nanos(nanoseconds_since_epoch % 1000000000);
    return time_stamp;
}

::google::protobuf::Duration asProtoDuration(std::chrono::nanoseconds value) {
    ::google::protobuf::Duration duration;
    duration.set_seconds(value.count() / 1000000000);
    duration.set_nanos(value.count() % 1000000000);
    return duration;
}

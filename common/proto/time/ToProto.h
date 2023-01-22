//
// Created by grefab on 9/27/20.
//

#pragma once

#include <google/protobuf/duration.pb.h>
#include <google/protobuf/timestamp.pb.h>

// https://github.com/istio/old_mixerclient_repo/blob/master/utils/protobuf.h

// Convert system_clock time to protobuf timestamp
::google::protobuf::Timestamp asProtoTimestamp(std::chrono::system_clock::time_point tp);

// Convert time as ns since epoch to protobuf timestamp
::google::protobuf::Timestamp asProtoTimestamp(long long nanoseconds_since_epoch);

// Convert from chrono duration to protobuf duration
::google::protobuf::Duration asProtoDuration(std::chrono::nanoseconds value);

//
// Created by grefab on 9/27/20.
//

#pragma once

#include <google/protobuf/duration.pb.h>
#include <google/protobuf/timestamp.pb.h>

// https://github.com/istio/old_mixerclient_repo/blob/master/utils/protobuf.h

// Convert protobuf timestamp to system_clock time
std::chrono::system_clock::time_point asTimePoint(::google::protobuf::Timestamp const& ts);

// Convert from protobuf duration to chrono duration
std::chrono::milliseconds asDurationMs(const ::google::protobuf::Duration& duration);

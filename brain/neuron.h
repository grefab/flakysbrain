#pragma once

#include <atomic>
#include <functional>
#include <limits>
#include <memory>
#include <unordered_set>
#include <utility>
#include "brain/types.h"
#include "common/geometry/PointF.h"

class brain;
struct connection;
using connection_ptr = std::shared_ptr<connection>;

static std::atomic_uint32_t next_id = 0;

struct neuron {
  friend class brain;
  friend class brain_mass;

  std::atomic_uint32_t const id;

  // Constructor
  neuron(pulse power, double bias) : id(next_id++), power(power), bias(bias) {}

  // Is called by brain when an event reaches this neuron.
  void apply_pulse(pulse p, timestamp now, brain* b);

  /* The following parameters shall be modified during learning. */
  // Amount of firing power in [0, 1]
  pulse power;
  // If potential exceeds bias, we fire. In (0, inf)
  double bias;
  // Where and when to fire
  std::unordered_set<connection_ptr> connections;

  // Monitoring
  std::function<void(timestamp now, pulse power)> on_fire_ = [](timestamp,
                                                                pulse) {};

  // Display stuff
  PointF pos = PointF(0, 0);

  private:
  // Fires a pulse along the connections.
  void fire(timestamp now, brain* b);

  // Modifies potential depending on time passed since last pulse receive event.
  // Potential shall decay over time until it reaches 0.
  [[nodiscard]] static double decayed(duration time_passed,
                                      double last_potential);

  // Modifies pulse depending on time passed since last time fired.
  // Pulse shall be weakened if last event has happened recently.
  [[nodiscard]] static double cooled_down(duration time_passed, pulse p);

  private:
  // Internal state
  double potential_ = 0;
  timestamp last_pulse_received_timestamp_ =
    std::numeric_limits<timestamp>::max();
  timestamp last_fired_timestamp_ = std::numeric_limits<timestamp>::max();
};

using neuron_ptr = std::shared_ptr<neuron>;

struct connection {
  // Constructor
  connection(neuron_ptr target, duration distance, double weight)
      : target(std::move(target)), distance(distance), weight(weight) {}

  // Where this event will be applied to.
  neuron_ptr target;

  // The timely distance to the target, shall be positive.
  duration distance;

  // Is multiplied to an outgoing pulse, shall be in [-1, 1].
  double weight;
};

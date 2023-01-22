//
// Created by Gregor Fabritius on 22.01.23.
//

#include "brain_mass.h"
#include "common/proto/geometry/Geometry.h"

neuron_ptr brain_mass::add_neuron(neuron_ptr n) {
  neurons_.insert(n);
  return n;
}

void brain_mass::remove_neuron(neuron_ptr n) {
  // Remove all connections from this neuron, so that if it is still part of
  // the event queue, it does not fire anywhere upon event execution.
  n->connections.clear();

  neurons_.erase(n);

  // Remove all connections to the to-be-deleted neuron.
  for (auto& neuron : neurons_) {
    for (auto c_it = neuron->connections.begin();
         c_it != neuron->connections.end();) {
      if ((*c_it)->target == n) {
        neuron->connections.erase(c_it++);
      } else {
        ++c_it;
      }
    }
  }
}

void brain_mass::add_connection(neuron_ptr n, connection_ptr c) {
  n->connections.insert(c);
}

void brain_mass::remove_connection(neuron_ptr n, connection_ptr c) {
  n->connections.erase(c);
}

brain_api::Snapshot brain_mass::makeSnapshot() const {
  brain_api::Snapshot s;

  for (auto const& neuron : neurons()) {
    brain_api::Neuron n;
    n.set_id(neuron->id);
    n.set_power(neuron->power);
    n.set_bias(neuron->bias);
    n.set_potential(neuron->potential_);
    *n.mutable_pos() = asPoint(neuron->pos);
    for (auto const& connection : neuron->connections) {
      brain_api::Connection c;
      c.set_to_id(connection->target->id);
      c.set_weight(connection->weight);
      *n.add_connections() = c;
    }
    *s.add_neurons() = n;
  }

  return s;
}

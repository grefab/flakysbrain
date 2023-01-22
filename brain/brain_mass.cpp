//
// Created by Gregor Fabritius on 22.01.23.
//

#include "brain_mass.h"

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
      if ((*c_it)->target_ == n) {
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

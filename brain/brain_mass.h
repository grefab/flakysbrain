//
// Created by Gregor Fabritius on 22.01.23.
//

#pragma once

#include <unordered_set>
#include "brain/api/proto/api.pb.h"
#include "brain/neuron.h"

class brain_mass {
  friend class brain;

  public:
  // Fiddle with neurons.
  neuron_ptr add_neuron(neuron_ptr n);
  void remove_neuron(neuron_ptr n);

  void add_connection(neuron_ptr n, connection_ptr c);
  void remove_connection(neuron_ptr n, connection_ptr c);

  std::unordered_set<neuron_ptr> const& neurons() const { return neurons_; }

  brain_api::Snapshot makeSnapshot() const;

  private:
  // Contains all neurons in this brain. Mainly used for bookkeeping.
  std::unordered_set<neuron_ptr> neurons_;
};

using brain_mass_ptr = std::shared_ptr<brain_mass>;

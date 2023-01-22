//
// Created by gregor on 20.06.16.
//

#include "brain/brain.h"
#include "3rdparty/catch/catch.hpp"
#include "brain/brain_mass.h"

TEST_CASE("brain modifications", "[brain]") {
  auto b = std::make_shared<brain_mass>();

  SECTION("neurons can be added") {
    auto n1 = b->add_neuron(std::make_shared<neuron>(1, 0.5));
    REQUIRE(b->neurons().size() == 1);

    SECTION("new neurons have no connections") {
      REQUIRE(n1->connections_.size() == 0);
    }

    SECTION("neurons can be deleted") {
      b->remove_neuron(n1);
      REQUIRE(b->neurons().size() == 0);
    }

    SECTION("connections can be added") {
      auto n2 = b->add_neuron(std::make_shared<neuron>(1, 0.5));
      auto c = std::make_shared<connection>(n2, 1, 1);
      b->add_connection(n1, c);
      REQUIRE(n1->connections_.size() == 1);

      SECTION("connections can be deleted") {
        b->remove_connection(n1, c);
        REQUIRE(n1->connections_.size() == 0);
      }

      SECTION("connections are removed upon neuron removal") {
        b->remove_neuron(n2);
        REQUIRE(n1->connections_.size() == 0);
      }
    }
  }
}

TEST_CASE("brain operations", "[brain]") {
  auto bm = std::make_shared<brain_mass>();

  SECTION("events are queued") {
    auto n1 = bm->add_neuron(std::make_shared<neuron>(1, 0.5));
    auto n2 = bm->add_neuron(std::make_shared<neuron>(0, 0.5));
    bm->add_connection(n1, std::make_shared<connection>(n2, 1, 1));
    bm->add_connection(n2, std::make_shared<connection>(n1, 1, 1));

    auto b = brain(bm);
    n1->apply_pulse(1, 0, &b);

    REQUIRE(b.events_in_queue() == 1);

    SECTION("events are processed") {
      b.run(false);
      REQUIRE(b.events_in_queue() == 0);
    }
  }
}

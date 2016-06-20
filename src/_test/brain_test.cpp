//
// Created by gregor on 20.06.16.
//

#include "catch/catch.hpp"
#include "../brain.h"

TEST_CASE("brain modifications", "[brain]") {
    brain b;

    SECTION("neurons can be added") {
        auto n1 = b.add_neuron(std::make_shared<neuron>(1, 0.5));
        REQUIRE(b.neurons().size() == 1);

        SECTION("new neurons have no connections") {
            REQUIRE(n1->connections_.size() == 0);
        }

        SECTION("neurons can be deleted") {
            b.remove_neuron(n1);
            REQUIRE(b.neurons().size() == 0);
        }

        SECTION("connections can be added") {
            auto n2 = b.add_neuron(std::make_shared<neuron>(1, 0.5));
            auto c = std::make_shared<connection>(n2, 1, 1);
            b.add_connection(n1, c);
            REQUIRE(n1->connections_.size() == 1);

            SECTION("connections can be deleted") {
                b.remove_connection(n1, c);
                REQUIRE(n1->connections_.size() == 0);
            }

            SECTION("connections are removed upon neuron removal") {
                b.remove_neuron(n2);
                REQUIRE(n1->connections_.size() == 0);
            }
        }
    }
}

#include <spdlog/spdlog.h>
#include "brain/runner/brain_runner.h"
#include "gui/gui.h"

int main() {
  spdlog::info("running brain");
  {
    brain_runner runner;
    runner.run();
    gui g(&runner.brain_);
    spdlog::info("waiting for user to close GUI...");
    g.run();  // blocks
    spdlog::info("waiting for runner to finish...");
  }
  spdlog::info("done.");

  return 0;
}

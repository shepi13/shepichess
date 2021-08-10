#include "uci_application.h"

#include <sstream>

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_string.hpp>

using Catch::Matchers::Contains;
using Catch::Matchers::EndsWith;

TEST_CASE("uci_application command uci", "[application]")
{
  std::stringstream in {"uci\nquit\n"};
  std::stringstream out;
  shepichess::UCIApp app(in, out);
  app.mainLoop();
  REQUIRE_THAT(
    out.str(), Contains("id author") && Contains("id name") && EndsWith("uciok\n"));
}

TEST_CASE("uci_application command isready", "[application]")
{
  std::stringstream in {"isready\nquit\n"};
  std::stringstream out;
  shepichess::UCIApp app(in, out);
  app.mainLoop();
  REQUIRE(out.str() == "readyok\n");
}
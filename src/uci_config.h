#pragma once

#include <string>
#include <vector>

namespace shepichess {

class UCIOption {
  public:
  inline std::string uciString() { return ""; }
};

class UCIConfig {
  public:
  bool setOption(const std::string&, const std::string&);
  inline std::vector<UCIOption> getAvailableOptions()
  {
    return std::vector<UCIOption>();
  }
};

} // namespace shepichess

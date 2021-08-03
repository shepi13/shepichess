#pragma once

#include <string>

namespace shepichess {

class UCIConfig {
public:
  bool setOption(const std::string&, const std::string&);
};

} // namespace shepichess

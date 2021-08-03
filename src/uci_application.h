#pragma once

#include "position.h"
#include "uci_config.h"

namespace shepichess {

constexpr const char* kEngineName = "shepichess";
constexpr const char* kEngineAuthor = "shepi13";

class UCIApp {
public:    
  UCIApp();
  UCIApp(const UCIApp&) = delete;
  UCIApp& operator=(const UCIApp&) = delete;

  void mainLoop();
private:
  Position position;
  UCIConfig config;
};
  

} // namespace shepichess

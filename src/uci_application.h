#pragma once

#include <iostream>
#include <string>

#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

#include "position.h"
#include "uci_config.h"

namespace shepichess {

const inline std::string kEngineName {"shepichess"};
const inline std::string kEngineAuthor {"shepi13"};

class UCIApp {
public:
  UCIApp(std::istream& in = std::cin, std::ostream& out = std::cout);
  ~UCIApp() = default;

  UCIApp(const UCIApp&) = delete;
  UCIApp(UCIApp&&) = delete;
  UCIApp& operator=(const UCIApp&) = delete;
  UCIApp& operator=(UCIApp&&) = delete;

  void mainLoop();
  static void init();

private:
  Position position;
  UCIConfig config;
  bool uciDebugMode = false;
  std::istream& in;
  std::ostream& out;

  // IO Handlers
  struct UCICommand;
  UCICommand getUCICommand();
  void sendUCICommand(const std::string&);

  // UCI Command Handlers:
  void respondUCI();
  void respondReady();
  void uciNewGame();
  void setDebugMode(const std::string& args);
  void setOption(const std::string& args);
  void setPosition(const std::string& args);
  void startCalculation(const std::string& args);
  void stopCalculation();
  void ponderhit();
};

} // namespace shepichess

#include "uci_application.h"

#include <iostream>
#include <string>
#include <utility>

#include "bitboard.h"
#include "logging.h"

using std::string;

namespace shepichess {

namespace {

  void respondUCI() 
  {

  }

  void setDebugMode(const string& line)
  {

  }
  
  void respondWhenReady()
  {

  }

  std::pair<string, string> parseOptionString(const string& line)
  {

  }

} // namespace

UCIApp::UCIApp()
  :position(),
  config()
{
  initLogging();
  bitboards::init(); 
}

void UCIApp::mainLoop() {
  string line;
  while(true) {
    std::getline(std::cin, line);

    if(line.rfind("quit", 0) == 0) break;
    if(line.rfind("uci", 0) == 0) respondUCI();
    if(line.rfind("debug", 0) == 0) setDebugMode(line);
    if(line.rfind("isready", 0) == 0) respondWhenReady();
    if(line.rfind("setoption", 0) == 0) {
      auto&& [name, value] = parseOptionString(line);
      if(!config.setOption(name, value)) {
        SPDLOG_ERROR("Failed to set UCI Option: {name}: {value}");        
      }
    }
    if(line.rfind("position", 0) == 0) {
  
    }
    if(line.rfind("go", 0) == 0) {
      
    }
    if(line.rfind("stop", 0) == 0){
    
    }
    if(line.rfind("ponderhit", 0) == 0) {

    }
  }
}

} // namespace shepichess

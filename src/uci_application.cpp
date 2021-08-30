#include "uci_application.h"

#include <iostream>
#include <mutex>
#include <regex>
#include <sstream>
#include <string>

#include <spdlog/spdlog.h>

#include "bitboard.h"

namespace shepichess {

namespace {

const std::string kWhiteSpace = " \t\n\r\f\v";
std::string trim(std::string str, const std::string& trim_chars = kWhiteSpace)
{
  str.erase(str.find_last_not_of(trim_chars) + 1);
  str.erase(0, str.find_first_not_of(trim_chars));
  return str;
}

void initLogging()
{
  auto&& stderr_logger = spdlog::stderr_color_mt("shepichess_stderr_logger");
  stderr_logger->set_level(spdlog::level::trace);
  spdlog::set_level(spdlog::level::trace);
  spdlog::set_default_logger(stderr_logger);
  spdlog::set_pattern("[%Y-%m-%d %H:%M:%S] [thread %t] [%^%l%$] [%s:%#] %v");
  SPDLOG_INFO("Created logger with level: {}", SPDLOG_ACTIVE_LEVEL);
}

} // namespace

struct UCIApp::UCICommand {
  std::string command;
  std::string args;
};

void UCIApp::init()
{
  static std::once_flag init_flag;
  std::call_once(init_flag, []() {
    initLogging();
    initBitboards();
    initZobrist();
  });
}

UCIApp::UCIApp(std::istream& in, std::ostream& out) : in(in), out(out) {}

void UCIApp::mainLoop()
{
  while (true) {
    auto&& [command, args] = getUCICommand();

    if (command == "quit")
      break;
    else if (command == "uci")
      respondUCI();
    else if (command == "isready")
      respondReady();
    else if (command == "debug")
      setDebugMode(args);
    else if (command == "setoption")
      setOption(args);
    else if (command == "position")
      setPosition(args);
    else if (command == "go")
      startCalculation(args);
    else if (command == "stop")
      stopCalculation();
    else if (command == "ponderhit")
      ponderhit();
    else {
      // Invalid UCI commands must be ignored, but we still want to log them.
      SPDLOG_ERROR("Invalid UCI command: command unrecognized");
    }
  }
}

UCIApp::UCICommand UCIApp::getUCICommand()
{
  std::string line, command, args;
  std::getline(in, line);
  SPDLOG_INFO("INPUT: \"{}\"", line);
  std::istringstream iss {line};
  iss >> command >> std::ws;
  std::getline(iss, args);
  return UCICommand {command, args};
}

void UCIApp::sendUCICommand(const std::string& line)
{
  out << line << std::endl;
  SPDLOG_INFO("OUTPUT: \"{}\"", line);
}

void UCIApp::respondUCI()
{
  sendUCICommand("id name " + kEngineName);
  sendUCICommand("id author " + kEngineAuthor);
  for (auto&& option : config.getAvailableOptions()) {
    std::string uciString = option.uciString();
    sendUCICommand("option " + uciString);
    SPDLOG_DEBUG("Available Option: {}", uciString);
  }
  sendUCICommand("uciok");
}

void UCIApp::respondReady()
{
  sendUCICommand("readyok");
}

// TODO: Probably just will clear hash tables?
void UCIApp::uciNewGame() {}

void UCIApp::setDebugMode(const std::string& args)
{
  if (args == "on") {
    SPDLOG_INFO("Setting UCIDebugMode to ON (was {})", uciDebugMode ? "ON" : "OFF");
    uciDebugMode = true;
  } else if (args == "off") {
    SPDLOG_INFO("Setting UCIDebugMode to OFF (was {})", uciDebugMode ? "ON" : "OFF");
    uciDebugMode = false;
  } else {
    SPDLOG_ERROR("UCI: debug: Failed to parse args \"{}\"", args);
  }
}

void UCIApp::setOption(const std::string& args)
{
  std::smatch nameMatch, valueMatch;
  std::regex_search(args, nameMatch, std::regex(R"(name\s*(.*?[^\s])\s*(?:$|value))"));
  std::regex_search(args, valueMatch, std::regex(R"(value\s*(.*?[^\s])\s*(?:$|name))"));
  std::string name = nameMatch[1];
  std::string value = valueMatch[1];
  if (!name.size()) {
    SPDLOG_ERROR("UCI: setoption: failed to parse option name from \"{}\"", args);
    return;
  }
  SPDLOG_DEBUG("UCI: setoption: name=\"{}\" value=\"{}\"", name, value);
  config.setOption(name, value);
}

void UCIApp::setPosition(const std::string& args)
{
  std::string fen = trim(args.substr(0, args.find("moves")));
  if (fen == "startpos") {
    fen = kStartPositionFen;
  }
  SPDLOG_INFO("Found fen {}", fen);
  position.setPosition(fen);
}

// TODO: depends on search implementation
void UCIApp::startCalculation(const std::string& args) {}

// TODO: depends on search implementation
void UCIApp::stopCalculation() {}

void UCIApp::ponderhit()
{
  // TODO: Add when pondering support is added
}

} // namespace shepichess

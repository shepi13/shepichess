#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE
#include "uci_application.h"

int main()
{
  shepichess::UCIApp::init();
  shepichess::UCIApp app;
  app.mainLoop();
}

#include "uci_application.h"

int main()
{
  shepichess::UCIApp::init();
  shepichess::UCIApp app;
  app.mainLoop();
}

//
// Created by tobias on 29/09/16.
//

#include <iostream>
#include "rlutil/rlutil.h"
#include "sockserver/server.h"

int main() {
  rlutil::setBackgroundColor(rlutil::MAGENTA);
  rlutil::setColor(rlutil::BLUE);
  std::cout << "Hello World";
  Server* server = new Server(1111, "/yo_mama/");
  free(server);
}
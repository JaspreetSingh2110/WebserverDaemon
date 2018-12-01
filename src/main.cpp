//
//  main.cpp
//  WebServerDaemon
//
//  Created by Jaspreet Singh on 01/12/18.
//  Copyright © 2018 Jaspreet Singh. All rights reserved.
//

#include "main.hpp"
#include "webserver_daemon.h"

int main () {
  WebserverDaemon daemon;
  daemon.OnInit();
  std::cout<< "Webserver daemon started\n";
  
  return 0;
}

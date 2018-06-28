# WebServer_ContentAdaptation
## AGA Server

Project focus: realize a concurrent Web server that adapt dinamically the static contents depending on device-client features.

Course: Ingegneria di Internet e Web

University: Università di Roma Tor Vergata

Team: 3 people

### Requirements
 
1- ImageMagick  available to the link: http://www.imagemagick.org/script/install-source.php

2- SQLite3
 
### Installation and Instructions

To install the executable file:

$ source instal

Three scenarios for the configuration:



1) $ ./server

The default value are set (port: 5193, 7 child).

2) $ ./server 1234

The port 1234 and default number of child (7) are set.

3) $ ./server 1234 5

The port 1234 and 5 child are set.

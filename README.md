# trackerino

## depencies
- Gtkmm-3.0
  - sudo apt install libgtkmm-3.0-dev
- MySQL Conector/C++
  - https://dev.mysql.com/downloads/connector/cpp/
  - sudo apt install libmysqlcppconn-dev
- Garmin FIT SDK
  - https://developer.garmin.com/fit/download/
  - tested with FIT 21.141 SDK

## compiling
``g++ -c *.cpp
g++ -std=c++17 -c *.cc $(pkg-config --cflags --libs gtkmm-3.0) -lmysqlcppconn
g++ -std=c++17 *.o -o trackerino $(pkg-config --cflags --libs gtkmm-3.0) -lmysqlcppconn``

# Prometheus Exporter using C++

## Supported Platforms
 - requires a compiler that conforms to the C++23 standard or higher

### Operating Systems
 - CentOS
   - Stream release 9
 - Ubuntu
   - 20.04 LTS
   - 22.04 LTS

### Compiler
 - gcc version 13.1.0

### Build System
 - cmake version 3.20.6 or higher

<br/>

## Build
 - prepare
   - CentOS
     - `dnf install curl-devel`
   - Ubuntu
     - `sudo apt install libcurl4-openssl-dev`
 - basic
   - `mkdir build && cd build`
   - `cmake ../ && make -j4 install`
   - output
     - `./install/prometheus-exporter`
 - test
   - `mkdir build && cd build`
   - `cmake ../ -DENABLE_TEST=on && make -j4 install && make test`
 - code coverage
   - prepare
     - CentOS
       - `dnf install perl-FindBin`
       - `dnf install perl-Time-HiRes`
       - `dnf install perl-CPAN`
       - `perl -MCPAN -e 'install Capture::Tiny'`
       - `perl -MCPAN -e 'install DateTime'`
       - `perl -MCPAN -e 'install Date::Parse'`
       - `perl -MCPAN -e 'install JSON::XS'`
     - Ubuntu
       - `sudo perl -MCPAN -e 'install Capture::Tiny'`
       - `sudo perl -MCPAN -e 'install DateTime'`
   - `mkdir build && cd build`
   - `cmake ../ -DENABLE_TEST=on && make -j4 install && make test && make lcov_run`
   - run `./coverage/html/index.html` in your browser

<br/>

## Operation
 - `./install/prometheus-exporter --address 0.0.0.0:10000 --uri metrics`
 - run `127.0.0.1:10000/metrics` in your browser

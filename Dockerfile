FROM gcc:13.1 AS build
WORKDIR /work
RUN wget https://github.com/Kitware/CMake/releases/download/v3.20.6/cmake-3.20.6-linux-x86_64.tar.gz
RUN tar xvzf cmake-3.20.6-linux-x86_64.tar.gz
RUN ls -al /work/cmake-3.20.6-linux-x86_64/bin
ENV PATH=/work/cmake-3.20.6-linux-x86_64/bin:$PATH
COPY ./ ./
RUN rm -rf build
RUN mkdir -p build
RUN cd build && cmake ../ && make -j4 install

FROM scratch
COPY --from=build /work/build/src/prometheus-exporter /
ENTRYPOINT ["/prometheus-exporter"]

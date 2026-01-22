FROM ubuntu:24.04
RUN apt update && apt install -y \
    build-essential cmake git \
    qt6-base-dev libqt6widgets6 \
    libopencv-dev \
    wget gpg

# Install Intel oneAPI
RUN wget -O- https://apt.repos.intel.com/intel-gpg-keys/GPG-PUB-KEY-INTEL-SW-PRODUCTS.PUB | gpg --dearmor > /usr/share/keyrings/oneapi.gpg
RUN echo "deb [signed-by=/usr/share/keyrings/oneapi.gpg] https://apt.repos.intel.com/oneapi all main" > /etc/apt/sources.list.d/oneAPI.list
RUN apt update && apt install -y intel-basekit

WORKDIR /app
COPY . .
RUN mkdir build && cd build && \
    bash -c 'source /opt/intel/oneapi/setvars.sh && cmake -DCMAKE_CXX_COMPILER=icpx .. && make -j4'

CMD ["bash", "-c", "source /opt/intel/oneapi/setvars.sh && ./build/gui/ImageFlowGUI"]

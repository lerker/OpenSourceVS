FROM ubuntu:16.04
LABEL Description="This image is used to start the Video Resume app executable" Vendor="Ponzoni Nelson" Version="0.20170408"

ENV CV_VERSION '2.4.13'
ENV LD_LIBRARY_PATH $LD_LIBARARY_PATH:/usr/local/lib

RUN apt-get update && \
    apt-get install -y --no-install-recommends \
    build-essential \
    cmake \	
    git \
    ca-certificates \
    wget \
    unzip \
    # 				basic stuff
    pkg-config \
    libgtk2.0-dev \
    libavcodec-dev \
    libavformat-dev \
    libswscale-dev \
    libtbb2 \
    libtbb-dev \
    libjpeg-dev \
    libpng-dev \
    libtiff-dev \
    libjasper-dev \
    # 				python stuff
    #python3-dev \
    #python3-numpy \
    #python3-pip \
    #python3-scipy \
    #python3-matplotlib \
    #python-dev \
    #python-pip \
    #python-tk \
    # 				lib stuff
    libqt4-dev \
    libqt4-opengl-dev \
    libeigen3-dev \
    libopencore-amrnb-dev \
    libopencore-amrwb-dev \
    libtheora-dev \
    libvorbis-dev \
    libxvidcore-dev \
    libv4l-dev \
    libdc1394-22-dev \
    libavcodec-dev \
    libavformat-dev \
    ##default-jdk \
    ##ant \
    ##yasm \
    # 				GUI stuff
    qt4-default \
    libvtk5-qt4-dev \
    libgtk2.0-dev \
    libgtkglext1 \
    libgtkglext1-dev \
    # 				image manipulation
    libpng3 \
    pngtools \
    #libpng12-dev \
    #libpng12-0 \
    libpng++-dev \
    libjpeg-dev \
    libjpeg-progs \
    libjpeg9 \
    libjpeg9-dbg \
    #libtiff5-dev \
    #libtiff5 \
    #libtiffxx5 \
    #libtiff-tools \
    libjasper-dev \
    libjasper-runtime \
    libjasper1  \
    zlib1g-dbg \
    zlib1g-dev \
    # 				video manipulation
    libavformat-dev \
    libavutil-dev \
    libxine2-dev \
    libxine2 \
    libswscale-dev \
    libswscale-ffmpeg3 \
    libdc1394-22 \
    libdc1394-22-dev \
    libdc1394-utils \
    # 				codecs
    libavcodec-dev \
	libmp3lame-dev \
	libopencore-amrnb-dev \
	libopencore-amrwb-dev \
	libtheora-dev \
	libvorbis-dev \
	libxvidcore-dev \
	ffmpeg \
	x264 \
	libx264-dev \
	libv4l-0 \ 
	v4l-utils \
	#libpng16-dev \
	libavutil-ffmpeg54 \
    #				install opencv
    && CV_VERSION='2.4.13' \
    && wget --progress=bar:force https://github.com/Itseez/opencv/archive/"$CV_VERSION".zip \
    && unzip "$CV_VERSION".zip \
    && mkdir /opencv-"$CV_VERSION"/cmake_binary \
    && cd /opencv-"$CV_VERSION"/cmake_binary \
    && cmake -DCMAKE_BUILD_TYPE=RELEASE -DCMAKE_INSTALL_PREFIX=/usr/local -DINSTALL_C_EXAMPLES=ON -DINSTALL_PYTHON_EXAMPLES=OFF -DBUILD_EXAMPLES=ON -DBUILD_opencv_cvv=OFF -DBUILD_NEW_PYTHON_SUPPORT=OFF -DBUILD_DOCS=OFF -DBUILD_PERF_TESTS=OFF -DBUILD_TESTS=OFF -DWITH_TBB=ON -DWITH_V4L=ON -DWITH_QT=ON -DWITH_OPENGL=ON -DWITH_VTK=ON .. \
    && make -j$(nproc) \
    && make install -j$(nproc) \
    && rm /"$CV_VERSION".zip \
    && rm -r /opencv-"$CV_VERSION" \
    #				install VideoSourceVS
    && cd /root/ \
    && git clone https://github.com/lerker/OpenSourceVS.git \
    && cd OpenSourceVS/ \
    && g++ -std=c++11 -o OpenSourceVS main.cpp `pkg-config --cflags --libs opencv` \
    && cd /root/ \
    && cp OpenSourceVS/OpenSourceVS opensourcevs \
    && cp OpenSourceVS/Video\ 2/Video\ original\ 2.avi video2.avi \
    #&& ./opensourcevs video2.avi \
    #				limpiar
    && apt-get -y clean all \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /root
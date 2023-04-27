# Run this with one of the following commands. This will compile the code
# and save the result into a folder or an archive at the host machine
# if the compilation is succesful. Otherwise the error messages will be
# printed at the console.
# 
# This is a WIP.
# 
# DOCKER_BUILDKIT=1 docker build --output type=local,dest=webkitty-os4 .
# DOCKER_BUILDKIT=1 docker build --output type=tar,dest=webkitty-os4.tar .

FROM walkero/amigagccondocker:ppc-amigaos-gcc11-newclib2 AS build-stage

LABEL maintainer="Georgios Sokianos <walkero@gmail.com>"

# TODO: Necessary for the jscore-native compilation. Need to move to separate dockerfile
# ENV PACKAGES="build-essential \
# 	ccache \
# 	libcairo2-dev \
# 	libicu-dev \
# 	ruby-dev"

ENV PACKAGES="build-essential \
	ccache"

ENV OS4_SDK_PATH="/opt/sdk/ppc-amigaos"

RUN apt-get update && apt-get -y --no-install-recommends install ${PACKAGES};

# TODO: Necessary for the jscore-native compilation. Need to move to separate dockerfile
# RUN apt-get update && apt-get -y --no-install-recommends install ${PACKAGES} && \
# 	gem install json highline

WORKDIR /tmp
RUN curl -fsSL "https://walkero.gr/betas/clib2_beta8_selfcompiled.tar.xz" -o /tmp/clib2.tar.xz && \
	tar xvf clib2.tar.xz && \
	cp -r clib2 ${OS4_SDK_PATH}; \
    mkdir ${OS4_SDK_PATH}/local/clib2/include; \
    rm -rf /tmp/*;

RUN git clone https://github.com/kas1e/Odyssey.git --depth 1 /tmp/Odyssey && \
	cp -r /tmp/Odyssey/odyssey-r155188-1.23_SDK/SDK/local/common/include/cairo ${OS4_SDK_PATH}/local/newlib/include/ && \
	cp /tmp/Odyssey/odyssey-r155188-1.23_SDK/SDK/local/newlib/lib/libcairo.a ${OS4_SDK_PATH}/local/newlib/lib/; \
	rm -rf /tmp/*;

RUN curl -fsSL "https://github.com/3246251196/icu/raw/main/icu4c/source/libicu.lha" -o /tmp/libicu.lha && \
    lha -xfq2 libicu.lha && \
    \cp ./libicu/SDK/* ${OS4_SDK_PATH}/ -R && \
    rm -rf /tmp/*;

RUN curl -fsSL "https://github.com/3246251196/libpng/raw/libpng16/libpng.lha" -o /tmp/libpng.lha && \
    lha -xfq2 libpng.lha && \
    \cp ./libpng/SDK/* ${OS4_SDK_PATH}/local/ -R && \
    rm -rf /tmp/*;

ADD . /opt/code/webkitty
WORKDIR /opt/code/webkitty

# The following steps can be changed to get the final build binaries
RUN make Dummy/libdummy.a && \
	cp Dummy/libdl.a /opt/sdk/ppc-amigaos/local/clib2/lib/ && \
	make jscore-amigaos

FROM scratch AS export-stage
COPY --from=build-stage /opt/code/webkitty /

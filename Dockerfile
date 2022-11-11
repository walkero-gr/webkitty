# Run this with one of the following commands. This will compile the code
# and save the result into a folder or an archive at the host machine
# 
# This is still a WIP. Now it builds only the native x86 code
# 
# DOCKER_BUILDKIT=1 docker build --output type=local,dest=out .
# DOCKER_BUILDKIT=1 docker build --output type=tar,dest=out.tar .

FROM walkero/amigagccondocker:ppc-amigaos-gcc11-newclib2 AS build-stage

ENV PACKAGES="build-essential \
	ccache \
	libcairo2-dev \
	libicu-dev \
	ruby-dev"

RUN apt-get update && apt-get -y --no-install-recommends install ${PACKAGES} && \
	gem install json highline

ADD . /opt/code/webkitty
WORKDIR /opt/code/webkitty

# The following steps can be changed to get the final build binaries
RUN make jscore-amigaos

FROM scratch AS export-stage
COPY --from=build-stage /opt/code/webkitty /


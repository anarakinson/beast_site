FROM ubuntu:20.04

# set working dir
WORKDIR /usr/src/app

# set working port
EXPOSE 3333

# set working language
ENV LANG C.UTF-8

# set timezone
ENV TZ="America/New_York"
RUN ln -snf /usr/share/zoneinfo/$TZ /etc/localtime && echo $TZ > /etc/timezone

# # avoiding installation questions
# ARG DEBIAN_FRONTEND=noninteractive

# copy content to container
COPY . /usr/src/app

# update repositories
RUN apt update

# install required software
RUN apt install -y cmake
RUN apt install -y git
RUN apt install -y curl wget zip unzip tar
RUN apt install -y gcc g++
RUN apt install -y pkg-config 

# install cpp libraries via apt
# RUN apt install -y libboost-all-dev>=1.66.0
# RUN apt install -y cget
# RUN cget install pantor/inja -y

# install vcpkg 
RUN git clone --depth 1 https://github.com/Microsoft/vcpkg.git vcpkg
RUN ./vcpkg/bootstrap-vcpkg.sh
RUN ./vcpkg/vcpkg integrate install

# # install cpp libraries via vcpkg
# RUN ./vcpkg/vcpkg install jsoncpp
# RUN ./vcpkg/vcpkg install openssl
# RUN ./vcpkg/vcpkg install cpp-httplib
RUN ./vcpkg/vcpkg install boost-beast
RUN ./vcpkg/vcpkg install boost-property-tree
RUN ./vcpkg/vcpkg install boost-program-options
RUN ./vcpkg/vcpkg install inja


# build project
RUN cmake -B build -DCMAKE_TOOLCHAIN_FILE=/usr/src/app/vcpkg/scripts/buildsystems/vcpkg.cmake
RUN cmake --build build 

# run application
# ENTRYPOINT [ "./build/website", "0.0.0.0", "3333", "./app/", "4" ] 
ENTRYPOINT ./build/website 0.0.0.0 3333 ./app/ 4

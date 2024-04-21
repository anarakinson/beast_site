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
RUN apt install -y libboost-all-dev
RUN apt install -y gcc g++
# RUN apt install -y pkg-config # ??? 
RUN apt install -y curl wget zip unzip tar
RUN apt install -y cget

# install inja
RUN cget install pantor/inja -y

# # configure git
# RUN git config --global http.postBuffer 2M
# # RUN git config --global core.compression 0

# # install vcpkg 
# RUN git clone --depth 1 https://github.com/Microsoft/vcpkg.git vcpkg
# RUN ./vcpkg/bootstrap-vcpkg.sh
# RUN ./vcpkg/vcpkg integrate install

# # install cpp libraries
# RUN ./vcpkg/vcpkg install jsoncpp
# RUN ./vcpkg/vcpkg install openssl
# RUN ./vcpkg/vcpkg install cpp-httplib
# RUN ./vcpkg/vcpkg install inja


# build project
RUN cmake -B build 
### -DCMAKE_TOOLCHAIN_FILE=/usr/src/app/vcpkg/scripts/buildsystems/vcpkg.cmake
RUN cmake --build build 

# run application
ENTRYPOINT [ "./build/website" ] 

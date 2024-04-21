# cd ..

git clone https://github.com/Microsoft/vcpkg.git

./vcpkg/bootstrap-vcpkg.sh
./vcpkg/vcpkg integrate install

./vcpkg/vcpkg list

# ./vcpkg/vcpkg search sdl2
# ./vcpkg/vcpkg install sdl2:x64-linux # sdl2:arm64-osx // sdl2:arm
# ./vcpkg/vcpkg list
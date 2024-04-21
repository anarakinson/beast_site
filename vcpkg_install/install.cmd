@REM cd ..

git clone https://github.com/Microsoft/vcpkg.git

.\vcpkg\bootstrap-vcpkg.bat
.\vcpkg\vcpkg integrate install

.\vcpkg\vcpkg list

@REM commands:
@REM .\vcpkg\vcpkg search sdl2       - install pkg
@REM .\vcpkg\vcpkg install sdl2:x64-windows @REM sdl2:x84-windows // sdl2:arm
@REM .\vcpkg\vcpkg list              - show installed pkgs
@REM .\vcpkg\vcpkg help              - show help
@REM .\vcpkg\vcpkg help triplet      - show available triplets
@REM .\vcpkg\vcpkg integrate install - set up integrate install and show dependency path for cmake 
@REM                                 - like -DCMAKE_TOOLCHAIN_FILE=.../vcpkg/scripts/buildsystems/vcpkg.cmake

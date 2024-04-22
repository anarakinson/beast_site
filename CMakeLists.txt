cmake_minimum_required(VERSION 3.5)


project(website)


if(MSVC)
    set(OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR}/Debug)
else()
    set(OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR}/)
    # set(CMAKE_CXX_FLAGS -pthread) # ???
endif()


find_package(Boost COMPONENTS system program_options REQUIRED)
find_package(Threads REQUIRED)
find_package(inja CONFIG REQUIRED)



add_executable(
    website
    main.cpp
)

file(
    COPY 
    ${CMAKE_CURRENT_SOURCE_DIR}/app 
    ${CMAKE_CURRENT_SOURCE_DIR}/configs 
    DESTINATION 
    ${OUTPUT_DIR}
)

set_target_properties(
    website PROPERTIES 
    CXX_STANDARD 20 
    CXX_STANDARD_REQUIRED ON 
)

include_directories(
    ${CMAKE_SOURCE_DIR}/include
    ${Boost_INCLUDE_DIR}
)


target_link_libraries(
    website PRIVATE 
    pantor::inja
    Threads::Threads
    ${Boost_LIBRARIES}
)


# set compile options
if (MSVC)
    target_compile_options(website PRIVATE
        # /W4
    )
else ()
    target_compile_options(website PRIVATE
        # -Wall 
        # -Wextra 
        # -pedantic 
        # -Werror
    )
endif()


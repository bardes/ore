SET(src
    core/Map.cpp
    core/Utils.cpp
    tinyxml2/tinyxml2.cpp
)

add_library(ore ${src})

target_link_libraries(ore z)

add_executable(test main.cpp)

target_link_libraries(test ore)
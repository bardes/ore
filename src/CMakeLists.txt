SET(src
    core/Map.cpp
    core/Tileset.cpp
    core/Utils.cpp
    tinyXml/tinystr.cpp
    tinyXml/tinyxml.cpp
    tinyXml/tinyxmlerror.cpp
    tinyXml/tinyxmlparser.cpp
)

add_library(ore ${src})

target_link_libraries(ore z)

install(DIRECTORY core tinyXml DESTINATION include
        FILES_MATCHING PATTERN *.hpp PATTERN *.h)

install(TARGETS ore ARCHIVE DESTINATION lib)

add_executable(teste core/main.cpp)
target_link_libraries(teste ore SDL SDL_image z)
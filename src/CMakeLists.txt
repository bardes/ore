SET(src
    ore/Base64.cpp
    ore/Map.cpp
    ore/Tileset.cpp
    ore/Layer.cpp
    tinyXml/tinystr.cpp
    tinyXml/tinyxml.cpp
    tinyXml/tinyxmlerror.cpp
    tinyXml/tinyxmlparser.cpp
)

add_library(ore ${src})

target_link_libraries(ore sfml-system sfml-graphics sfml-window)

install(DIRECTORY ore tinyXml DESTINATION include
        FILES_MATCHING PATTERN *.hpp PATTERN *.h)

install(TARGETS ore ARCHIVE DESTINATION lib)

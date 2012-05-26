SET(src
    ore/Map.cpp
    ore/Tileset.cpp
    ore/Utils.cpp
    tinyXml/tinystr.cpp
    tinyXml/tinyxml.cpp
    tinyXml/tinyxmlerror.cpp
    tinyXml/tinyxmlparser.cpp
)

add_library(ore ${src})

install(DIRECTORY ore tinyXml DESTINATION include
        FILES_MATCHING PATTERN *.hpp PATTERN *.h)

install(TARGETS ore ARCHIVE DESTINATION lib)

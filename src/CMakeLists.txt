SET(src
    ore/Base64.cpp
    ore/Base64.hpp
    ore/Image.cpp
    ore/Image.hpp
    ore/Map.cpp
    ore/Map.hpp
    ore/Tileset.cpp
    ore/Tileset.hpp
    ore/Layer.cpp
    ore/Layer.hpp
    ore/Object.cpp
    ore/Object.hpp
    ore/Resource.cpp
    ore/Resource.hpp
    ore/ResourceManager.cpp
    ore/ResourceManager.hpp
    ore/Utils.cpp
    ore/Utils.hpp
    tinyXml/tinystr.cpp
    tinyXml/tinystr.h
    tinyXml/tinyxml.cpp
    tinyXml/tinyxml.h
    tinyXml/tinyxmlerror.cpp
    tinyXml/tinyxmlparser.cpp
)

add_library(ore ${src})

target_link_libraries(ore sfml-system sfml-graphics sfml-window)

#install(DIRECTORY ore tinyXml DESTINATION include
#        FILES_MATCHING PATTERN *.hpp PATTERN *.h)
install(DIRECTORY ore DESTINATION include
        FILES_MATCHING PATTERN *.hpp)

install(TARGETS ore ARCHIVE DESTINATION lib)

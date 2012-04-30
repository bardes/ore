SET(src
    core/Map.cpp
    core/Utils.cpp
    tinyXml/tinystr.cpp
    tinyXml/tinyxml.cpp
    tinyXml/tinyxmlerror.cpp
    tinyXml/tinyxmlparser.cpp
)

add_library(ore ${src})

target_link_libraries(ore z)

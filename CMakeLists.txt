cmake_minimum_required(VERSION 3.17)

project(easy_translate VERSION 1.2.0 LANGUAGES CXX)

add_library(${PROJECT_NAME} INTERFACE)
add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

include(GNUInstallDirs)

set(3RDPARTY ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty)

option(EASY_TRANSLATE_USE_BUNDLED_JSON "Use the bundled nlohmann_json instead of requiring an externally installed one" ON)

if(EASY_TRANSLATE_USE_BUNDLED_JSON)
    if(NOT TARGET nlohmann_json::nlohmann_json)
        add_subdirectory(${3RDPARTY}/json)
        set(EASY_TRANSLATE_JSON_VENDORED TRUE)
    else()
        set(EASY_TRANSLATE_JSON_VENDORED FALSE)
    endif()
    target_link_libraries(${PROJECT_NAME} INTERFACE $<BUILD_INTERFACE:nlohmann_json::nlohmann_json>)
else()
    set(EASY_TRANSLATE_JSON_VENDORED FALSE)
    find_package(nlohmann_json REQUIRED)
    target_link_libraries(${PROJECT_NAME} INTERFACE nlohmann_json::nlohmann_json)
endif()
target_include_directories(
    ${PROJECT_NAME} INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

include(CMakePackageConfigHelpers)
configure_package_config_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/${PROJECT_NAME}Config.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
    PATH_VARS CMAKE_INSTALL_INCLUDEDIR CMAKE_INSTALL_LIBDIR
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
)

write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

install(
    TARGETS ${PROJECT_NAME}
    EXPORT ${PROJECT_NAME}
)

install(
    EXPORT ${PROJECT_NAME}
    FILE ${PROJECT_NAME}Targets.cmake
    NAMESPACE ${PROJECT_NAME}::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
)

install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
if(EASY_TRANSLATE_JSON_VENDORED)
    install(DIRECTORY ${3RDPARTY}/json/include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
endif()
install(DIRECTORY doc/ DESTINATION ${CMAKE_INSTALL_DOCDIR})
install(FILES LICENSE DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/license/${PROJECT_NAME})
install(
    FILES
    ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
)

option(EASY_TRANSLATE_BUILD_EXAMPLE "Build the easy translate example program (need Qt)" OFF)
if(EASY_TRANSLATE_BUILD_EXAMPLE)
    add_subdirectory(./example/base_example)
endif()

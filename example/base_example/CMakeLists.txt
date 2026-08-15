cmake_minimum_required(VERSION 3.17)

project(EasyTranslate-BaseExample LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

file(GLOB SRC *.c *.cpp)
file(GLOB UI *.ui)
set(PROJECT_SOURCE ${SRC} ${UI})

qt_standard_project_setup()
qt_add_executable(${PROJECT_NAME} ${PROJECT_SOURCE})

target_link_libraries(
    ${PROJECT_NAME} PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    easy_translate::easy_translate
)

set_target_properties(
    ${PROJECT_NAME} PROPERTIES
    AUTOMOC ON
    AUTOUIC ON
    WIN32_EXECUTABLE TRUE
)

option(UPDATE_TRANSLATIONS_MAPPING_FILES "Whether update the translations mapping files" ON)
target_compile_definitions(
    ${PROJECT_NAME} PRIVATE
    $<$<BOOL:${UPDATE_TRANSLATIONS_MAPPING_FILES}>:EASY_TRANSLATE_UPDATE_TRANSLATIONS_MAPPING_FILES>
)

# Copy the langugaes directory to target directory after build.
add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different
    ${CMAKE_CURRENT_SOURCE_DIR}/languages $<TARGET_FILE_DIR:${PROJECT_NAME}>/languages
)

install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)
install(DIRECTORY languages DESTINATION bin)

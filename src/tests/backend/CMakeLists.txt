file(GLOB HEADER_LIST CONFIGURE_DEPENDS "${3D_Viewer_SOURCE_DIR}/tests/backend/*.h")

add_executable("tests_backend"
    tests_main.c
    master_suite.c
    tests_transformations.c
    tests_loader.c
    ${HEADER_LIST}
)

target_link_libraries("tests_backend"
    PRIVATE "3D_Viewer_backend" ${CHECK_LINK_LIBRARIES} pthread
)

target_include_directories(
    "tests_backend"
    PRIVATE ${CHECK_INCLUDE_DIRS}
)

add_test(
    NAME tests_backend
    COMMAND tests_backend
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

file(GLOB HEADER_LIST CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/include/backend/*.h")

add_library(
  "3D_Viewer_backend" STATIC
  transformations.c
  ${HEADER_LIST}
)

target_include_directories(
    "3D_Viewer_backend"
    PUBLIC ../../include/
)

find_library(MATH_LIBRARY m)

if(MATH_LIBRARY)
    target_link_libraries("3D_Viewer_backend" PUBLIC ${MATH_LIBRARY})
endif()

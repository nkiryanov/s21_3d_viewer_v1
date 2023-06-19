file(GLOB HEADER_LIST CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/include/frontend/*.hpp")

qt_add_library(
  "3D_Viewer_frontend" STATIC
  model_viewer.cpp
  object_gl_widget.cpp
  ui/model_viewer.ui
  resources.qrc
  ${HEADER_LIST}
)

target_include_directories(
    "3D_Viewer_frontend"
    PUBLIC ../../include/
    INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(
  "3D_Viewer_frontend"
  PUBLIC "3D_Viewer_backend" Qt6::Widgets Qt6::OpenGLWidgets
)

set(DOXYGEN_EXTRACT_ALL YES)
set(DOXYGEN_BUILTIN_STL_SUPPORT YES)

doxygen_add_docs(docs
  "${CMAKE_CURRENT_SOURCE_DIR}/main_page.md"
  backend/
  frontend/
  WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}/include"
)

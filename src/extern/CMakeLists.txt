include(AddGitSubmodule)

add_git_submodule("gif-h" "gif.h")

set(GIF_H_HEADERS gif-h/gif.h)

add_library(
    "gif-h" STATIC
    ${GIF_H_HEADERS}
)

target_include_directories(
    "gif-h"
    PUBLIC ./gif-h/
)

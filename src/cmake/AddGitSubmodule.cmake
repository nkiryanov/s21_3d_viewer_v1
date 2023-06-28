function(add_git_submodule SUBMODULE_NAME FILENAME_SUBMODULE_UPDATED)
  # Try to update the `SUBMODULE_NAME` in the directroy `extern/${SUBMODULE_NAME}`
  # Set `FILENAME_SUBMODULE_UPDATED` (relative, according to external repo) to verify the submodule exists and updated.
  
  find_package(Git QUIET)

  if(GIT_FOUND AND EXISTS "${PROJECT_SOURCE_DIR}/../.git")
    # Update submodules as needed
    option(GIT_SUBMODULE "Check submodules during build" ON)

    if(GIT_SUBMODULE)
      message(STATUS "Updating submodule ${SUBMODULE_NAME}")
      execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive "${PROJECT_SOURCE_DIR}/extern/${SUBMODULE_NAME}"
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        RESULT_VARIABLE GIT_SUBMOD_RESULT)

      if(NOT GIT_SUBMOD_RESULT EQUAL "0")
        message(FATAL_ERROR "git submodule update --init --recursive failed with ${GIT_SUBMOD_RESULT} for submodule '${SUBMODULE_NAME}', please checkout submodules")
      endif()
    endif()
  endif()

  if(NOT EXISTS "${PROJECT_SOURCE_DIR}/extern/${SUBMODULE_NAME}/${FILENAME_SUBMODULE_UPDATED}")
    message(FATAL_ERROR "The submodules were not downloaded! GIT_SUBMODULE was turned off or failed. Please update submodules and try again.")
  endif()
endfunction()

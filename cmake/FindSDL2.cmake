# Minimal SDL2 finder for environments without SDL2Config.cmake.
# Allows users to set SDL2_DIR, SDL2_INCLUDE_DIR, and SDL2_LIBRARY.

find_path(SDL2_INCLUDE_DIR
  NAMES SDL.h
  HINTS
    ENV SDL2_INCLUDE_DIR
    ${SDL2_DIR}
  PATH_SUFFIXES include/SDL2 include
)

find_library(SDL2_LIBRARY
  NAMES SDL2 SDL2main
  HINTS
    ENV SDL2_LIBRARY
    ${SDL2_DIR}
  PATH_SUFFIXES lib
)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(SDL2 DEFAULT_MSG SDL2_INCLUDE_DIR SDL2_LIBRARY)

if(SDL2_FOUND)
  if(NOT TARGET SDL2::SDL2)
    add_library(SDL2::SDL2 UNKNOWN IMPORTED)
    set_target_properties(SDL2::SDL2 PROPERTIES
      IMPORTED_LOCATION "${SDL2_LIBRARY}"
      INTERFACE_INCLUDE_DIRECTORIES "${SDL2_INCLUDE_DIR}"
    )
  endif()
endif()

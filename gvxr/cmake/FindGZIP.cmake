# - Find GZIP
# Find the native GZIP command
# This module defines
#  GZIP_TOOL, where to find gzip
#  GZIP_FOUND, If false, do not try to use GZIP.

SET(GZIP_NAMES ${GZIP_NAMES} gzip gzip.exe gzip124.exe)

find_program(GZIP_TOOL
  NAMES ${GZIP_NAMES}
  PATHS /bin /usr/bin /usr/local/bin ${PROJECT_BINARY_DIR}
)


# handle the QUIETLY and REQUIRED arguments and set GZIP_FOUND to TRUE if 
# all listed variables are TRUE

include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(GZIP REQUIRED_VARS GZIP_TOOL VERSION_VAR GZIP_VERSION)

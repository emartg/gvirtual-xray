set(CTEST_CUSTOM_MAXIMUM_NUMBER_OF_ERRORS           "200" )
set(CTEST_CUSTOM_MAXIMUM_NUMBER_OF_WARNINGS         "500" )
set(CTEST_CUSTOM_MAXIMUM_PASSED_TEST_OUTPUT_SIZE    "104857600") # 100 MB
set(CTEST_CUSTOM_COVERAGE_EXCLUDE                   "")

# either customize these directly or write as CMake Template
# and use configure_file(... @ONLY) with CMake
#set(CTEST_SOURCE_DIRECTORY   "/path/to/source")
#set(CTEST_BINARY_DIRECTORY   "/path/to/source")
# build options
set(OPTION_BUILD             "-j8")
# define generator (optional), e.g. default to 'Unix Makefiles' on UNIX, Visual Studio on Windows
set(CTEST_GENERATOR          "Unix Makefiles")
# submit under Continuous, Nightly (default), Experimental
set(CTEST_MODEL              "Continuous")
# define how to checkout code, e.g. copy a directory, git pull, svn co, etc.
set(CTEST_CHECKOUT_COMMAND   "svn co")
# define how to update (optional), e.g. git checkout <git-branch>
#set(CTEST_UPDATE_COMMAND     "...")
# define how to configure (e.g. cmake -DCMAKE_INSTALL_PREFIX=...)
set(CTEST_CONFIGURE_COMMAND  "cmake -DCMAKE_INSTALL_PREFIX=install")
# the name of the build
set(CTEST_BUILD_NAME         "Release")
# how to configure
#set(CTEST_CONFIGURE_COMMAND  "cmake")
# how to build
set(CTEST_BUILD_COMMAND      "make")
# default max time each tests can run (in seconds)
set(CTEST_TIMEOUT            "7200")
# locale to English
set(ENV{LC_MESSAGES}         "en_EN")

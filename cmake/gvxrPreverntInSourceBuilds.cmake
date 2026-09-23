#
# This function will prevent in-source builds
FUNCTION (AssureOutOfSourceBuilds)
    # make sure the user doesn't play dirty with symlinks
    get_filename_component(srcdir "${CMAKE_SOURCE_DIR}" REALPATH)
    get_filename_component(bindir "${CMAKE_BINARY_DIR}" REALPATH)

    # Get the dir lengths
    STRING (LENGTH ${srcdir} src_dir_size)
    STRING (LENGTH ${bindir} bin_dir_size)

    # Based on the dir lengths, it is possible that the bindir is the same as the srcdir,
    # or is a subdirectory of srcdir
    IF ( src_dir_size LESS_EQUAL bin_dir_size )

        # Get the first N characters of bindir, with N the size of srcdir
        string(SUBSTRING ${bindir} 0 ${src_dir_size} short_bin_dir)

        # bindir is within srcdir
        IF ("${srcdir}" STREQUAL "${short_bin_dir}")
            message("######################################################")
            message("# gVirtualXRay should not be configured & built in the source directory")
            message("# You must run cmake in a separate build directory.")
            message("######################################################")
            message(FATAL_ERROR "Quitting configuration")
        ENDIF ()
    ENDIF ()
ENDFUNCTION ()

AssureOutOfSourceBuilds()


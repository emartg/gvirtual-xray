ctest_read_custom_files(${CTEST_BINARY_DIRECTORY})

ctest_start             (${CTEST_MODEL} TRACK ${CTEST_MODEL})
ctest_configure         (BUILD ${CTEST_BINARY_DIRECTORY} RETURN_VALUE ret_con)
ctest_build             (BUILD ${CTEST_BINARY_DIRECTORY} RETURN_VALUE ret_bld)

if(ret_bld)
    # add as desired
    ctest_test              (BUILD ${CTEST_BINARY_DIRECTORY} RETURN_VALUE ret_tst)
    ctest_memcheck          (BUILD ${CTEST_BINARY_DIRECTORY} RETURN_VALUE ret_mem)
    ctest_coverage          (BUILD ${CTEST_BINARY_DIRECTORY} RETURN_VALUE ret_cov)

    # attach build notes if desired, e.g. performance info, output files from tests
    #list(APPEND CTEST_NOTES_FILES "/file/to/attach/as/build-note")
endif()

# standard submit
ctest_submit(RETURN_VALUE ret_sub)
# if dashboard requires a token that restricts who can submit to dashboard
ctest_submit(RETURN_VALUE ret_sub HTTPHEADER "Authorization: Bearer ${CTEST_TOKEN}")

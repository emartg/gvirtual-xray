# Module to give the tests the reference data of TestSinogramFromMesh
#   - The two files of the phantom (68 MB each) are not in the repository: they are in the archive
#     gvxr-test-data-1.zip, the asset of the release test-data-1 of https://github.com/emartg/gvirtual-xray
#   - The archive is downloaded once into the build folder, its SHA-256 is checked, and it is extracted there
#   - gVXR_TEST_DATA_ARCHIVE is the path of the archive: a machine without network access gives the path of a copy
#     downloaded by hand; when the file does not exist, it is downloaded to that path
#   - It sets gVXR_TEST_DATA_1_DIR, the folder of the extracted files
# The tests of gvxr and of SimpleGVXR both include it; the download and the extraction are done only once

SET (gVXR_TEST_DATA_1_NAME   "gvxr-test-data-1")
SET (gVXR_TEST_DATA_1_URL    "https://github.com/emartg/gvirtual-xray/releases/download/test-data-1/gvxr-test-data-1.zip")
SET (gVXR_TEST_DATA_1_SHA256 "fa0dcabf694c0ac3ddf323cf9033c78fedf11ade94d5ce4bbd5234cb6b04993f")

SET (gVXR_TEST_DATA_ARCHIVE "${CMAKE_BINARY_DIR}/test_data/${gVXR_TEST_DATA_1_NAME}.zip" CACHE FILEPATH
     "Archive of the reference data of the tests (the asset of the release test-data-1); downloaded when missing")

SET (gVXR_TEST_DATA_1_DIR "${CMAKE_BINARY_DIR}/test_data/${gVXR_TEST_DATA_1_NAME}")

# The stamp holds the SHA-256 of the archive that was extracted
SET (_gvxr_test_data_stamp "${CMAKE_BINARY_DIR}/test_data/${gVXR_TEST_DATA_1_NAME}.sha256")
SET (_gvxr_test_data_ready FALSE)

IF (EXISTS "${_gvxr_test_data_stamp}" AND EXISTS "${gVXR_TEST_DATA_1_DIR}/phantom_projections.raw")
    FILE (READ "${_gvxr_test_data_stamp}" _gvxr_test_data_extracted)
    STRING (STRIP "${_gvxr_test_data_extracted}" _gvxr_test_data_extracted)

    IF (_gvxr_test_data_extracted STREQUAL gVXR_TEST_DATA_1_SHA256)
        SET (_gvxr_test_data_ready TRUE)
    ENDIF ()
ENDIF ()

IF (NOT _gvxr_test_data_ready)

    # Download the archive when it is missing; the partial file has another name, so that an interrupted
    # download never leaves a file at the path of the archive
    IF (NOT EXISTS "${gVXR_TEST_DATA_ARCHIVE}")
        MESSAGE (STATUS "Download the test data from ${gVXR_TEST_DATA_1_URL}")

        FILE (DOWNLOAD "${gVXR_TEST_DATA_1_URL}" "${gVXR_TEST_DATA_ARCHIVE}.part"
              INACTIVITY_TIMEOUT 60
              STATUS _gvxr_test_data_status)

        LIST (GET _gvxr_test_data_status 0 _gvxr_test_data_code)
        LIST (GET _gvxr_test_data_status 1 _gvxr_test_data_message)

        IF (NOT _gvxr_test_data_code EQUAL 0)
            FILE (REMOVE "${gVXR_TEST_DATA_ARCHIVE}.part")
            MESSAGE (FATAL_ERROR
                     "The test data could not be downloaded from ${gVXR_TEST_DATA_1_URL} "
                     "(${_gvxr_test_data_message}). Download that file by hand and give its path with "
                     "-DgVXR_TEST_DATA_ARCHIVE=<path>, or configure with -DBUILD_TESTING=OFF.")
        ENDIF ()

        FILE (RENAME "${gVXR_TEST_DATA_ARCHIVE}.part" "${gVXR_TEST_DATA_ARCHIVE}")
    ENDIF ()

    # The SHA-256 is checked for a downloaded archive and for one given by hand
    FILE (SHA256 "${gVXR_TEST_DATA_ARCHIVE}" _gvxr_test_data_actual)

    IF (NOT _gvxr_test_data_actual STREQUAL gVXR_TEST_DATA_1_SHA256)
        MESSAGE (FATAL_ERROR
                 "${gVXR_TEST_DATA_ARCHIVE} has the SHA-256 ${_gvxr_test_data_actual}, "
                 "not ${gVXR_TEST_DATA_1_SHA256}: it is not the archive of the release test-data-1. "
                 "Delete it and configure again to download it, or give the right file with -DgVXR_TEST_DATA_ARCHIVE=<path>.")
    ENDIF ()

    FILE (REMOVE_RECURSE "${gVXR_TEST_DATA_1_DIR}")
    FILE (ARCHIVE_EXTRACT INPUT "${gVXR_TEST_DATA_ARCHIVE}" DESTINATION "${CMAKE_BINARY_DIR}/test_data")

    IF (NOT EXISTS "${gVXR_TEST_DATA_1_DIR}/phantom_projections.raw")
        MESSAGE (FATAL_ERROR "${gVXR_TEST_DATA_ARCHIVE} does not hold ${gVXR_TEST_DATA_1_NAME}/phantom_projections.raw.")
    ENDIF ()

    FILE (WRITE "${_gvxr_test_data_stamp}" "${gVXR_TEST_DATA_1_SHA256}\n")
    MESSAGE (STATUS "Test data extracted in ${gVXR_TEST_DATA_1_DIR}")
ENDIF ()

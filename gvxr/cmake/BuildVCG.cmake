macro(build_vcg staging_prefix build_prefix temp_path)


# make a custom VCG configuration file

SET (XCOM_VERSION_STRING 3.1)
SET (XCOM_VERSION_MAJOR  3.1)
SET (XCOM_VERSION_MINOR  0)

        ExternalProject_Add(vcg

          GIT_REPOSITORY "https://github.com/cnr-isti-vclab/vcglib.git"
          GIT_TAG master
          UPDATE_COMMAND ""
            SOURCE_DIR VCG
            CONFIGURE_COMMAND ""
            CMAKE_COMMAND ""
            BUILD_COMMAND ""
            INSTALL_COMMAND ""
        )

SET(VCG_PATH ${build_prefix}/VCG)
SET(VCG_INCLUDE_DIR ${VCG_PATH} )

# copy libraries into install tree
install(DIRECTORY ${VCG_PATH}     DESTINATION ${INSTALL_DIR}/third_party     COMPONENT Development)

endmacro(build_vcg)

# Module to check for Git LFS installation
#   - It finds the git-lfs executable and sets the GIT_LFS_EXECUTABLE variable
#   - Otherwise, it will raise a fatal error indicating that Git LFS is required

find_program(GIT_LFS_EXECUTABLE git-lfs)

if (NOT GIT_LFS_EXECUTABLE)
    message(FATAL_ERROR
            "Git LFS is required. Install it from https://git-lfs.com/")
endif ()

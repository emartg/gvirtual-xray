#!/usr/bin/env bash

xhost +

PS3="Which architecture do you want to use to build gVXR's new Python wheels?: "
options=("x86_64" "aarch64" "Quit")
select opt in "${options[@]}"
do
    case $opt in
        "x86_64")
            echo "you chose x86_64"
            ARCH=x86_64
            PLATFORM=linux/amd64
            break
            ;;
        "aarch64")
            echo "you chose aarch64"
            ARCH=aarch64
            PLATFORM=linux/arm64
            break
            ;;
        "Quit")
            echo "Abort"
            exit
            ;;
        *) echo "invalid option $REPLY";;
    esac
done

MANYLINUX=manylinux_2_34_$ARCH
#MANYLINUX=manylinux2014_$ARCH
echo $MANYLINUX

cd ..

PS3="Build a new Docker image (e.g. in case Manylinux has been updated)?: "
options=("Yes" "No" "Quit")
select opt in "${options[@]}"
do
    case $opt in
        "Yes")
            echo "you chose Yes"
            docker build --platform $PLATFORM --tag build-gvxr-$ARCH -f $PWD/pip/Dockerfile-$MANYLINUX .
            break
            ;;
        "No")
            echo "you chose No"
            break
            ;;
        "Quit")
            echo "Abort"
            exit
            ;;
        *) echo "invalid option $REPLY";;
    esac
done

if [ $? -eq 0 ]; then
    docker run \
        --platform $PLATFORM \
        --rm \
        --attach stdin \
        --attach stdout \
        --attach stderr \
        --ipc=host \
        --gpus all \
        -e NVIDIA_VISIBLE_DEVICES=all -e NVIDIA_DRIVER_CAPABILITIES=display,compute \
        -e EGL_PLATFORM="EGL_PLATFORM_GBM_MESA" \
        -e DISPLAY \
        -e QT_X11_NO_MITSHM \
        --privileged \
        -p 5000:5000 \
        -v ~/.Xauthority:/root/.Xauthority:rw \
        -v /tmp/.X11-unix:/tmp/.X11-unix:rw \
        -v $(pwd):/io \
        -w /io/pip \
        -it build-gvxr-$ARCH \
        /bin/bash

    if [ $? -ne 0 ]; then
        echo Could not run the Docker container. Try running it again, but without GPU support.

        docker run \
            --platform $PLATFORM \
            --rm \
            --attach stdin \
            --attach stdout \
            --attach stderr \
            --ipc=host \
            -e EGL_PLATFORM="EGL_PLATFORM_GBM_MESA" \
            -e DISPLAY \
            -e QT_X11_NO_MITSHM \
            --privileged \
            -p 5000:5000 \
            -v ~/.Xauthority:/root/.Xauthority:rw \
            -v /tmp/.X11-unix:/tmp/.X11-unix:rw \
            -v $(pwd):/io \
            -w /io/pip \
            -it build-gvxr-$ARCH \
            /bin/bash
    fi
else
    echo Could not build the Docker container.
fi


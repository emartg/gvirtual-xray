#!/usr/bin/env bash

if [[ ! -d  $(pwd)/output ]]; then
    mkdir  $(pwd)/output
fi

rm -rf $(pwd)/output/code
cp -r code $(pwd)/output

rm -rf $(pwd)/output/results
mkdir $(pwd)/output/results

docker build --platform linux/amd64 -t gvxr . \
\
&& \
\
docker run \
    --runtime nvidia \
    --privileged \
    --user $(id -u) \
    --group-add users \
    -p 8888:8888 \
    -v $(pwd)/output:/gvxr:rw \
    -v ~/.Xauthority:/root/.Xauthority:rw \
    -v /tmp/.X11-unix:/tmp/.X11-unix:rw \
    -e DISPLAY=$DISPLAY \
    -e NVIDIA_VISIBLE_DEVICES=all -e NVIDIA_DRIVER_CAPABILITIES=display,compute \
    -e EGL_PLATFORM="EGL_PLATFORM_GBM_MESA" \
    -e QT_X11_NO_MITSHM \
    -e WAYLAND_DISPLAY=$WAYLAND_DISPLAY \
    -e XDG_RUNTIME_DIR=$XDG_RUNTIME_DIR \
    --gpus all  \
    gvxr

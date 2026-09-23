#!/usr/bin/env bash


# Make sure the wheel exists
WHEEL_FILE=../pip/wheelhouse/repaired/gvxr-2.0.11-cp310-cp310-manylinux_2_34_aarch64.whl
if [[ ! -f $WHEEL_FILE ]];
then
  echo The wheel file does not exists.
  echo The script will exit.
  exit
fi

xhost +

cd ..

docker build --platform aarch64 --tag run-gvxr-jetson -f docker/Dockerfile-jetson .

OUTPUT_PATH="output-jetson"
if [[ ! -d $OUTPUT_PATH ]]; then
    mkdir  $OUTPUT_PATH
fi

sudo docker run \
    --platform aarch64 \
    -it \
    --rm \
    --runtime nvidia \
    --network=host \
    --ipc=host \
    --privileged \
    --gpus all -e NVIDIA_DRIVER_CAPABILITIES=all \
    --user $(id -u) \
    --group-add users \
    -p 8888:8888 \
    -v $OUTPUT_PATH:/gvxr:rw \
    -v ~/.Xauthority:/root/.Xauthority:rw \
    -v /tmp/.X11-unix:/tmp/.X11-unix:rw \
    -e DISPLAY=$DISPLAY \
    -e EGL_PLATFORM="EGL_PLATFORM_GBM_MESA" \
    -e QT_X11_NO_MITSHM \
    -e WAYLAND_DISPLAY=$WAYLAND_DISPLAY \
    -e XDG_RUNTIME_DIR=$XDG_RUNTIME_DIR \
    run-gvxr-jetson

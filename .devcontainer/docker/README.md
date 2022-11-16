#### Build this docker image

DOCKER_BUILDKIT=1 docker build -t stm32_dev -f .devcontainer/docker/Dockerfile .

#### Run container instance

docker run --rm -it stm32_dev

Recommended to use VSCode with `Dev Containers` installed.

#### X11 Server

Using JLink tools requires X11 Server installed on the host (it has pop-up window options)

This should be already installed on a Linux host.

For MacOS hosts you can install [XQuartz](https://www.xquartz.org/)

See instructions here - https://gist.github.com/sorny/969fe55d85c9b0035b0109a31cbcb088

/opt/JLink_Linux_V782b_x86_64/JLinkExe -NoGui 1
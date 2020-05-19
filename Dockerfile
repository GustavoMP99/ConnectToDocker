FROM amytabb/docker_ubuntu16_essentials:latest
COPY Code /Code
WORKDIR /Code/
RUN gcc -o Code main.c
CMD ["./Code"]
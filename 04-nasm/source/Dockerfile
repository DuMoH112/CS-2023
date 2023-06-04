# Use an existing base image as the starting point
FROM ubuntu:23.04
WORKDIR /source
COPY ./ /source/

# Update the package list and Install NASM and LD
RUN apt update && apt install -y nasm gcc-multilib gdb binutils

ENTRYPOINT ["sh", "-c", "/source/start.sh"]
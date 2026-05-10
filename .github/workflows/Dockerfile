FROM alpine:edge

# Install essential build tools, Clang (we target 21 via edge), CMake, Ninja, Py3/pipx (for Conan)
RUN apk update && apk add --no-cache \
    clang \
    compiler-rt \
    libc++-dev \
    libc++-static \
    py3-pip \
    pipx \
    bash \
    git \
    pkgconfig \
    build-base

# Install Conan using pipx (recommended for isolated environments)
RUN pipx install conan
ENV PATH="/root/.local/bin:${PATH}"

# Set Clang as the default compiler toolchain
ENV CC=clang
ENV CXX=clang++

WORKDIR /workspace

# Set default command, can be overridden when running the container
CMD ["/bin/bash", "-c", "./build.sh RELEASE"]

FROM alpine:edge

# Install tools
RUN apk update && apk add --no-cache \
    clang21 \
    clang21-extra-tools \
    compiler-rt \
    cmake \
    ninja \
    py3-pip \
    pipx \
    bash \
    git \
    build-base

# Protocol Handshake: Create symlinks for generic names
RUN ln -sf /usr/bin/clang-21 /usr/bin/clang && \
    ln -sf /usr/bin/clang++-21 /usr/bin/clang++

# Isolated Conan setup
RUN pipx install conan
ENV PATH="/root/.local/bin:${PATH}"

# Explicitly set the variables to the versioned names
ENV CC=clang-21
ENV CXX=clang++-21

WORKDIR /workspace

CMD ["/bin/bash", "-c", "./build.sh Testing"]
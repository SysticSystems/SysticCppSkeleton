FROM alpine:edge

# Install tools - removed alpine's ninja package
RUN apk update && apk add --no-cache \
    clang21 \
    clang21-extra-tools \
    compiler-rt \
    cmake \
    py3-pip \
    pipx \
    bash \
    git \
    build-base

# Protocol Handshake: Force system-wide recognition of clang-21
RUN ln -sf /usr/bin/clang-21 /usr/bin/clang && \
    ln -sf /usr/bin/clang++-21 /usr/bin/clang++ && \
    ln -sf /usr/bin/clang-21 /usr/bin/cc && \
    ln -sf /usr/bin/clang++-21 /usr/bin/c++ \
    ln -sf /usr/lib/llvm21/bin/clang-tidy /usr/bin/clang-tidy

# Isolated Conan & Modern Ninja setup
# Installing ninja via pipx guarantees version 1.11.x or 1.12.x
RUN pipx install conan && \
    pipx install ninja

ENV PATH="/root/.local/bin:${PATH}"

# Export variables for the build environment
ENV CC=clang
ENV CXX=clang++

# Init the folder where Conan 2 pulled deps lives.
RUN mkdir /root/.conan2
WORKDIR /workspace

CMD ["/bin/bash", "-c", "./build.sh Testing"]
FROM mcr.microsoft.com/devcontainers/base:ubuntu-24.04

# Toolchain para compilar pokefirered (igual que .github/workflows/build.yml)
RUN apt-get update \
    && DEBIAN_FRONTEND=noninteractive apt-get install -y --no-install-recommends \
        build-essential \
        binutils-arm-none-eabi \
        gcc-arm-none-eabi \
        libpng-dev \
        pkg-config \
    && rm -rf /var/lib/apt/lists/*

# agbcc (compilador original de GBA). Se compila en la imagen y setup.sh lo copia a tools/agbcc
RUN git clone --depth 1 https://github.com/pret/agbcc /opt/agbcc \
    && cd /opt/agbcc \
    && ./build.sh \
    && chmod -R a+rX /opt/agbcc

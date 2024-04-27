# Docker Structure and Purpose

This documentation covers the structure and purpose of the Docker environment for the project, detailing the builder images and their dependencies. Aimed at developers and future AI parsers, the structure is outlined to provide clarity on the build process and dependencies.

## Overview

The Docker environment comprises several builder images, each tailored for specific aspects of the project's build and deployment process. These images are organized to ensure a seamless and efficient pipeline, from compilation to deployment.

## Builders

### GCC Linux Builder (`builder-linux-gcc`)

- **Purpose**: Serves as the foundational GCC compiler environment for Linux builds.
- **Dependencies**: None.
- **Dockerfile**: `docker/Dockerfile-builder-linux-gcc`.
- **Build Args**:
  - GCC, CMake, and various libraries versions.
  - Utilizes `MAX_CPU_CORES` for optimized compilation.

### Clang Linux Builder (`builder-linux-clang`)

- **Purpose**: Provides a Clang compiler environment for Linux, building upon the GCC base.
- **Dependencies**: `builder-linux-gcc`.
- **Dockerfile**: `docker/Dockerfile-builder-linux-clang`.
- **Build Args**:
  - Inherits `CI_REGISTRY` and `CI_BUILDER_ROOT_IMAGE` for consistency.

### MinGW Linux Builder (`builder-linux-mingw`)

- **Purpose**: Facilitates cross-compilation for Windows using MinGW on Linux.
- **Dependencies**: None.
- **Dockerfile**: `docker/Dockerfile-builder-linux-mingw`.
- **Build Args**:
  - Includes Windows-specific dependencies and MinGW.

### Dev env (`dev-env`)

- **Purpose**: Supplements `builder-linux-clang` and `builder-windows-mingw` with additional development tools.
- **Dependencies**: `builder-linux-clang`, `builder-windows-mingw`.
- **Dockerfile**: `docker/Dockerfile-dev-env`.

### CI Linux Runner (`ci-linux-runner`)

- **Purpose**: Configured for CI tasks, including code analysis and automated testing.
- **Dependencies**: `builder-linux-clang`.
- **Dockerfile**: `docker/Dockerfile-ci-linux-runner`.
- **Build Args**:
  - Includes tools for static analysis and testing.

### CI Deploy (`ci-deploy`)

- **Purpose**: Dedicated to deployment tasks, packaging, and releases.
- **Dependencies**: None.
- **Dockerfile**: `docker/Dockerfile-ci-deploy`.

## Building and Deployment

A script `build_and_deploy_all.sh` automates the build and deployment process of all docker images in the correct order, respecting their dependencies. This ensures a streamlined CI/CD pipeline.

## Best Practices

- **Order of Operations**: Build and deploy images in dependency order to ensure base images are updated before dependent images.
- **Version Control**: Manage versions of all components carefully to maintain compatibility and reproducibility across environments.
- **Optimization**: Utilize `MAX_CPU_CORES` for parallel compilation where applicable to reduce build times.

This structure and workflow are designed to maintain a robust and efficient development environment, streamlining the process from development to deployment.

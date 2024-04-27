#!/bin/bash

# Build all docker images in the correct order and push them in gitlab registry

DOCKER_COMPOSE_DIR_PATH=$(readlink -f "$(dirname "$(dirname "${BASH_SOURCE}")")")
DOCKER_COMPOSE_FILE_PATH=${DOCKER_COMPOSE_DIR_PATH}/docker-compose.yml

echo "All docker builders images will be built and deployed based on ${DOCKER_COMPOSE_FILE_PATH}"

docker compose -f "${DOCKER_COMPOSE_FILE_PATH}" build builder-linux-gcc && docker compose push builder-linux-gcc &&
docker compose -f "${DOCKER_COMPOSE_FILE_PATH}" build builder-linux-clang && docker compose push builder-linux-clang &&
docker compose -f "${DOCKER_COMPOSE_FILE_PATH}" build builder-linux-mingw && docker compose push builder-linux-mingw &&
docker compose -f "${DOCKER_COMPOSE_FILE_PATH}" build dev-env && docker compose push dev-env &&
docker compose -f "${DOCKER_COMPOSE_FILE_PATH}" build ci-linux-runner && docker compose push ci-linux-runner &&
docker compose -f "${DOCKER_COMPOSE_FILE_PATH}" build ci-deploy && docker compose push ci-deploy &&
echo "All docker builders images built and deployed"

exit 0

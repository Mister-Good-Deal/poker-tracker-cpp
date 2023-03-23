#!/bin/bash

CURRENT_DIR_PATH=$(readlink -f "$(dirname "$0")")

for i in 1 2 3
do
  docker run -d --name gitlab-runner-code-quality-${i} --restart always \
    -v "${CURRENT_DIR_PATH}/config/code-quality-${i}:/etc/gitlab-runner" \
    -v /var/run/docker.sock:/var/run/docker.sock \
    gitlab/gitlab-runner:latest
done

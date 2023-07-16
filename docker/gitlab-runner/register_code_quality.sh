#!/bin/bash

CURRENT_DIR_PATH=$(readlink -f "$(dirname "$0")")

echo "Docker runners configuration are saved in ${CURRENT_DIR_PATH}/config"

# Register the runner for Code quality optimized settings
# @see https://docs.gitlab.com/ee/ci/testing/code_quality.html#improve-code-quality-performance-with-private-runners
for i in 1 2 3
do
  docker run --rm -it \
    -v "${CURRENT_DIR_PATH}/config/code-quality-${i}:/etc/gitlab-runner" \
    gitlab/gitlab-runner:latest register \
      --url "https://gitlab.laneuville.me/" \
      --executor "docker" \
      --docker-image docker:stable \
      --description "Code quality docker-runner ${i} - cq-sans-dind" \
      --maintenance-note "Code quality optimized (cq-sans-dind) docker-runner ${i} launched from docker host machine" \
      --tag-list "runner,cq-sans-dind,code-quality" \
      --clone-url "https://gitlab.laneuville.me/" \
      --access-level "not_protected" \
      --locked=false \
      --docker-volumes "/cache" \
      --docker-volumes "/var/run/docker.sock:/var/run/docker.sock" \
      --registration-token "bqbyyxhqo_WkNWxPHU-N" \
      --non-interactive
done

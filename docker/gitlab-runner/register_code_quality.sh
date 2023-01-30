#!/bin/bash

# Register the runner for Code quality optimized settings
# @see https://docs.gitlab.com/ee/ci/testing/code_quality.html#improve-code-quality-performance-with-private-runners
docker run --rm -it \
  -v /home/rom1/Projects/poker-bot/docker/gitlab-runner/config/code-quality:/etc/gitlab-runner \
  gitlab/gitlab-runner:latest register \
    --url "https://gitlab.laneuville.me/" \
    --executor "docker" \
    --docker-image docker:stable \
    --description "Code quality docker-runner - cq-sans-dind" \
    --maintenance-note "Code quality optimized (cq-sans-dind) docker-runner launched from docker host machine" \
    --tag-list "runner,cq-sans-dind,code-quality" \
    --clone-url "https://gitlab.laneuville.me/" \
    --access-level "not_protected" \
    --locked=false \
    --docker-volumes "/cache" \
    --docker-volumes "/var/run/docker.sock:/var/run/docker.sock" \
    --registration-token "bqbyyxhqo_WkNWxPHU-N" \
    --non-interactive

#!/bin/bash

# Register the runner
docker run --rm -it \
  -v /home/rom1/Projects/poker-bot/docker/gitlab-runner/config/host:/etc/gitlab-runner \
  gitlab/gitlab-runner:latest register \
    --url "https://gitlab.laneuville.me/" \
    --executor "docker" \
    --shell "bash" \
    --docker-image alpine:latest \
    --description "Host docker-runner" \
    --maintenance-note "Host docker-runner launched from docker host machine" \
    --tag-list "runner,host" \
    --clone-url "https://gitlab.laneuville.me/" \
    --docker-privileged=true \
    --access-level "not_protected" \
    --locked=false \
    --docker-volumes "/cache" \
    --docker-volumes "/var/run/docker.sock:/var/run/docker.sock" \
    --registration-token "bqbyyxhqo_WkNWxPHU-N" \
    --non-interactive
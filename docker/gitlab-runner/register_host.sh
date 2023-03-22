#!/bin/bash

# Register the runners
for i in 1 2 3
do
  docker run --rm -it \
    -v /home/rom1/Projects/poker-bot/docker/gitlab-runner/config/host-${i}:/etc/gitlab-runner \
    gitlab/gitlab-runner:latest register \
      --url "https://gitlab.laneuville.me/" \
      --executor "docker" \
      --shell "bash" \
      --docker-image alpine:latest \
      --description "Host docker-runner ${i}" \
      --maintenance-note "Host docker-runner ${i} launched from docker host machine" \
      --tag-list "runner,host" \
      --clone-url "https://gitlab.laneuville.me/" \
      --docker-privileged=true \
      --access-level "not_protected" \
      --locked=false \
      --docker-volumes "/cache" \
      --docker-volumes "/var/run/docker.sock:/var/run/docker.sock" \
      --registration-token "bqbyyxhqo_WkNWxPHU-N" \
      --non-interactive
done
#!/bin/bash

# Register the runner
docker run --rm -it \
  -v config:/etc/gitlab-runner \
  gitlab/gitlab-runner:latest register \
    --non-interactive \
    --url "https://gitlab.laneuville.me/" \
    --registration-token "bqbyyxhqo_WkNWxPHU-N" \
    --executor "docker" \
    --docker-image alpine:latest \
    --description "Host docker-runner" \
    --maintenance-note "Host docker-runner launched from docker host machine" \
    --tag-list "runner,host" \
    --clone-url "https://gitlab.laneuville.me/" \
    --docker-privileged=true \
    --locked=false \
    --access-level "not_protected"

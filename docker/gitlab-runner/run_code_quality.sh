#!/bin/bash

docker run -d --name gitlab-runner-code-quality --restart always \
  -v /home/rom1/Projects/poker-bot/docker/gitlab-runner/config/code-quality:/etc/gitlab-runner \
  -v /var/run/docker.sock:/var/run/docker.sock \
  gitlab/gitlab-runner:latest

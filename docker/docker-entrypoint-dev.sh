#!/bin/bash

# Start SSH server for IDE integration
if hash service 2>/dev/null; then
  # In ubuntu container
  service ssh start
else
  # In alpine container
  /usr/sbin/sshd -D
fi

# Launch GDB server session on a loop
#while true
#do
#  /usr/local/bin/gdbserver :1234 /app/build/run /app/resources/videos/720P.mp4
#done

tail -f /dev/null

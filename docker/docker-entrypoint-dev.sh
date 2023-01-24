#!/bin/bash

# Start SSH client for IDE integration
service ssh start

# Launch GDB server session on a loop
#while true
#do
#  /usr/local/bin/gdbserver :1234 /app/build/run /app/resources/videos/720P.mp4
#done

tail -f /dev/null

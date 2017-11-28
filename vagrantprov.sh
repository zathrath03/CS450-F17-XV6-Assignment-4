+#!/usr/bin/env bash
+
+apt-get update
+apt-get install -y qemu-system-x86
+apt-get install -y gdb
+apt-get install -y tmux
+
Add a comment to this line
+echo "set auto-load safe-path /" > /home/vagrant/.gdbinit
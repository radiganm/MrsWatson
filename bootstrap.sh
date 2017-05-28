#!/bin/sh
## boostrap.sh (for Mrs. Watson)
## Mac Radigan

  sudo apt-get install -y \
    libappindicator1      \
    libindicator7         \
    libaudiofile-dev      \
    libc6                 \
    libc6-dev
  git submodule init && git submodule update --recursive --remote
  cmake .
  make -j

## *EOF*

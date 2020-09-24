#!/bin/bash

echo "$@" >> ~/Software/cellID-linux/cell.log

# /usr/local/bin/cell "$@"
.~/Software/cellID-linux/cell "$@"

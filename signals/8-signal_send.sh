#!/bin/bash
if [ $# -ne 1 ]; then
	printf "Usage: %s <pid>\n" $0
	exit 1
else
	kill -s SIGQUIT $1
fi

#!/bin/sh

cut -d: -f3- err | sort | uniq | grep undefined | grep -v more\ und > undefs
wc -l undefs

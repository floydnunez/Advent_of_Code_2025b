#!/bin/bash
grep -Eo 'count: [0-9]+' yourfile.txt | awk '{s+=(($2>0)?$2-1:0)}END{print s}'

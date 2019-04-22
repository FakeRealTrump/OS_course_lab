#!/bin/bash

echo"testing para: ANNA_KARENINA.txt cat had her > result.txt"

echo"pipeTest"
time ./pipeTest.out ANNA_KARENINA.txt cat had her > result.txt

echo"socketTest"
time ./socketTest.out ANNA_KARENINA.txt cat had her > result.txt

echo"shmTest"
time ./shmTest.out ANNA_KARENINA.txt cat had her > result.txt
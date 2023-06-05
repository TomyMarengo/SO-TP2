#!/bin/bash
if [ "$1" == "buddy" ]; then
    MM="USE_BUDDY"
else
    MM=""
fi

docker run -d -v ${PWD}:/root --security-opt seccomp:unconfined -ti --name dockerSO agodio/itba-so:1.0
docker exec -it dockerSO make clean -C /root/Toolchain
docker exec -it dockerSO make all -C /root/Toolchain MM="$MM"
docker exec -it dockerSO make clean -C /root/
docker exec -it dockerSO make all -C /root/ MM="$MM"
docker stop dockerSO
docker rm dockerSO
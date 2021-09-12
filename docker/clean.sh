#!/usr/bin/env bash
containers=$(docker ps -f label=chicken.liberator -q)
images=$(docker images -f label=chicken.liberator -q)

docker stop $containers || true
docker rm -f $containers || true
docker rmi -f $images || true
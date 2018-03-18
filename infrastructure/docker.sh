#!/usr/bin/env sh

MONGODB_CONTAINER_NAME="low_mongodb"
COMMAND=""
DOCKER_IMAGE=""

SCRIPT_FOLDER="$(dirname $(realpath "${0}"))"

help() {
    cat << EOF
usage: $(basename ${0}) [build IMG | start IMG | stop IMG] [--help | -h]

  build IMG      Build the docker image IMG
  start IMG      Start a docker container from the image IMG
  stop IMG       Stop the docker container which was started from the image IMG
  --help | -h    Prints this dialog

Supported images (IMG) are:
  mongodb

EOF
}

stop_mongodb() {
    sudo docker container stop "${MONGODB_CONTAINER_NAME}"
    sudo docker container rm "${MONGODB_CONTAINER_NAME}"
}

build_mongodb() {
    sudo docker build -t low_mongodb "${SCRIPT_FOLDER}/mongodb/"
}

run_mongodb() {
    sudo docker run --name low_mongodb --detach --publish 27017:27017 low_mongodb
}

main() {
    for arg in "$@"
    do
        if [ "${arg}" = "--help" ] || [ "${arg}" = "-h" ]
        then
            help
            exit 0
        elif [ "${arg}" = "build" ] || [ "${arg}" = "run" ] || [ "${arg}" = "stop" ]
        then
            COMMAND="$arg"
        elif [ "${COMMAND}" != "" ] && [ "${arg}" = "mongodb" ]
        then
            DOCKER_IMAGE="${arg}"
        else
            echo "The option \"${arg}\" is not supported" >&2
            help >&2
            exit 1
        fi
    done

    "${COMMAND}_${DOCKER_IMAGE}"
}

main "$@"

#!/usr/bin/env sh

MONGODB_CONTAINER_NAME="low_mongodb"
HTTP_CONTAINER_NAME="low_http"
COMMAND=""
DOCKER_IMAGE=""

SCRIPT_FOLDER="$(dirname $(realpath "${0}"))"
LOW_ROOT="${SCRIPT_FOLDER}/../"

help() {
    cat << EOF
usage: $(basename ${0}) [build IMG | start IMG | stop IMG] [--help | -h]

  build IMG      Build the docker image IMG
  start IMG      Start a docker container from the image IMG
  stop IMG       Stop the docker container which was started from the image IMG
  --help | -h    Prints this dialog

Supported images (IMG) are:
  mongodb (contains the mongodb service for this application)
  http (build and run the HTTP component)

EOF
}

stop_mongodb() {
    sudo docker container stop "${MONGODB_CONTAINER_NAME}"
    sudo docker container rm "${MONGODB_CONTAINER_NAME}"
}

build_mongodb() {
    sudo docker build -t "${MONGODB_CONTAINER_NAME}" "${SCRIPT_FOLDER}/mongodb/"
}

start_mongodb() {
    sudo docker run --name "${MONGODB_CONTAINER_NAME}" --detach --publish 27017:27017 low_mongodb
}

stop_http() {
    sudo docker container stop "${HTTP_CONTAINER_NAME}"
    sudo docker container rm "${HTTP_CONTAINER_NAME}"
}

build_http() {
    sudo docker build -t "${HTTP_CONTAINER_NAME}" "${SCRIPT_FOLDER}/http/"
}

start_http() {
    sudo docker run --name "${HTTP_CONTAINER_NAME}" --tty --interactive \
         --mount type=bind,source="${LOW_ROOT}",target=/project/low \
         "${HTTP_CONTAINER_NAME}"
}

main() {
    for arg in "$@"
    do
        if [ "${arg}" = "--help" ] || [ "${arg}" = "-h" ]
        then
            help
            exit 0
        elif [ "${COMMAND}" = "" ] && ([ "${arg}" = "build" ] || [ "${arg}" = "start" ] || \
                                           [ "${arg}" = "stop" ])
        then
            COMMAND="$arg"
        elif [ "${COMMAND}" != "" ] && ([ "${arg}" = "mongodb" ] || [ "${arg}" = "http" ] )
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

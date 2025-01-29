#! /bin/bash

if [ -f .env ]; then
    source .env
fi

version="v0.0.1"
image_name="go-api-pl-project"
docker_namspace=$DOCKER_NAMESPACE
push=false

while [ -n "$1" ]; do
    case "$1" in
    -p | --push)
        case "$2" in
            t | true)
                push=true
                ;;
            f | false)
                push=false
                ;;
            *)
                echo "E: please set the push state with [-p | --push] [[t]rue | [f]alse]"
                exit 1
                ;;
        esac
        shift
        ;;
    *)
        echo "E: Unknown flag"
        exit 1
    esac
    shift
done

mkdir -p ./shared/config
cp -r ../shared/config ./shared

docker build -t $image_name:$version .

rm -r ./shared

if [ "$push" == "true" ]; then
    docker image tag $image_name:$version ghcr.io/$docker_namspace/$image_name:$version
    docker push ghcr.io/$docker_namspace/$image_name:$version
fi


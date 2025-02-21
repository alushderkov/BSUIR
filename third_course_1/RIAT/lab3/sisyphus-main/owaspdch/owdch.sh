#!/bin/sh
#после команды owdch.sh требуется указать название каталога отчета и путь к каталогу с файлами 
DC_VERSION="latest"
REPORT_NAME=$1
DC_DIRECTORY=$2
DC_PROJECT="dependency-check scan: $(pwd)"
DATA_DIRECTORY="$(pwd)/data"
CACHE_DIRECTORY="$(pwd)/data/cache"

if [ ! -d "$DATA_DIRECTORY" ]; then
    echo "Initially creating persistent directory: $DATA_DIRECTORY"
    mkdir -p "$DATA_DIRECTORY"
fi
if [ ! -d "$CACHE_DIRECTORY" ]; then
    echo "Initially creating persistent directory: $CACHE_DIRECTORY"
    mkdir -p "$CACHE_DIRECTORY"
fi

docker pull owasp/dependency-check:$DC_VERSION

docker run --rm \
    -e user=$USER \
    -u $(id -u ${USER}):$(id -g ${USER}) \
    --volume $DC_DIRECTORY:/src:z \
    --volume "$DATA_DIRECTORY":/usr/share/dependency-check/data:z \
    --volume $(pwd)/$REPORT_NAME:/report:z \
    owasp/dependency-check:$DC_VERSION \
    --scan /src \
    --format "JSON" \
    --format "HTML" \
    --project "$DC_PROJECT" \
    --out /report
    # Use suppression like this: (where /src == $pwd)
    # --suppression "/src/security/dependency-check-suppression.xml"

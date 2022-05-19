#!/bin/bash

BASE_DIR=$(cd "$(dirname "$0")"; pwd)/
VERSION="1.0.0"

DEBIAN_STAGING="${BASE_DIR}/debian-staging"
SOURCE="${BASE_DIR}/source"

rm -rf "${DEBIAN_STAGING}"

mkdir -p "${DEBIAN_STAGING}"

cp -r "${SOURCE}" "${DEBIAN_STAGING}/kieker-lang-pack-c"

cd "${DEBIAN_STAGE}"

tar -cvzpf "kieker-lang-pack-c_${VERSION}.orig.tgz" kieker-lang-pack-c

# end
 

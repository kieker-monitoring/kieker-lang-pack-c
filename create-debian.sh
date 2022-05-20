#!/bin/bash

BASE_DIR=$(cd "$(dirname "$0")"; pwd)/
VERSION="1.0.0"

DEBIAN_STAGING="${BASE_DIR}/debian-staging"
DEBIAN_STAGING_SOURCE="${DEBIAN_STAGING}/kieker-lang-pack-c"
SOURCE="${BASE_DIR}/source"

# prepare staging
echo "Prepare staging"

rm -rf "${DEBIAN_STAGING}"

mkdir -p "${DEBIAN_STAGING}"

# copy data
echo "Copy data"

cp -r "${SOURCE}" "${DEBIAN_STAGING_SOURCE}"
rm -f "${DEBIAN_STAGING_SOURCE}//README"
cp -f "${BASE_DIR}/README.md" "${DEBIAN_STAGING_SOURCE}/README"

# build

cd "${DEBIAN_STAGING_SOURCE}"

libtoolize
aclocal
autoconf
automake --add-missing

# create
echo "Create archive"

cd "${DEBIAN_STAGING}"

tar -czpf "kieker-lang-pack-c_${VERSION}.orig.tar.gz" kieker-lang-pack-c

# Setup debian
echo "Stetup Debian"

cp -r "${BASE_DIR}/debian" "${DEBIAN_STAGING_SOURCE}"

cd "${DEBIAN_STAGING_SOURCE}"

debuild -us -uc

# end


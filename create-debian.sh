#!/bin/bash

BASE_DIR=$(cd "$(dirname "$0")"; pwd)/
VERSION="1.0.0"

DEBIAN_STAGING="${BASE_DIR}/debian-staging"
DEBIAN_STAGING_SOURCE="${DEBIAN_STAGING}/kieker-lang-pack-c"
SOURCE="${BASE_DIR}/source"

# prepare staging
echo "Prepare staging"

# cd "${SOURCE}"

# make clean ; git clean -f

rm -rf "${DEBIAN_STAGING}"
rm -rf "${SOURCE}/m4"

mkdir -p "${DEBIAN_STAGING}"

# copy data
echo "Copy data"

cp -r "${SOURCE}" "${DEBIAN_STAGING_SOURCE}"
rm -f "${DEBIAN_STAGING_SOURCE}/README"
cp -f "${BASE_DIR}/README.md" "${DEBIAN_STAGING_SOURCE}/README"

# build
echo "Build"
cd "${DEBIAN_STAGING_SOURCE}"

libtoolize
aclocal
autoconf
automake --add-missing
./configure

# create
echo "Create archive"

cd "${DEBIAN_STAGING}"

find "${DEBIAN_STAGING}" -name '.libs' -exec rm -rf {} \;
find "${DEBIAN_STAGING}" -name '.deps' -exec rm -rf {} \;
find "${DEBIAN_STAGING}" -name '*.o' -exec rm {} \;
find "${DEBIAN_STAGING}" -name '*.lo' -exec rm {} \;
find "${DEBIAN_STAGING}" -name '*.la' -exec rm {} \;
rm -rf "${DEBIAN_STAGING_SOURCE}/autom4te.cache" "${DEBIAN_STAGING_SOURCE}/m4" "${DEBIAN_STAGING_SOURCE}/autotools"

tar -czpf "kieker-lang-pack-c_${VERSION}.orig.tar.gz" kieker-lang-pack-c

# Setup debian
echo "Setup Debian"

cp -r "${BASE_DIR}/debian" "${DEBIAN_STAGING_SOURCE}"

cd "${DEBIAN_STAGING_SOURCE}"

debuild -us -uc

# end

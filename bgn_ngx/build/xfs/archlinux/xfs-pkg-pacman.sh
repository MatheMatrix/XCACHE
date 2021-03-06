#!/bin/bash -x

fs_type=${1:-xfs}
version=${2:-5.7.5.0}
pkg_name=${fs_type}-${version}
pkg_src=${3:-${pkg_name}}

need_confirm=${4:-yes}


echo -e "\nCommand: \n\t\033[0;34m$0 <xfs> <version> <src path>\033[0m\t\tdefault version is ${version}"

if [ "$need_confirm" == "yes" ]; then
    echo -ne "\nBuild \033[0;32m${pkg_name} from ${pkg_src}\033[0m (y/N) "
    read answer
    
    if [ "$answer" != "y" ]; then
        exit -1;
    fi
fi

CUR_DIR=$(pwd)
BUILD_DIR=${CUR_DIR}/pkgbuild
DEPEND_DIR=/usr/local/${fs_type}/depend

BUILDROOT=${BUILD_DIR}

# remove old packages
rm -f xfs-*.pkg.tar.xz

rm -rf ${BUILD_DIR}
mkdir -vp ${BUILD_DIR}/{src,pkg}

# prepare makefile
rm -f ${CUR_DIR}/${fs_type}.PKGBUILD
cp -af ${pkg_src}/bin/${fs_type}.PKGBUILD 		${CUR_DIR}/${fs_type}.PKGBUILD
cp -af ${pkg_src}/bin/${fs_type}.pkg.install 	${CUR_DIR}/${fs_type}.pkg.install
cp -af ${pkg_src}/bin/${fs_type}.pkg.service    ${CUR_DIR}/${fs_type}.pkg.service

mkdir -vp $BUILDROOT/${fs_type}-${version}
cp -af ${CUR_DIR}/${pkg_src}/*      			$BUILDROOT/${fs_type}-${version}
cp -af ${CUR_DIR}/${fs_type}.PKGBUILD   		$BUILDROOT/PKGBUILD
cp -af ${CUR_DIR}/${fs_type}.pkg.install 		$BUILDROOT/${fs_type}.pkg.install
cp -af ${CUR_DIR}/${fs_type}.pkg.service 		$BUILDROOT/${fs_type}.pkg.service
rm -f $BUILDROOT/${fs_type}-${version}/bin/${fs_type}_init.sh
mv  $BUILDROOT/${fs_type}-${version}/bin/${fs_type}.pkg.init.sh $BUILDROOT/${fs_type}-${version}/bin/${fs_type}_init.sh

# replace files
mkdir -vp $BUILDROOT/${DEPEND_DIR}
cp -af ${DEPEND_DIR}/* $BUILDROOT/${DEPEND_DIR}  || exit 1

sed -i "s@^pkgver=.*@pkgver=$version@" $BUILDROOT/PKGBUILD

cd $BUILDROOT
makepkg --noconfirm -C -f -s -L && mv $BUILDROOT/*.pkg.tar.xz $CUR_DIR

# clean up
rm -rf ${BUILD_DIR}
rm -rf ${CUR_DIR}/${fs_type}.PKGBUILD
rm -rf ${CUR_DIR}/${fs_type}.pkg.install
rm -rf ${CUR_DIR}/${fs_type}.pkg.service

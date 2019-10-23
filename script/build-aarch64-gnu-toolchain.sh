#!/bin/bash

top_dir=`pwd`
tools_dir=/tmp
cd ${build_dir}

# check free space for building the toolchain
free_space=`df . | awk '{print $4}' | tail -n 1`

if [ ${free_space} -lt 524288 ]
then
	echo "not enough space under /tmp for build AArch64 GNU toolchain"
	exit 1
fi

# install path
install_dir=${top_dir}/install
if [ ! -d ${install_dir} ]
then
	mkdir ${install_dir}
fi

# src path
src_dir=${tools_dir}/aarch64-gnu-toolchain-src
if [ ! -d ${src_dir} ]
then
	mkdir ${src_dir}
	cd ${src_dir}
	wget https://ftp.gnu.org/gnu/gcc/gcc-9.1.0/gcc-9.1.0.tar.xz
	wget https://ftp.gnu.org/gnu/binutils/binutils-2.32.tar.bz2
	tar xvf gcc-9.1.0.tar.xz
	tar xvf binutils-2.32.tar.bz2

	# download gcc prerequisite
	cd gcc-9.1.0
	./contrib/download_prerequisites
fi

# build path
build_dir=${tools_dir}/aarch64-gnu-toolchain-build
if [ ! -d ${build_dir} ]
then
	mkdir ${build_dir}
	cd ${build_dir}
	mkdir build-binutils
	mkdir build-gcc
	cd ${build_dir}/build-binutils
	${src_dir}/binutils-2.32/configure --prefix=${install_dir} --target=aarch64-linux --disable-gold --enable-plugins
	cd ${build_dir}/build-gcc
	${src_dir}/gcc-9.1.0/configure --prefix=${install_dir} --enable-languages=c --target=aarch64-linux --disable-libssp
fi

cd ${build_dir}/build-binutils
make all -j10; make install

cd ${build_dir}/build-gcc
make all -j10; make install

# Do NOT delete the source and build files, so the next the build could be
# quicker.
#rm -rf ${src_dir}
#rm -rf ${build_dir}

# return and report
cd ${top_dir}
echo ""
echo "=============================================================================="
echo "Finished buliding AArch64 GNU toolchain (no lib) under ${top_dir}/install/"
echo "You may have seen some build errors, but those are fine, the toolchain components needed by us should have been generated"
echo "Suggest adding ${top_dir}/install/bin to your \$PATH"
echo "=============================================================================="

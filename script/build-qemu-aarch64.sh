#!/bin/bash

top_dir=`pwd`
tools_dir=/tmp
cd ${build_dir}

# check free space for building qemu
free_space=`df . | awk '{print $4}' | tail -n 1`

if [ ${free_space} -lt 524288 ]
then
	echo "not enough space under /tmp for build qemu"
	exit 1
fi

# install path
install_dir=${top_dir}/install
if [ ! -d ${install_dir} ]
then
	mkdir ${install_dir}
fi

# src path
src_dir=${tools_dir}/qemu-src
if [ ! -d ${src_dir} ]
then
	mkdir ${src_dir}
	cd ${src_dir}
	wget https://download.qemu.org/qemu-4.1.0.tar.bz2
	tar xvf qemu-4.1.0.tar.bz2
fi

# build path (we care about aarch64 only at the moment)
build_dir=${tools_dir}/qemu-build
if [ ! -d ${build_dir} ]
then
	mkdir -p ${build_dir}/qemu-aarch64-build
        cd ${build_dir}/qemu-aarch64-build
	${src_dir}/qemu-4.1.0/configure --prefix=${install_dir} --target-list=aarch64-linux-user
fi

cd ${build_dir}/qemu-aarch64-build
make all -j10; make install

# Do NOT delete the source and build files, so the next the build could be
# quicker.
#rm -rf ${src_dir}
#rm -rf ${build_dir}

# return and report
cd ${top_dir}
echo ""
echo "=============================================================================="
echo "Finished buliding AArch64 qemu as ${top_dir}/install/bin/qemu-aarch64"
echo "Suggest adding ${top_dir}/install/bin to your \$PATH"
echo "=============================================================================="

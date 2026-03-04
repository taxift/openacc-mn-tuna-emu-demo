#!/bin/sh

# 任意のパスに書き換えて良い
INSTALL_DIR=${PWD}
echo ${INSTALL_DIR}

git clone -b mnacc https://github.com/taxift/xcodeml-tools.git

# 以下もダウンロードする
wget https://github.com/taxift/omni-compiler/raw/8a27425e464a06accc905dbe789d67266f28e1c4/Driver/src/om_replace_pragma.c

# libxml2がない場合はインストールする（なんかaptで入れてもヘッダーが見つからないことがある）
# libxml2が要求するglibcのバージョンがOSにあるものより新しい場合は、古いバージョンのlibxml2をインストールする
wget ftp://xmlsoft.org/libxml2/libxml2-2.9.9.tar.gz
tar xvfz libxml2-2.9.9.tar.gz
cd libxml2-2.9.9
# ※Pythonのあたりでコケるがxcodeml-toolsのインストールには関係ないので無視
	# なお、prefixの場所はどこでも良い
./configure --prefix=${INSTALL_DIR}/libxml2/2_9_9; make; make install
cd ..

# aptになっているが、適宜dnfやyumに書き換えること
# bisonもインストール（byaccはコケるからダメ）
sudo apt update
sudo apt install -y bison
# 以下もインストールする
sudo apt install -y flex
# Javaも
sudo apt install -y default-jre
sudo apt install -y default-jdk

cd xcodeml-tools
./configure --prefix=${INSTALL_DIR}/XcodeML-Tools --with-libxml2=${INSTALL_DIR}/libxml2/2_9_9
make; make install
cd ..

# 以下も必要なので、コンパイルする（Omni本体のファイル）
# 	プリプロセッサでpragma内部のマクロの置き換えを行うために必要（Cのみ）
gcc om_replace_pragma.c -o ${INSTALL_DIR}/XcodeML-Tools/bin/om_replace_pragma

# エミュレータを実行して、結果を見やすい形でファイル出力

_FILENAME=${1%.*}

gpfn3_package_main -i ${_FILENAME}.asm -d ${_FILENAME}.dmp

BUILD_DIR=$(cd $(dirname $0); pwd)
python3 ${BUILD_DIR}/emu_asm_run.py ${_FILENAME}.dmp
# 追加のREADME

## 公開バージョン
beta2026.03.1

### 動作確認環境
Ubuntu 20.04、22.04

## 依存関係パッケージのインストール
PyArmorを入れる。  
なお、リポジトリに含まれているファイルはPython3.9で作成。
```
$ pip3 install pyarmor
```
Ubuntu 22.04にPython3.9を入れて実行するにはひと手間必要だが、aptで簡単に導入するには以下のようにする。
```
$ sudo apt install -y software-properties-common
$ sudo add-apt-repository ppa:deadsnakes/ppa
$ sudo apt install -y python3.9
$ sudo apt install -y python3.9-distutils
$ python3.9 -m pip install lxml pyarmor
```
これに加えて、適宜mnaccの中身のpython3コマンドをpython3.9コマンドに変更したりする。
# OpenACC for MN-Coreの仕様書
doc/を参照。

# 現在OpenACC/MNコンパイラでできること
- OpenACCをMNCLへ変換
- OpenACCを直接アセンブリへ変換

どちらも制約があるが、それらはREADME.mdや以下の説明を参照。

# OpenACCから直接アセンブリ生成
## MN-Core2エミュレータ環境構築
以下のページにあるエミュレータをダウンロードする。
https://projects.preferred.jp/mn-core/#resources
```
wget https://projects.preferred.jp/mn-core/assets/mncore2_emuenv_20240826.tar.xz
```
ファイルを解凍し、適宜中の資料を参照してインストールする。

なお、変換後のアセンブリを理解するには以下のマニュアルの解読が必要。
https://projects.preferred.jp/mn-core/assets/mncore2_dev_manual_ja.pdf


## コマンド実行方法
doc/の「エミュレータ実行環境説明資料」を参照。  
OpenACCから直接アセンブリへ変換してエミュレータ上で実行する流れを記載している。

変換可能なコードの制約や生成されるファイル群についても記載。
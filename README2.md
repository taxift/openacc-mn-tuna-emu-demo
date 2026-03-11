# OpenACC for MN-Coreコンパイラ

## これは何？

OpenACC/MN-Coreの言語処理系のプロトタイプ実装。  
OpenACCのファイルをMNCL（MN-Core用のOpenCL相当の言語）へ変換する。

ゆくゆくはMNCLからバイナリ変換するところまで呼び出して、一発変換できるようにする。

## ファイル説明
- test/
    - テスト用のソースコードが入っているディレクトリ
- build-xcodeml-tools.sh
    - ソースファイル <-> XcodeML（中間コード表現形式）相互変換のためのツールをインストールするためのシェルスクリプト（適宜書き換えて使用）
- mnacc
    - コンパイラ実行コマンド（C言語版OpenACC -> MNCLコンパイルを行うシェルスクリプト）
- ~~mnacf~~
    - ~~コンパイラ実行コマンド（Fortran版OpenACC -> MNCLコンパイルを行うシェルスクリプト、未実装）~~
- translator_C_to_MNCL.py
    - コンパイラミドルウェア本体（C言語用）：XcodeML -> MNCL（ホストコード＋デバイスコード）
- ~~translator_F_to_MNCL.py~~
    - ~~コンパイラミドルウェア本体（Fortran用）：XcodeML -> MNCL（ホストコード＋デバイスコード）~~
- MN_acc.h
    - MNCLホストコードで使われるランタイム関数の宣言ヘッダーファイル

# インストール
動作環境はgccがインストール済みのLinuxを想定している。

まず、XcodeML-Tools関連のプログラムビルドスクリプトbuild-xcodeml-tools.shを参照する。  
このシェルスクリプトはDebian系Linux向けに書かれているため、もしDebian系Linuxではない場合は適宜`dnf`などにコマンドを修正すること。  

git cloneしてできたディレクトリにインストールして良い場合は何も考えずに
```
$ ./build-xcodeml-tools.sh
```
を実行して、XcodeML-Tools関連のプログラムをインストールする。

なお、必要に応じて、INSTALL_DIRやprefixのパスを任意のパスに書き換えて実行して良い。  
また、もし既にインストール済みのパッケージがあれば適宜コメントアウトして良い。

次に、XML解析用Pythonライブラリlxmlもインストールする。なお、2025年1月現在、最新版のlxmlにバグが発生している（将来的に修正されるというメッセージが出てくる）ため、バージョン5.1.0で動作確認している。
```
$ pip install lxml
```

# 使い方

まず、環境変数のパスを通す。  
デフォルトのディレクトリ構成でインストールした場合、mnaccの存在するディレクトリに移動して以下を行う。
```
$ echo "export PATH=$PATH:${PWD}:${PWD}/XcodeML-Tools/bin" >> ~/.bash_profile
$ source ~/.bash_profile
```

git cloneしたときに展開されるデフォルト配置からファイルを移動した場合は、mnacc、mnacfの中身に書かれているBUILD_DIRパスも適宜変更する。

## C言語
```
$ mnacc <file_name>.c
```

のようにすると、

- <file_name>_host.c（MNCLホストコード）
- <file_name>_device.cl（MNCLデバイスコード）

の2つのMNCLファイルが生成される。

## Frotran

まだできてないが、Cと同様、以下のように使う想定。
```
$ mnacf <file_name>.f90
```
生成されるファイル名はCと同じ予定（ホストコードはFortran?）。

# 既知のバグ、できないことなど

- 現時点での実装上の記述制約については[こちらのファイル](./current_restrictions.md)も参照

- あまり心配無さそうだが、デバイス上でポインタの”アドレス”の書き換えはできない（中身の実体の書き換えは可）

- ループ変数以外の変数名の被り処理には非対応（おかしなコードが吐かれる可能性があるので、必ず一意の変数名にする）

- ホストコードの関数はまだダミー

- 現状はまだプロトタイプであり、エラー処理等、まだ抜けがある（ご意見はお手柔らかに）

# 質問等
開発者の綱島＠神戸大まで

tsunashima あっと gold.kobe-u.ac.jp  
（所属フリーな転送アドレス：ryuta.tsunashima あっと alum.riken.jp）


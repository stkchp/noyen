# noyen(no ¥)

## 詳細

Remote Desktop for Macにて、日本語キーボードの円(¥)キーを押した際、
ASCIIのバックスラッシュにあたる`0x5C`ではなく、Unicodeの0xC2A5が
入力されてしまうのをwindows側で回避するプログラム。常駐型。

## ダウンロード

[Releases](releases)からダウンロードしてください。


## 使い方

### 開始

`noyen.exe`を起動する。


### 終了

タスクトレイにあるアイコンをクリックすると出てくる`exit`を選ぶ。


## ライセンス

- CC0

[![CC0](http://i.creativecommons.org/p/zero/1.0/88x31.png "CC0")](http://creativecommons.org/publicdomain/zero/1.0/deed.ja)


## なぜ管理者権限が必要なのか

管理者権限で開かないと別権限で開いているアプリケーションに対してhookが効かないので。



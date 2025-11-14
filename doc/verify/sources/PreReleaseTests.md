# ThinBridge リリース前検証手順

## 検証環境の用意

* Windows 11
* Google Chrome、Microsoft Edgeをインストール済みである。

準備は以下の手順で行う。

1. PrintCancelerの最新のインストーラ `PrintCanceler.exe` をダウンロードし、実行、インストールする。
2. ユーザー認証が必要なプロキシサーバーを作成するため、[Squid](https://squid.diladele.com/)のWindows版をダウンロード、インストールする
3. [doc\verify\sources\TestTools/.passwd](../TestTools/.passwd)を`C:\Squid`に配置する
4. [doc\verify\sources\TestTools/squid-configure.conf](../TestTools/squid-configure.conf)を`C:\Squid\etc\squid\squid.conf`にこの名前で配置（上書き）する。
5. Windows版の現在の最新のSquid 4.14ではBASIC認証に必要な`cygcrypto-2.dll`が含まれていないため、Cygwinからダウンロードして配置する。
  5-1. [Cygwin](https://www.cygwin.com/install.html)のインストーラーをダウンロードする
  5.2. Select Packages画面でlibcrypt2を選択してインストールする
  5.3. インストールが完了したら`C:\cygwin64\bin\cygcrypt-2.dll`を`C:\Squid\lib\squid`にコピーする
6. SquidのトレイからSquidを再起動する



## 検証

### ユーザー名・パスワードが正しい場合

#### 準備

以下の通り設定して検証を行う。

* [doc\verify\sources\TestTools/Scenarios/scenario1.ini](../TestTools/Scenarios/scenario1.ini) を `C:\Program Files\PrintCanceler\PrintCanceler.ini` に配置する。
* 念のためEdgeを再起動する

#### 検証

##### ダイアログを自動でキャンセルするケース

* `powershell doc\verify\sources\TestTools\http_server.ps1`でローカルのWebサーバーを起動する
  * `http://localhost:8080`で簡易Webサーバーが起動する
* `doc\verify\sources\TestTools\form.html` を開く
* フォームに「test」と入力して、「送信」ボタンを押す
* 遷移先の http://localhost:8080 をリロードする
  * [ ] 「フォームを再送信しますか？」ダイアログ一瞬表示され、キャンセルされること
* この状態で、2分程待機する（時間経過で拡張機能が停止しないことの確認）
* http://localhost:8080 をリロードする
  * [ ] 「フォームを再送信しますか？」ダイアログ一瞬表示され、キャンセルされること

##### ERR_CACHE_MISSページが表示されるケース

* `powershell doc\verify\sources\TestTools\http_server.ps1`でローカルのWebサーバーを起動する
  * `http://localhost:8080`で簡易Webサーバーが起動する
* `doc\verify\sources\TestTools\form.html` を開く
* 「フォームをダイアログで開くボタン」を押す
  * ポップアップダイアログが開く
* フォームに「test」と入力して、「送信」ボタンを押す
* 遷移先の http://localhost:8080 で右クリックし、「戻る」を押す
* 遷移先の 「form.html」 で右クリックし、「進む」を押す
  * [ ] ERR_CACHE_MISSページが表示されること

### すべてのURLを対象にした時の動作確認（警告ダイアログ有）

#### 準備

以下の通り設定して検証を行う。

* [doc\verify\sources\TestTools/Scenarios/scenario2.ini](../TestTools/Scenarios/scenario2.ini) を `C:\Program Files\PrintCanceler\PrintCanceler.ini` に配置する。
  * これは、「https://example.com」を開いている場合は
* 念のためEdgeを再起動する

#### 検証

##### ダイアログを自動でキャンセルするケース

* `powershell doc\verify\sources\TestTools\http_server.ps1`でローカルのWebサーバーを起動する
  * `http://localhost:8080`で簡易Webサーバーが起動する
* `doc\verify\sources\TestTools\form.html` を開く
* フォームに「test」と入力して、「送信」ボタンを押す
* 「form.html」 をリロードする
  * [ ] 「フォームを再送信しますか？」ダイアログ一瞬表示され、キャンセルされること
  * [ ] 「フォームの再送信が発生するため、このサイトでのリロードは禁止されています。\n\nリロードはキャンセルされました。」という警告ダイアログが表示されること
  * [ ] 警告ダイアログが前面に表示されること（Edgeの後ろに隠れないこと）
* 警告ダイアログをOKで閉じる
* 「form.html」 をリロードする
* 「フォームの再送信が発生するため、このサイトでのリロードは禁止されています。\n\nリロードはキャンセルされました。」という警告ダイアログが表示された状態で、再度「form.html」 をリロードする
* 「form.html」 をリロードする
  * [ ] 「フォームを再送信しますか？」ダイアログ一瞬表示され、キャンセルされること
  * [ ] 「フォームの再送信が発生するため、このサイトでのリロードは禁止されています。\n\nリロードはキャンセルされました。」という警告ダイアログが表示されること
  * [ ] 警告ダイアログが前面に表示されること（Edgeの後ろに隠れないこと）
* すべての警告ダイアログをOKで閉じる

##### ERR_CACHE_MISSページを自動で閉じるケース

* `powershell doc\verify\sources\TestTools\http_server.ps1`でローカルのWebサーバーを起動する
  * `http://localhost:8080`で簡易Webサーバーが起動する
* `doc\verify\sources\TestTools\form.html` を開く
* 「フォームをダイアログで開くボタン」を押す
  * ポップアップダイアログが開く
* フォームに「test」と入力して、「送信」ボタンを押す
* 遷移先の http://localhost:8080 で右クリックし、「戻る」を押す
* 遷移先の 「form.html」 で右クリックし、「進む」を押す
  * [ ] 「フォームの再送信が発生するため、このサイトでのリロードは禁止されています。\n\nリロードはキャンセルされました。」という警告ダイアログが表示**されない**こと
    * このケースは非サポート。警告を出しているネイティブアプリ側ではなく、拡張機能側が閉じていることと、そもそもキャンセルされているので。
  * [ ] ポップアップダイアログが閉じること

### ダイアログを閉じる機能で特定のURLを対象にした時の動作確認（警告ダイアログ無）

#### 補足

本アドオンのダイアログを自動でキャンセルする機能は、現在のタブが指定したURLを開いていなくても、いずれかのタブで指定したURLが開いている場合に動作する。
これは、ネイティブアプリ側で現在開いているタブを判定するのが難しいためである。

#### 準備

以下の通り設定して検証を行う。

* [doc\verify\sources\TestTools/Scenarios/scenario3.ini](../TestTools/Scenarios/scenario3.ini) を `C:\Program Files\PrintCanceler\PrintCanceler.ini` に配置する。
  * 設定の内容は以下の通り
    * 以下のサイトを対象とする
      * `*://example.com/jp*`
      * `*://example.com/us/??/`
      * `https://www.clear-code.com/`
    * 以下のサイトを除外する
      * `*://example.com/jp/exclude*`
* 念のためEdgeを再起動する

#### 検証

##### ダイアログを自動でキャンセルするケース

* `powershell doc\verify\sources\TestTools\http_server.ps1`でローカルのWebサーバーを起動する
  * `http://localhost:8080`で簡易Webサーバーが起動する
* `https://example.com/jp/exclude` を開く
* `doc\verify\sources\TestTools\form.html` を開く
* フォームに「test」と入力して、「送信」ボタンを押す
* 「form.html」 をリロードする
  * [ ] 「フォームを再送信しますか？」ダイアログが表示され、キャンセル**されない**こと
* 「フォームを再送信しますか？」ダイアログをキャンセルで閉じる
* `https://example.com/jp/` を開く
* 「form.html」 をリロードする
  * [ ] 「フォームを再送信しますか？」ダイアログ一瞬表示され、キャンセルされること
* Edgeを終了する
* タスクマネージャーを起動する
* 詳細タブを開く
* PrintCanceler.exeが起動していれば終了する
* Edgeを起動する
* `https://example.com/us/ab/` を開く
* `doc\verify\sources\TestTools\form.html` を開く
* フォームに「test」と入力して、「送信」ボタンを押す
* 「form.html」 をリロードする
  * [ ] 「フォームを再送信しますか？」ダイアログ一瞬表示され、キャンセルされること
* Edgeを終了する
* タスクマネージャーを起動する
* 詳細タブを開く
* PrintCanceler.exeが起動していれば終了する
* Edgeを起動する
* `https://www.clear-code.com/` を開く
* `doc\verify\sources\TestTools\form.html` を開く
* フォームに「test」と入力して、「送信」ボタンを押す
* 「form.html」 をリロードする
  * [ ] 「フォームを再送信しますか？」ダイアログ一瞬表示され、キャンセルされること

### ERR_CACHE_MISSページを閉じる機能で特定のURLを対象にした時の動作確認

#### 補足

ERR_CACHE_MISSページを閉じる機能は指定したURLでのみ動作する。

#### 対象のページではない場合

##### 準備

以下の通り設定して検証を行う。

* [doc\verify\sources\TestTools/Scenarios/scenario3.ini](../TestTools/Scenarios/scenario3.ini) を `C:\Program Files\PrintCanceler\PrintCanceler.ini` に配置する。
  * 設定の内容は以下の通り
    * 以下のサイトを対象とする
      * `*://example.com/jp*`
      * `*://example.com/us/??/`
      * `https://www.clear-code.com/`
    * 以下のサイトを除外する
      * `*://example.com/jp/exclude*`
* 念のためEdgeを再起動する

##### 検証

* `powershell doc\verify\sources\TestTools\http_server.ps1`でローカルのWebサーバーを起動する
  * `http://localhost:8080`で簡易Webサーバーが起動する
* `doc\verify\sources\TestTools\form.html` を開く
* 「フォームをダイアログで開くボタン」を押す
  * ポップアップダイアログが開く
* フォームに「test」と入力して、「送信」ボタンを押す
* 遷移先の http://localhost:8080 で右クリックし、「戻る」を押す
* 遷移先の 「form.html」 で右クリックし、「進む」を押す
  * [ ] ERR_CACHE_MISS（フォームの再送信しますか？）エラーページが表示されること

#### 対象のページの場合

##### 準備

以下の通り設定して検証を行う。

* [doc\verify\sources\TestTools/Scenarios/scenario4.ini](../TestTools/Scenarios/scenario4.ini) を `C:\Program Files\PrintCanceler\PrintCanceler.ini` に配置する。
  * 設定の内容は以下の通り
    * 以下のサイトを対象とする
      * `*://localhost:8080*`
* 念のためEdgeを再起動する

補足: マッチング処理の実装はダイアログキャンセルと同様であるため、ここでは詳しいマッチング処理の判定まではテストしない。

##### 検証

* `powershell doc\verify\sources\TestTools\http_server.ps1`でローカルのWebサーバーを起動する
  * `http://localhost:8080`で簡易Webサーバーが起動する
* `doc\verify\sources\TestTools\form.html` を開く
* 「フォームをダイアログで開くボタン」を押す
  * ポップアップダイアログが開く
* フォームに「test」と入力して、「送信」ボタンを押す
* 遷移先の http://localhost:8080 で右クリックし、「戻る」を押す
* 遷移先の 「form.html」 で右クリックし、「進む」を押す
  * [ ] ダイアログが閉じること
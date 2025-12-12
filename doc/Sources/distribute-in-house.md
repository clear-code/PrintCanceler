---
CJKmainfont: Noto Sans CJK JP
CJKoptions:
  - BoldFont=Noto Sans CJK JP Bold
titlepage-logo: ./distribute-in-house/media/image1.png
title: |
  Microsoft Edge用PrintCanceler拡張機能
  組織内サーバーを用いた配布・更新手順
subject: 組織内サーバーを用いた配布・更新手順
date: 2025/12/12
author: 株式会社クリアコード
keywords: [PrintCanceler, Distribute in-house]
titlepage: true
toc-title: 目次
toc-own-page: true
---

更新履歴

| 日付       | Version | 備考                                  |
|------------|---------|---------------------------------------|
| 2025/12/12 | 1.0.0   | 第1版                                 |

# 目的

本手順書は、Active Directoryドメイン参加端末を対象として、組織内サーバーを使用してMicrosoft Edge用PrintCanceler拡張機能（以下、PrintCanceler拡張機能）を配布する手順をまとめたものです。

# 概要

PrintCanceler拡張機能は、ADドメイン参加端末に対してグループポリシー（GPO）で導入のための設定を行い、設定を認識した各端末のEdgeが、PrintCanceler拡張機能のファイルをダウンロードしてくることによりインストールされます。 通常、GPOを用いた拡張機能のインストール時には、拡張機能のファイルはMicrosoftのWebストアからインストールされます。この場合、ブラウザーおよびWebストアの仕様により、インストールされる拡張機能は常にWebストア上の最新バージョンとなり、それ以外の任意のバージョンを使用することはできません。

本手順書では、Webストアからではなくお客さま組織内サーバーからPrintCanceler拡張機能の更新情報を取得するようにGPOを設定することにより、当該サーバーに設置した任意のバージョンのPrintCanceler拡張機能を、ADドメイン参加端末へ新規または更新の形で一斉インストールさせる手順を説明します。

# 前提

本項目の作業手順は以下の作業環境で実施することを想定します。

* 作業者：システム管理者 1名
* 作業環境：Active Directoryドメインコントローラーを操作可能な Windows 端末 1台
* 適用時に必要なサーバー：全端末からアクセス可能なファイル配布用サーバー 1台
  * Windows ファイル共有サーバー、もしくは WWW サーバーが必要です。
  * 本サーバーはドメインコントローラーとの兼用可です。
* ドメインコントローラーに、Microsoft Edgeのグループポリシーテンプレートが導入済みである必要があります。  
  https://learn.microsoft.com/ja-jp/deployedge/configure-microsoft-edge

また、クリアコードが提供する PrintCanceler_組織内サーバー配布.zip （組織内サーバー配布用PrintCanceler拡張機能関連ファイル一式を含むZipファイル）を準備します。

\newpage
# PrintCanceler拡張機能の組織内サーバーからのインストール

## 一部の検証対象端末を対象としたインストール（事前検証）

全体展開前の検証のために、ADドメイン参加端末のうち、情報システム部門のみの端末など、一部の端末のみを対象として、PrintCanceler拡張機能を組織内サーバーで配布する形で新規に導入する場合、もしくは、組織内サーバーでの配布を行っていなかった状態から組織内サーバーでの配布へ切り替える場合について、手順の大まかな流れは以下の通りです。

1. GPOによるPrintCanceler拡張機能の新規インストール
2. 端末でPrintCanceler拡張機能が新規インストールされたことの確認

以下に具体的な作業手順を記します。

### GPOによるPrintCanceler拡張機能の新規インストール

組織内サーバーに設置したPrintCanceler拡張機能をインストールするため、グループ ポリシーの設定を変更します。

以下の作業はすべて、作業環境にて、システム管理者が管理者ユーザーアカウントで実施します。

* 1.1 全端末からアクセス可能なファイル配布用サーバー上に、一般ユーザー権限で読み取り可能な、ファイル配布用フォルダーを作成します。  
以下、コンピューター名/ホスト名が「fileserver」である Windowsファイル共有サーバーを使用し、ファイル配布用フォルダー名は「print-canceler-test」を使用するものと仮定します。  
この仮定に従い、ファイル共有サーバー上に作成された共有フォルダーのUNCパスが「`\\fileserver\print-canceler-test\`」 となると仮定します。
* 1.2 Windows のエクスプローラーもしくはZip形式の展開が可能なツールを使用し、PrintCanceler_組織内サーバー配布.zipを展開します。  
PrintCanceler_組織内サーバー配布.zip はMicrosoft Edge拡張機能バージョンごとに異なります。この手順では、PrintCanceler拡張機能バージョン1.0.0を使用しています。  
以下の2つのファイルが展開されます。  
「edge.crx」  
「manifest.xml」
* 1.3 1.2 で展開された2つのファイルを、1.1で作成したファイル配布用フォルダーにコピーします。  
前述の仮定に従い、各ファイルの UNC パスは  
「`\\fileserver\print-canceler-test\edge.crx`」  
「`\\fileserver\print-canceler-test\manifest.xml`」  
となると仮定します。
* 1.4 配置した各ファイルの「プロパティ」を開き、「セキュリティ」タブを選択して、当該ファイルが「Everyone」で読み取り可能な状態になっていることを確認します。  
もしそのようになっていない場合は、「編集」ボタンをクリックし、開かれたダイアログ内で「追加」ボタンをクリックして、開かれたダイアログ内で「選択するオブジェクト名を入力してください」欄に「Everyone」と入力し、「OK」ボタンを押してダイアログを閉じる操作を3回繰り返して、ファイルを「Everyone」で読み取り可能な状態に設定します。
* 1.5 1.3で設置した「manifest.xml」を「メモ帳」もしくは何らかのテキスト編集ツールで開きます。ファイルが以下のような内容であることを確認します。

```
<?xml version='1.0' encoding='UTF-8'?>
<gupdate xmlns='http://www.google.com/update2/response' protocol='2.0'>
  <app appid='mpcpmdhaecmjiaglbkaejhhiipghlfea'>
    <updatecheck
      codebase='（edge.crx の URL）'
      version='（バージョン番号）' />
  </app>
</gupdate>
```

* 1.6 1.5 で開いたファイルの「codebase='」から「'」までの間の箇所を、1.3で設置 したファイル「edge.crx」の URL で置き換えます。また、「version='」から 「'」までの間の箇所を、インストールするPrintCanceler拡張機能のバージョンで置き換えます。  
前述の仮定に従うと、ファイルのURLはUNCパスに基づいて  
「file://fileserver/print-canceler-test/edge.crx」  
となり、PrintCanceler拡張機能バージョン 1.0.0 をインストールする場合の編集後の「manifest.xml」の内容は以下の要領となります。

```
<?xml version='1.0' encoding='UTF-8'?>
<gupdate xmlns='http://www.google.com/update2/response' protocol='2.0'>
  <app appid='mpcpmdhaecmjiaglbkaejhhiipghlfea'>
    <updatecheck
      codebase='file://fileserver/print-canceler-test/edge.crx'
      version='1.0.0' />
  </app>
</gupdate>
```

* 1.7 編集後の「manifest.xml」を上書き保存します。
* 1.8 検証対象の端末に一般ユーザーでログインし、1.3で配置した2つのファイルを読み取れることを確認します。
  * manifest.xmlのURL（前述の仮定に従うと「file://fileserver/print-canceler-test/manifest.xml」）をEdgeで開き、1.6で設定した通りの内容を読み取れることを確認します。
  * edge.crxのURL（前述の仮定に従うと「file://fileserver/print-canceler-test/edge.crx」）をEdgeで開き、ファイルがダウンロード可能であることを確認します。
* 1.9 グループポリシー管理コンソールを起動します。
* 1.10 検証対象の端末の一般ユーザーに適用するためのGPOを作成、もしくは検証対象の端末の一般ユーザーに適用されるGPOを用意します。  
ここでは例として、「PrintCanceler拡張機能（事前検証）」という名称のGPOを使用するものとします。
* 1.11 1.10 で用意した GPOについて、  
「管理用テンプレート」  
→「Microsoft Edge」  
→「拡張機能」  
→「サイレントインストールされる拡張機能を制御する」  
をダブルクリックして、当該ポリシーの設定画面を開きます。
* 1.12 設定の状態を「有効」に設定します。
* 1.13 「表示...」をクリックして、値の設定画面を開きます。
* 1.14 以下の項目を追加します。  
「mpcpmdhaecmjiaglbkaejhhiipghlfea;（manifest.xml の URL）」  
前述の仮定に従うと、追加する項目は以下の要領となります。  
「mpcpmdhaecmjiaglbkaejhhiipghlfea;file://fileserver/print-canceler-test/manifest.xml」
* 1.15 「OK」ボタンを押して、値の設定画面を閉じます。
* 1.16 「OK」ボタンを押して、ポリシーの設定画面を閉じます。

### 端末でPrintCanceler拡張機能がインストールされたことの確認

端末の強制再起動などを行い、グループポリシーが検証対象の端末に反映された状態として下さい。

以下にグループポリシーの反映を確認する手順を示します。  

以下の作業はすべて、ADドメイン参加状態の検証対象の端末にて、システム管理者または一般ユーザーが一般ユーザーアカウントで実施します。

* 2.1 Edgeを起動します。
* 2.2 数秒待ち、Edgeのツールバー上にパズルピース型のボタンが表示されることを確認します。
* 2.3 Edgeのツールバー上にパズルピース型のボタンをクリックし、「拡張機能の管理」をクリックします。
* 2.4 拡張機能の管理画面が開かれますので、「インストール済の拡張機能」の一覧に以下の項目が存在する事を確認します。
  * PrintCanceler
* 2.5 項目の詳細情報を表示し、バージョンが組織内サーバーで配布しているバージョンと一致していることを確認します。

以上の手順により、組織内サーバーに設置したバージョンのPrintCanceler拡張機能が、検証対象の端末にインストールされます。

## 全端末を対象としたインストール（本番展開）

ADドメイン参加端末全体を対象として、PrintCanceler拡張機能を組織内サーバーで配布する形で新規に導入する場合、もしくは、組織内サーバーでの配布を行っていなかった状態から組織内サーバーでの配布へ切り替える場合について、手順の大まかな流れは以下の通りです。

1. GPOによるPrintCanceler拡張機能の新規インストール
2. 端末でPrintCanceler拡張機能が新規インストールされたことの確認

以下に、具体的な作業手順を記します。

### GPOによるPrintCanceler拡張機能の新規インストール

組織内サーバーに設置したPrintCanceler拡張機能をインストールするため、グループポリシーの設定を変更します。  
以下の作業はすべて、作業環境にて、システム管理者が管理者ユーザーアカウントで実施します。

* 1.1 全端末からアクセス可能なファイル配布用サーバー上に、一般ユーザー権限で読み取り可能な、事前検証用とは別のファイル配布用フォルダーを作成します。  
以下、コンピューター名/ホスト名が「fileserver」である Windowsファイル共有サーバーを使用し、全体展開用のファイル配布用フォルダー名は「print-canceler」を使用するものと仮定します。  
この仮定に従い、ファイル共有サーバー上に作成された共有フォルダーの UNCパスが「`\\fileserver\print-canceler\`」となると仮定します。
* 1.2 Windows のエクスプローラーもしくはZip形式の展開が可能なツールを使用し、PrintCanceler_組織内サーバー配布.zipを展開します。   
以下の2つのファイルが展開されます。  
「edge.crx」  
「manifest.xml」
* 1.3 1.2 で展開された2つのファイルを、1.1で作成したファイル配布用フォルダーにコピーします。  
前述の仮定に従い、各ファイルのUNCパスは  
「`\\fileserver\print-canceler\edge.crx`」  
「`\\fileserver\print-canceler\manifest.xml`」  
となると仮定します。
* 1.4 配置した各ファイルの「プロパティ」を開き、「セキュリティ」タブを選択して、当該ファイルが「Everyone」で読み取り可能な状態になっていることを確認します。  
もしそのようになっていない場合は、「編集」ボタンをクリックし、開かれたダイアログ内で「追加」ボタンをクリックして、開かれたダイアログ内で「選択するオブジェクト名を入力してください」欄に「Everyone」と入力し、「OK」ボタンを押してダイアログを閉じる操作を3回繰り返して、ファイルを「Everyone」で読み取り可能な状態に設定します。
* 1.5 1.3で設置した「manifest.xml」を「メモ帳」もしくは何らかのテキスト編集ツールで開きます。ファイルが以下のような内容であることを確認します。

```
<?xml version='1.0' encoding='UTF-8'?>
<gupdate xmlns='http://www.google.com/update2/response' protocol='2.0'>
  <app appid='mpcpmdhaecmjiaglbkaejhhiipghlfea'>
    <updatecheck
      codebase='（edge.crx の URL）'
      version='（バージョン番号）' />
  </app>
</gupdate>
```

* 1.6 1.5で開いたファイルの「codebase='」から「'」までの間の箇所を、1.3で設置したファイル「edge.crx」のURLで置き換えます。また、「version ='」から「'」までの間の箇所を、インストールするPrintCanceler拡張機能のバージョンで置き換えます。  
前述の仮定に従うと、ファイルのURLはUNCパスに基づいて  
「file://fileserver/print-canceler/edge.crx」  
となり、PrintCanceler拡張機能バージョン 1.0.0をインストールする場合の編集後の「manifest.xml」の内容は以下の要領となります。

```
<?xml version='1.0' encoding='UTF-8'?>
<gupdate xmlns='http://www.google.com/update2/response' protocol='2.0'>
  <app appid='mpcpmdhaecmjiaglbkaejhhiipghlfea'>
  <updatecheck
    codebase='file://fileserver/print-canceler/edge.crx'
    version='1.0.0' />
  </app>
</gupdate>
```

* 1.7 編集後の「manifest.xml」を上書き保存します。
* 1.8 任意のADドメイン参加端末に一般ユーザーでログインし、1.3 で配置した2つのファイルを読み取れることを確認します。
  * 1.8.1 manifest.xmlのURL（前述の仮定に従うと「file://fileserver/print-canceler/manifest.xml」）をEdgeで開き、1.6で設定した通りの内容を読み取れることを確認します。
  * 1.8.2 edge.crxのURL（前述の仮定に従うと「file://fileserver/print-canceler/edge.crx」）をEdgeで開き、ファイル がダウンロード可能であることを確認します。
* 1.9 グループポリシー管理コンソールを起動します。
* 1.10 全端末の一般ユーザーに適用するための GPOを作成、もしくは全端末の一般ユーザーに適用されるGPOを用意します。  
ここでは例として、「PrintCanceler拡張機能」という名称のGPOを使用するものとします。
* 1.11 1.10で用意したGPOについて、  
「管理用テンプレート」  
→「Microsoft Edge」  
→「拡張機能」  
→「サイレントインストールされる拡張機能を制御する」  
をダブルクリックして、当該ポリシーの設定画面を開きます。
* 1.12 設定の状態を「有効」に設定します。
* 1.13 「表示...」をクリックして、値の設定画面を開きます。
* 1.14 以下の項目を追加します。  
「mpcpmdhaecmjiaglbkaejhhiipghlfea;（manifest.xml の URL）」  
前述の仮定に従うと、追加する項目は以下の要領となります。  
「mpcpmdhaecmjiaglbkaejhhiipghlfea;file://fileserver/print-canceler/manifest.xml」
* 1.15 「OK」ボタンを押して、値の設定画面を閉じます。
* 1.16 「OK」ボタンを押して、ポリシーの設定画面を閉じます。

### 端末でPrintCanceler拡張機能がインストールされたことの確認

端末の強制再起動などを行い、グループポリシーが全ての端末に反映された状態として下さい。 

以下にグループポリシーの反映を確認する手順を示します。  
以下の作業はすべて、任意のADドメイン参加端末にて、システム管理者または一般ユーザーが一般ユーザーアカウントで実施します。全台で確認することは不可能ですので、何台か抽出して確認されることを推奨します。

* 2.1 Edgeを起動します。
* 2.2 数秒待ち、Edgeのツールバー上にパズルピース型のボタンが表示されることを確認します。
* 2.3 Edgeのツールバー上にパズルピース型のボタンをクリックし、「拡張機能の管理」をクリックします。
* 2.4 拡張機能の管理画面が開かれますので、「インストール済の拡張機能」の一覧に以下の項目が存在する事を確認します。
  * PrintCanceler
* 2.5 項目の詳細情報を表示し、バージョンが組織内サーバーで配布しているバージョンと一致していることを確認します。

以上の手順により、組織内サーバーに設置したバージョンのPrintCanceler拡張機能が、各端末に導入されます。

\newpage
# PrintCanceler拡張機能の組織内サーバーからの更新

## 一部の検証対象端末を対象とした更新（事前検証）

全体展開前の検証のために、ADドメイン参加端末のうち、情報システム部門のみの端末など、一部の端末のみを対象として、PrintCanceler拡張機能を組織内サーバーで配布する形で新バージョンへ更新する場合について、手順の大まかな流れは以下の通りです。

1. GPOによるPrintCanceler拡張機能の更新
2. 端末でPrintCanceler拡張機能が更新されたことの確認

**PrintCanceler拡張機能の組織内サーバーからのインストール時に、事前検証と本番展開で同一のファイル配布用フォルダーを使用していた場合には、以下に記す手順では、検証対象の一部の端末のみを対象として更新を実施することができません。その場合、「PrintCanceler拡張機能の組織内サーバーからのインストール」の「一部の検証対象端末を対象としたインストール（事前検証）」に記載の手順に則り、検証対象の端末について、事前検証用のファイル配布用フォルダーに設置した「manifest.xml」および「edge.crx」が使用される状態にあらかじめ変更しておいて下さい。**

以下に、検証対象の端末用に事前検証用のファイル配布用フォルダーが使われている状態を前提として、具体的な作業手順を記します。

### PrintCanceler拡張機能のファイルの更新

組織内サーバーに設置したPrintCanceler拡張機能のファイルを更新します。

以下の作業はすべて、作業環境にて、システム管理者が管理者ユーザーアカウントで実施します。

* 1.1 ファイル配布サーバー上の検証対象の端末用のファイル配布用フォルダーを開きます。  
「PrintCanceler拡張機能の組織内サーバーからのインストール」での仮定に従い、コンピューター名/ホスト名が「fileserver」である Windowsファイル共有サーバーを使用し、ファイル配布用フォルダー名は「print-canceler-test」を使用するものと仮定します。
* 1.2 新バージョンのPrintCanceler拡張機能のパッケージファイル「edge.crx」を用意します。
* 1.3 新バージョンの「edge.crx」を、1.1で開いたファイル配布用フォルダーに、既存のファイルへ上書きする形でコピーします。
* 1.4 ファイル配布用フォルダー内の「manifest.xml」を「メモ帳」もしくは何らかのテキスト編集ツールで開きます。ファイルが以下のような内容であることを確認します。

```
<?xml version='1.0' encoding='UTF-8'?>
<gupdate xmlns='http://www.google.com/update2/response' protocol='2.0'>
  <app appid='mpcpmdhaecmjiaglbkaejhhiipghlfea'>
  <updatecheck
    codebase='（edge.crx の URL）'
    version='（バージョン番号）' />
  </app>
</gupdate>
```

* 1.5 1.4 で開いたファイルの「version='」から「'」までの間の箇所を、更新後のPrintCanceler拡張機能のバージョンで置き換えます。  
前述の仮定に従うと、ファイルのURLはUNCパスに基づいて  
「file://fileserver/print-canceler-test/edge.crx」  
となり、PrintCanceler拡張機能をバージョン1.1.0へ更新する場合の編集後の「manifest.xml」の内容は以下の要領となります。

```
<?xml version='1.0' encoding='UTF-8'?>
<gupdate xmlns='http://www.google.com/update2/response' protocol='2.0'>
  <app appid='mpcpmdhaecmjiaglbkaejhhiipghlfea'>
  <updatecheck
    codebase='file://fileserver/print-canceler-test/edge.crx'
    version='1.1.0' />
  </app>
</gupdate>
```

* 1.6 編集後の「manifest.xml」を上書き保存します。
* 1.7 ファイル配布用フォルダー内の各ファイルの「プロパティ」を開き、「セキュリティ」タブを選択して、当該ファイルが「Everyone」で読み取り可能な状態になっていることを確認します。  
もしそのようになっていない場合は、「編集」ボタンをクリックし、開かれたダイアログ内で「追加」ボタンをクリックして、開かれたダイアログ内で「選択するオブジェクト名を入力してください」欄に「Everyone」と入力し、「OK」ボタンを押してダイアログを閉じる操作を3回繰り返して、ファイルを「Everyone」で読み取り可能な状態に設定します。
* 1.8 検証対象の端末に一般ユーザーでログインし、ファイル配布用フォルダー内のファイルを読み取れることを確認します。
  * manifest.xmlの URL（前述の仮定に従うと「file://fileserver/print-canceler-test/manifest.xml」）をEdgeで開き、1.5で設定した通りの内容を読み取れることを確認します。
  * edge.crx のURL（前述の仮定に従うと「file://fileserver/print-cancelertest/edge.crx」）をEdgeで開き、ファイルがダウンロード可能であることを確認します。

### 端末でPrintCanceler拡張機能が更新されることの確認

以下の作業はすべて、ADドメイン参加状態の検証対象の端末にて、システム管理者または一般ユーザーが一般ユーザーアカウントで実施します。

* 2.1 Edgeを起動します。
* 2.2 Edgeのツールバー上にパズルピース型のボタンをクリックし、「拡張機能の管理」をクリックします。
* 2.3 拡張機能の管理画面が開かれますので、「開発者モード」を有効化します。
* 2.4 「インストール済の拡張機能」の見出し横に表示される「更新」ボタンをクリックします。
* 2.5 読み込み中を示すアニメーションが表示されますので、アニメーションが消えて「拡張機能が更新されました」というメッセージが表示されるまで待ちます。
* 2.6 「インストール済の拡張機能」の一覧にある「PrintCanceler」の詳細情報を表示し、バージョンが更新後のバージョンと一致していることを確認します。
* 2.7 「開発者モード」を無効化します。

以上の手順により、組織内サーバーに設置したバージョンのPrintCanceler拡張機能が、検証対象の端末に反映されます。

## 全端末を対象とした更新（本番展開）

ADドメイン参加端末全体を対象として、PrintCanceler拡張機能を組織内サーバーで配布する形で新バージョンへ更新する場合について、手順の大まかな流れは以下の通りです。

1. GPOによるPrintCanceler拡張機能の更新
2. 端末でPrintCanceler拡張機能が更新されたことの確認

以下に、具体的な作業手順を記します。

### PrintCanceler拡張機能のファイルの更新

組織内サーバーに設置したPrintCanceler拡張機能のファイルを更新します。  
以下の作業はすべて、作業環境にて、システム管理者が管理者ユーザーアカウントで実施します。

* 1.1 ファイル配布サーバー上の全体展開用のファイル配布用フォルダーを開きます。  
「PrintCanceler拡張機能の組織内サーバーからのインストール」での仮定に従い、コンピューター名/ホスト名が「fileserver」であるWindowsファイル共有サーバーを使用し、ファイル配布用フォルダー名は「print-canceler」を使用するものと仮定します。
* 1.2 新バージョンのPrintCanceler拡張機能のパッケージファイル「edge.crx」を用意します。
* 1.3 新バージョンの「edge.crx」を、1.1で開いたファイル配布用フォルダーに、既存のファイルへ上書きする形でコピーします。
* 1.4 ファイル配布用フォルダー内の「manifest.xml」を「メモ帳」もしくは何らかのテキスト編集ツールで開きます。ファイルが以下のような内容であることを確認します。 

```
<?xml version='1.0' encoding='UTF-8'?>
<gupdate xmlns='http://www.google.com/update2/response' protocol='2.0'>
  <app appid='mpcpmdhaecmjiaglbkaejhhiipghlfea'>
    <updatecheck
      codebase='（edge.crx の URL）'
      version='（バージョン番号）' />
  </app>
</gupdate>
```

* 1.5 1.4 で開いたファイルの「version='」から「'」までの間の箇所を、更新後のPrintCanceler拡張機能のバージョンで置き換えます。  
前述の仮定に従うと、ファイルのURLは UNCパスに基づいて  
「file://fileserver/print-canceler/edge.crx」  
となり、PrintCanceler拡張機能をバージョン 1.1.0へ更新する場合の編集後の「manifest.xml」の内容は以下の要領となります。

```
<?xml version='1.0' encoding='UTF-8'?>
<gupdate xmlns='http://www.google.com/update2/response' protocol='2.0'>
  <app appid='mpcpmdhaecmjiaglbkaejhhiipghlfea'>
    <updatecheck
      codebase='file://fileserver/print-canceler/edge.crx'
      version='1.1.0' />
  </app>
</gupdate>
```

* 1.6 編集後の「manifest.xml」を上書き保存します。
* 1.7 ファイル配布用フォルダー内の各ファイルの「プロパティ」を開き、「セキュリティ」タブを選択して、当該ファイルが「Everyone」で読み取り可能な状態になっていることを確認します。  
もしそのようになっていない場合は、「編集」ボタンをクリックし、開かれたダイアログ内で「追加」ボタンをクリックして、開かれたダイアログ内で「選択するオブジェクト名を入力してください」欄に「Everyone」と入力し、「OK」ボタンを押してダイアログを閉じる操作を3回繰り返して、ファイルを「Everyone」で読み取り可能な状態に設定します。
* 1.8 ADドメイン参加状態の任意の端末に一般ユーザーでログインし、ファイル配布用フォルダー内のファイルを読み取れることを確認します。 
  * manifest.xmlのURL（前述の仮定に従うと「file://fileserver/print-canceler/manifest.xml」）をEdgeで開き、1.5で設定した通りの内容を読み取れることを確認します。
  * edge.crx の URL（前述の仮定に従うと「file://fileserver/print-canceler/edge.crx」）をEdgeで開き、ファイルがダウンロード可能であることを確認します。

### 端末でPrintCanceler拡張機能が更新されることの確認

以下の作業はすべて、ADドメイン参加状態の任意の端末にて、システム管理者または一般ユーザーが一般ユーザーアカウントで実施します。

* 2.1 Edgeを起動します。
* 2.2 Edgeのツールバー上にパズルピース型のボタンをクリックし、「拡張機能の管理」をクリックします。
* 2.3 拡張機能の管理画面が開かれますので、「開発者モード」を有効化します。
* 2.4 「インストール済の拡張機能」の見出し横に表示される「更新」ボタンをクリックします。
* 2.5 読み込み中を示すアニメーションが表示されますので、アニメーションが消えて「拡張機能が更新されました」というメッセージが表示されるまで待ちます。
* 2.6 「インストール済の拡張機能」の一覧にある「PrintCanceler」の詳細情報を表示し、バージョンが更新後のバージョンと一致していることを確認します。
* 2.7 「開発者モード」を無効化します。

以上の手順により、組織内サーバーに設置したバージョンのPrintCanceler拡張機能が、各端末に反映されます。

\newpage
# PrintCanceler拡張機能の組織内サーバーからの切り戻し

## 一部の検証対象端末を対象とした切り戻し（事前検証）

全体展開前の検証のために、ADドメイン参加端末のうち、情報システム部門のみの端末など、一部の端末のみを対象として、PrintCanceler拡張機能を組織内サーバーで配布する形で旧バージョンへ切り戻す場合について、手順の大まかな流れは以下の通りです。

1. GPOによるPrintCanceler拡張機能の切り戻し
2. 端末内にあるインストール済み拡張機能の情報の削除
3. 端末でPrintCanceler拡張機能が切り戻されたことの確認

**PrintCanceler拡張機能の組織内サーバーからのインストール時に、事前検証と本番展開で同一のファイル配布用フォルダーを使用していた場合には、以下に記す手順では、検証対象の一部の端末のみを対象として切り戻しを実施することができません。その場合、「PrintCanceler拡張機能の組織内サーバーからのインストール」の「一部の検証対象端末を対象としたインストール（事前検証）」に記載の手順に則り、検証対象の端末について、事前検証用のファイル配布用フォルダーに設置した「manifest.xml」および「edge.crx」が使用される状態にあらかじめ変更しておいて下さい。**

以下に、検証対象の端末用に事前検証用のファイル配布用フォルダーが使われている状態を前提として、具体的な作業手順を記します。

### PrintCanceler拡張機能のファイルの切り戻し

組織内サーバーに設置したPrintCanceler拡張機能のファイルを、切り戻し対象のバージョンに差し替えます。  
以下の作業はすべて、作業環境にて、システム管理者が管理者ユーザーアカウントで実施します。

* 1.1 ファイル配布サーバー上の検証対象の端末用のファイル配布用フォルダーを開きます。  
「PrintCanceler拡張機能の組織内サーバーからのインストール」での仮定に従い、コンピューター名/ホスト名が「fileserver」であるWindowsファイル共有サーバーを使用し、ファイル配布用フォルダー名は「print-canceler-test」を使用するものと仮定します。
* 1.2 切り戻し先となる旧バージョンのPrintCanceler拡張機能のパッケージファイル「edge.crx」を用意します。
* 1.3 旧バージョンの「edge.crx」を、1.1で開いたファイル配布用フォルダーに、既存のファイルへ上書きする形でコピーします。
* 1.4 ファイル配布用フォルダー内の「manifest.xml」を「メモ帳」もしくは何らかのテキスト編集ツールで開きます。ファイルが以下のような内容であることを確認します。

```
<?xml version='1.0' encoding='UTF-8'?>
<gupdate xmlns='http://www.google.com/update2/response' protocol='2.0'>
  <app appid='mpcpmdhaecmjiaglbkaejhhiipghlfea'>
    <updatecheck
      codebase='（edge.crx の URL）'
      version='（バージョン番号）' />
  </app>
</gupdate>
```

* 1.5 1.4 で開いたファイルの「version='」から「'」までの間の箇所を、更新後のPrintCanceler拡張機能のバージョンで置き換えます。  
前述の仮定に従うと、ファイルのURLはUNCパスに基づいて  
「file://fileserver/print-canceler-test/edge.crx」  
となり、PrintCanceler拡張機能をバージョン1.1.0からバージョン1.0.0へ切り戻す場合の編集後の「manifest.xml」の内容は以下の要領となります。

```
<?xml version='1.0' encoding='UTF-8'?>
<gupdate xmlns='http://www.google.com/update2/response' protocol='2.0'>
  <app appid='mpcpmdhaecmjiaglbkaejhhiipghlfea'>
    <updatecheck
      codebase='file://fileserver/print-canceler-test/edge.crx'
      version='1.0.0' />
  </app>
</gupdate>
```

* 1.6 編集後の「manifest.xml」を上書き保存します。
* 1.7 ファイル配布用フォルダー内の各ファイルの「プロパティ」を開き、「セキュリティ」タブを選択して、当該ファイルが「Everyone」で読み取り可能な状態になっていることを確認します。  
もしそのようになっていない場合は、「編集」ボタンをクリックし、開かれたダイアログ内で「追加」ボタンをクリックして、開かれたダイアログ内で「選択するオブジェクト名を入力してください」欄に「Everyone」と入力し、「OK」ボタンを押してダイアログを閉じる操作を3回繰り返して、ファイルを「Everyone」で読み取り可能な状態に設定します。
* 1.8 検証対象の端末に一般ユーザーでログインし、ファイル配布用フォルダー内のファイルを読み取れることを確認します。 
  * manifest.xmlのURL（前述の仮定に従うと「file://fileserver/print-canceler-test/manifest.xml」）をEdgeで開き、1.5で設定した通りの内容を読み取れることを確認します。
  * edge.crxのURL（前述の仮定に従うと「file://fileserver/print-cancelertest/edge.crx」）をEdgeで開き、ファイルがダウンロード可能であることを確認します。

### 端末内にあるインストール済み拡張機能の情報の削除

Edgeでは、拡張機能のバージョン管理において常に新しいバージョンが優先されます。そのため、古いバージョンの拡張機能に戻すには、端末内にある「インストール済み拡張機能」の情報を一度削除する必要があります。
以下の作業は、ADドメイン参加状態の検証対象の端末にて、システム管理者または一般ユーザーが一般ユーザーアカウントで実施します。

* 2.1 各端末内にある以下の二つのファイルを削除します。
  * `%LocalAppData%\Microsoft\Edge\User Data\Default\Preferences`
  * `%LocalAppData%\Microsoft\Edge\User Data\Default\Secure Preferences`
* 2.2 Windowsのタスクマネージャーのプロセスタブを開き、実行中の「Microsoft Edge」プロセスがあれば、右クリック→タスクの終了 で終了させます。
* 2.3 Windowsのタスクマネージャーの詳細タブを開き、実行中の「msedge.exe」プロセスがあれば、右クリック→タスクの終了 で終了させます。

### 端末でPrintCanceler拡張機能が切り戻されることの確認

以下の作業はすべて、ADドメイン参加状態の検証対象の端末にて、システム管理者または一般ユーザーが一般ユーザーアカウントで実施します。

* 3.1 Edgeを起動します。
* 3.2 Edgeのツールバー上にパズルピース型のボタンをクリックし、「拡張機能の管理」をクリックします。
* 3.3 拡張機能の管理画面が開かれますので、「開発者モード」を有効化します。
* 3.4 「インストール済の拡張機能」の見出し横に表示される「更新」ボタンをクリックします。
* 3.5 読み込み中を示すアニメーションが表示されますので、アニメーションが消えて「拡張機能が更新されました」というメッセージが表示されるまで待ちます。
* 3.6 「インストール済の拡張機能」の一覧にある「PrintCanceler」の詳細情報を表示し、バージョンが切り戻し後のバージョンと一致していることを確認します。
* 3.7 「開発者モード」を無効化します。

以上の手順により、組織内サーバーに設置したバージョンのPrintCanceler拡張機能が、検証対象の端末に反映されます。

## 全端末を対象とした切り戻し（本番展開）

ADドメイン参加端末全体を対象として、PrintCanceler拡張機能を組織内サーバーで配布する形で旧バージョンへ切り戻す場合について、手順の大まかな流れは以下の通りです。

1. GPOによるPrintCanceler拡張機能の切り戻し
2. 端末内にあるインストール済み拡張機能の情報の削除
3. 端末でPrintCanceler拡張機能が切り戻されたことの確認

以下に、具体的な作業手順を記します。

### PrintCanceler拡張機能のファイルの切り戻し

組織内サーバーに設置したPrintCanceler拡張機能のファイルを切り戻し対象のバージョンへ差し替えます。  
以下の作業はすべて、作業環境にて、システム管理者が管理者ユーザーアカウントで実施します。

* 1.1 ファイル配布サーバー上の全体展開用のファイル配布用フォルダーを開きます。「PrintCanceler拡張機能の組織内サーバーからのインストール」での仮定に従い、コンピューター名/ホスト名が「fileserver」であるWindowsファイル共有サーバーを使用し、ファイル配布用フォルダー名は「print-canceler」を使用するものと仮定します。
* 1.2 切り戻し先となる旧バージョンのPrintCanceler拡張機能のパッケージファイル「edge.crx」を用意します。
* 1.3 旧バージョンの「edge.crx」を、1.1で開いたファイル配布用フォルダーに、既存のファイルへ上書きする形でコピーします。
* 1.4 ファイル配布用フォルダー内の「manifest.xml」を「メモ帳」もしくは何らかのテキスト編集ツールで開きます。ファイルが以下のような内容であることを確認します。

```
<?xml version='1.0' encoding='UTF-8'?>
<gupdate xmlns='http://www.google.com/update2/response' protocol='2.0'>
  <app appid='mpcpmdhaecmjiaglbkaejhhiipghlfea'>
    <updatecheck
      codebase='（edge.crx の URL）'
      version='（バージョン番号）' />
  </app>
</gupdate>
```

* 1.5 1.4 で開いたファイルの「version='」から「'」までの間の箇所を、更新後のPrintCanceler拡張機能のバージョンで置き換えます。  
前述の仮定に従うと、ファイルのURLはUNCパスに基づいて  
「file://fileserver/print-canceler/edge.crx」  
となり、PrintCanceler拡張機能をバージョン1.1.0から1.0.0へ切り戻す場合の編集後の「manifest.xml」の内容は以下の要領となります。

```
<?xml version='1.0' encoding='UTF-8'?>
<gupdate xmlns='http://www.google.com/update2/response' protocol='2.0'>
  <app appid='mpcpmdhaecmjiaglbkaejhhiipghlfea'>
    <updatecheck
      codebase='file://fileserver/print-canceler/edge.crx'
      version='1.0.0' />
  </app>
</gupdate>
```

* 1.6 編集後の「manifest.xml」を上書き保存します。
* 1.7 ファイル配布用フォルダー内の各ファイルの「プロパティ」を開き、「セキュリティ」タブを選択して、当該ファイルが「Everyone」で読み取り可能な状態になっていることを確認します。  
もしそのようになっていない場合は、「編集」ボタンをクリックし、開かれたダイアログ内で「追加」ボタンをクリックして、開かれたダイアログ内で「選択するオブジェクト名を入力してください」欄に「Everyone」と入力し、「OK」ボタンを押してダイアログを閉じる操作を3回繰り返して、ファイルを「Everyone」で読み取り可能な状態に設定します。
* 1.8 ADドメイン参加状態の任意の端末に一般ユーザーでログインし、ファイル配布用フォルダー内のファイルを読み取れることを確認します。 
  * manifest.xmlのURL（前述の仮定に従うと「file://fileserver/print-canceler/manifest.xml」）をEdgeで開き、1.5で設定した通りの内容を読み取れることを確認します。
  * edge.crxのURL（前述の仮定に従うと「file://fileserver/print-canceler/edge.crx」）をEdgeで開き、ファイルがダウンロード可能であることを確認します。

### 端末内にあるインストール済み拡張機能の情報の削除

以下の作業は、ADドメイン参加状態の任意の端末にて、システム管理者または一般ユーザーが一般ユーザーアカウントで実施します。

* 2.1 各端末内にある以下の二つのファイルを削除します。
  * `%LocalAppData%\Microsoft\Edge\User Data\Default\Preferences`
  * `%LocalAppData%\Microsoft\Edge\User Data\Default\Secure Preferences`
* 2.2 Windowsのタスクマネージャーのプロセスタブを開き、実行中の「Microsoft Edge」プロセスがあれば、右クリック→タスクの終了 で終了させます。
* 2.3 Windowsのタスクマネージャーの詳細タブを開き、実行中の「msedge.exe」プロセスがあれば、右クリック→タスクの終了 で終了させます。

### 端末でPrintCanceler拡張機能が切り戻されることの確認

以下の作業はすべて、ADドメイン参加状態の任意の端末にて、システム管理者または一般ユーザーが一般ユーザーアカウントで実施します。

* 3.1 Edgeを起動します。
* 3.2 Edgeのツールバー上にパズルピース型のボタンをクリックし、「拡張機能の管理」をクリックします。
* 3.3 拡張機能の管理画面が開かれますので、「開発者モード」を有効化します。
* 3.4 「インストール済の拡張機能」の見出し横に表示される「更新」ボタンをクリックします。
* 3.5 読み込み中を示すアニメーションが表示されますので、アニメーションが消えて「拡張機能が更新されました」というメッセージが表示されるまで待ちます。
* 3.6 「インストール済の拡張機能」の一覧にある「PrintCanceler」の詳細情報を表示し、バージョンが切り戻し後のバージョンと一致していることを確認します。
* 3.7 「開発者モード」を無効化します。

以上の手順により、組織内サーバーに設置したバージョンのPrintCanceler拡張機能が、各端末に反映されます。

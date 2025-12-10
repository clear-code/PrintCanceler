---
CJKmainfont: Noto Sans CJK JP
CJKoptions:
  - BoldFont=Noto Sans CJK JP Bold
titlepage-logo: ./troubleshooting-guide/media/image1.png
title: |
  PrintCanceler
  トラブルシューティングガイド
subject: PrintCancelerトラブルシューティングガイド
date: 2025/12/10
author: 株式会社クリアコード
keywords: [PrintCanceler, Troubleshooting guide]
titlepage: true
toc-title: 目次
toc-own-page: true
---

更新履歴

| 日付       | Version | 備考                              |
|------------|---------|-----------------------------------|
| 2025/12/10 | 1.0.0   | 第1版                             |

**本書について**

本書は、株式会社クリアコードが、PrintCancelerを御利用いただく管理者向けに作成した資料となります。2025年9月時点のデータにより作成されており、それ以降の状況の変動によっては、本書の内容と事実が異なる場合があります。また、本書の内容に基づく運用結果については責任を負いかねますので、予めご了承下さい。

本書で使用するシステム名、製品名は、それぞれの各社の商標、または登録商標です。なお、本文中ではTM、®、©マークは省略しています。

\newpage

# ログ採取手順

以下の手順でPrintCancelerのログを採取できます。
障害発生時には以下の手順でログを採取いただき、サポート窓口までご送付ください。

## EdgeおよびPrintCancelerのEdge拡張機能のデバッグログの採取手順

1. 起動中のEdgeをすべて終了する
  * 起動中のEdgeを終了する
  * タスクマネージャーを起動し、プロセスタブからMicrosoft Edgeのプロセスが残留していればすべて終了する。
  * タスクマネージャーを起動し、詳細タブからmsedge.exeプロセスが残留していればすべて終了する
2. Edgeのショートカットをデスクトップ又は任意の場所にコピーする。
3. 2のショートカットを右クリックし、メニューから「プロパティ」を選ぶ
4. 「リンク先」の～msedge.exeの後ろに「 --enable-logging -v=1」を付け加える。
  * 編集前「～msedge.exe"」
  * 編集後「～msedge.exe --enable-logging -v=1"」
5. 4で編集したショートカットからEdgeを起動する。
6. この状態で現象を再現させる。
7. Edgeを終了する。
8. `%LocalAppData%\Microsoft\Edge\User Data\chrome_debug.log` を採取する。

## PrintCanceler.exeのログ採取手順

1. `%AppData%\PrintCanceler`配下の以下のファイルを採取する。
  * `PrintCanceler_server.log`
  * `PrintCanceler_server.log.N`（Nは1から10までの整数）
  * `PrintCanceler_client.log`
  * `PrintCanceler_client.log.N`（Nは1から10までの整数）

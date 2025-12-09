# PrintCanceler リリース前検証手順

## 検証環境の用意

検証には原則として `/doc/verify/windows-11-24H2` の環境を使用する。
他の環境を使用する場合は、以下の条件を事前に整えておく。

* Windows 11
* Active Directoryドメイン参加状態である。
  （または、`/doc/verify/windows-11-24H2/ansible/files/join-dummy-domain.bat` の内容を管理者権限で実行済みである。）
* Microsoft Edge 117をインストール済みである。
* Microsoft EdgeのGPO用ポリシーテンプレートを導入済みである。
* `C:\Users\Public\webextensions` 配下に `/webextensions/` の内容を配置済みである。
* `C:\Users\Public\webextensions\manifest.xml` の位置に `/doc/verify/windows-11-24H2/ansible/files/manifest.xml` を配置済みである。

準備は以下の手順で行う。

1. PrintCancelerの最新のインストーラ `PrintCancelerSetup_x64.exe` をダウンロードし、実行、インストールする。
2. 組織内配布用パッケージ作成用CRX署名用秘密鍵を用いて、crxファイルを作成する。
3. 作成したcrxファイルを検証環境の `C:\Users\Public\webextensions\edge.crx` に配置する。
4. Edge拡張機能をインストールするための設定を行う。
   1. `gpedit.msc` を起動する。
   2. `Computer Configuration\Administrative Templates\Microsoft Edge\Extensions`（`コンピューターの構成\管理用テンプレート\Microsoft Edge\拡張機能`）を開いて、以下のポリシーを設定する。
      * `Control which extensions are installed silently`（`サイレント インストールされる拡張機能を制御する`）
        * `Enabled`（`有効`）に設定して、`Show...`（`表示...`）をクリックし、以下の項目を追加する。
          * `mpcpmdhaecmjiaglbkaejhhiipghlfea;file:///C:/Users/Public/webextensions/manifest.xml`
   3. Edgeを起動し、アドオンの管理画面（`edge://extensions`）を開いて、PrintCancelerが管理者によってインストールされた状態になっていることを確認する。


## 検証

* Edgeを起動する。
* `https://example.com/` を開く。
* アドレスバーに `javascript:window.print()` と入力し、Enterキーを押す。
  * 確認
    * [ ] 印刷プレビューのダイアログが開かれ、そのまま維持される。
* 印刷ダイアログをキャンセルして閉じる。
* ページ中の余白を右クリックし、`Print`（`印刷`）を選択する。
  * 確認
    * [ ] 印刷プレビューのダイアログが一瞬表示され、自動的にキャンセルされる。
* アプリケーションメニューを開き、`Print`（`印刷`）を選択する。
  * 確認
    * [ ] 印刷プレビューのダイアログが一瞬表示され、自動的にキャンセルされる。
* PDFファイルのURLを開く。例： `https://www.clear-code.com/koukoku/19.pdf`
* PDFプレビューのツールバーの印刷ボタンをクリックする。
  * 確認
    * [ ] 印刷プレビューのダイアログが開かれ、そのまま維持される。
* 印刷ダイアログをキャンセルして閉じる。
* ページ中の余白を右クリックし、`Print`（`印刷`）を選択する。
  * 確認
    * [ ] 印刷プレビューのダイアログが開かれ、そのまま維持される。
* アプリケーションメニューを開き、`Print`（`印刷`）を選択する。
  * 確認
    * [ ] 印刷プレビューのダイアログが開かれ、そのまま維持される。

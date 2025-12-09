/*
This Source Code Form is subject to the terms of the Mozilla Public
License, v. 2.0. If a copy of the MPL was not distributed with this
file, You can obtain one at http://mozilla.org/MPL/2.0/.

Copyright (c) 2025 ClearCode Inc.
*/
using System;
using System.Diagnostics;
using System.Linq;
using System.Threading.Tasks;
using System.Windows.Automation;
using System.Windows.Forms;
using System.Windows.Forms.VisualStyles;

namespace PrintCanceler
{
    internal static class EdgePrintDialogCanceler
    {
        internal static void WatchDialog(RuntimeContext context)
        {
            AutomationElement desktop = AutomationElement.RootElement;
            while (!context.IsEndTime)
            {
                //Edgeのプロセスのうち、メインウィンドウがあるものに絞り込み
                var edges = Process.GetProcessesByName("msedge").Where(_ => _.MainWindowHandle != IntPtr.Zero);
                foreach (var edge in edges)
                {
                    var targetPid = edge.Id;
                    var windowCondition = new AndCondition(
                        new PropertyCondition(AutomationElement.ProcessIdProperty, targetPid),
                        new PropertyCondition(AutomationElement.ControlTypeProperty, ControlType.Window));
                    var edgeElement = desktop.FindFirst(TreeScope.Children, windowCondition);
                    if (edgeElement == null)
                    {
                        continue;
                    }
                    PrintControlIdentifiers(context, edgeElement, 0);
                    CancelDialog(context, edgeElement);
                }
                Task.Delay(500).Wait();
            }
        }

        [Conditional("DEBUG")]
        internal static void PrintControlIdentifiers(RuntimeContext context, AutomationElement element, int indent)
        {
            try
            {
                var ind = new string(' ', indent * 2);
                Console.WriteLine($"{ind}- {element.Current.ControlType.ProgrammaticName} : {element.Current.Name}");
                var children = element.FindAll(TreeScope.Children, Condition.TrueCondition);
                foreach (AutomationElement child in children)
                {
                    PrintControlIdentifiers(context, child, indent + 1);
                }
            }
            catch (Exception ex)
            {
                context.Logger.Log(ex);
            }
        }

        internal static void CancelDialog(RuntimeContext context, AutomationElement edgeElement)
        {
            try
            {
                var printDialogNameCondition = new OrCondition(
                    new PropertyCondition(AutomationElement.NameProperty, "Print"),
                    new PropertyCondition(AutomationElement.NameProperty, "印刷"));
                var printDialogCondition = new AndCondition(
                    printDialogNameCondition,
                    new PropertyCondition(AutomationElement.ControlTypeProperty, ControlType.Window));
                var printDialogElement = edgeElement.FindFirst(TreeScope.Descendants, printDialogCondition);
                if (printDialogElement == null)
                {
                    return;
                }
                context.Logger.Log($"Found print dialog");
                var cancelButtonNameCondition = new OrCondition(
                    new PropertyCondition(AutomationElement.NameProperty, "Cancel"),
                    new PropertyCondition(AutomationElement.NameProperty, "キャンセル"));
                var cancelButtonCondition = new AndCondition(
                    cancelButtonNameCondition,
                    new PropertyCondition(AutomationElement.ControlTypeProperty, ControlType.Button));
                var cancelButtonElement = printDialogElement.FindFirst(TreeScope.Descendants, cancelButtonCondition);
                if (cancelButtonElement == null)
                {
                    return;
                }
                InvokePattern cancelButton = cancelButtonElement.GetCurrentPattern(InvokePattern.Pattern) as InvokePattern;
                if (cancelButton == null)
                {
                    return;
                }
                cancelButton.Invoke();
                context.Logger.Log($"Dialog canceled");

                // 印刷ダイアログが消えていることを確認する。
                // 最大10秒待つ。
                for (int i = 0; i < 10; i++)
                {
                    printDialogElement = edgeElement.FindFirst(TreeScope.Descendants, printDialogCondition);
                    if (printDialogElement == null)
                    {
                        break;
                    }
                    Task.Delay(1000).Wait();
                }

                if (printDialogElement != null)
                {
                    context.Logger.Log($"Dialog not closed");
                    return;
                }

                // 印刷ダイアログのキャンセルに成功したので、終了時刻を現在時刻より前に設定して、
                // IsEndTimeがtrueを返すようにし、このプロセスをすぐに終了するようにする。
                // （そうしないと、PDFビューワーからの印刷など、beforeprintイベントが発生しない印刷操作が常にキャンセルされてしまう）
                context.FinishTime = DateTime.Now.AddSeconds(-1);

                if (context.Config?.WarningWhenCloseDialog ?? false)
                {
                    context.Logger.Log($"Display warning dialog");
                    ShowWarningDialog();
                }
            }
            catch (Exception ex)
            {
                context.Logger.Log(ex);
            }
        }

        internal static void ShowWarningDialog()
        {
            // メッセージボックスの表示はスレッドをブロックするので、別スレッドで実行する
            Task.Run(() =>
            {
                MessageBox.Show("この場面での印刷は禁止されています。\n\n印刷はキャンセルされました。", "PrintCanceler", MessageBoxButtons.OK, MessageBoxIcon.Warning);
            });

            // メッセージボックスがEdgeの後ろ側に来てしまうことがあるので、強制的にフォーカスする。
            Task.Run(() =>
            {
                Task.Delay(100).Wait();
                AutomationElement desktop = AutomationElement.RootElement;
                var windowCondition = new AndCondition(
                    new PropertyCondition(AutomationElement.ControlTypeProperty, ControlType.Window),
                    new PropertyCondition(AutomationElement.NameProperty, "PrintCanceler"));
                var dialog = desktop.FindFirst(TreeScope.Children, windowCondition);
                dialog?.SetFocus();
            });
        }

    }
}

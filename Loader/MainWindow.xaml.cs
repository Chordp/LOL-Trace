using MahApps.Metro.Controls;
using MahApps.Metro.Controls.Dialogs;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Threading;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.Diagnostics;
using System.IO;
using Microsoft.Win32;
namespace Loader
{

    /// <summary>
    /// MainWindow.xaml 的交互逻辑
    /// </summary>
    public partial class MainWindow : MetroWindow
    {
        public MainWindow()
        {
            InitializeComponent();
        }
        DispatcherTimer timer;
        int CurrentId = 0;
        int InjectCount = 0;
        bool Inject = false;
        ProgressDialogController controller;
        MetroDialogSettings Settings = new MetroDialogSettings
        {
            AffirmativeButtonText = "确定",
            NegativeButtonText = "取消"
            
        };
        private void initTimer()
        {
            timer = new DispatcherTimer();
            timer.Interval = TimeSpan.FromSeconds(1);
            timer.Tick += async (object sender, EventArgs e) => {


                if (controller == null && !Inject)
                {
                    controller = await this.ShowProgressAsync("等待", "等待游戏启动.....",true, Settings);
                    controller.SetIndeterminate();
                }
                if(controller != null)
                {
                    if (controller.IsCanceled)
                    {
                        timer.Stop();
                        await controller.CloseAsync();
                        controller = null;
                    }

                }
                var Proce = Process.GetProcessesByName("League of Legends");
                if (Proce.Length  >=1)
                {
                    var Game = Proce[0];
                    if (CurrentId == Game.Id)
                    {
                        if (controller != null)
                        {
                            await controller.CloseAsync();
                            controller = null;
                        }
                        return;
                    }
                    else
                    {
                        Inject = false;
                    }

                    Debug.WriteLine(System.IO.Path.GetFullPath("LOL Trace.dll"));

                    WinApi.Inject(Game.Id, Encoding.Default.GetBytes(System.IO.Path.GetFullPath("LOL Trace.dll")));
                    Inject = true;
                    CurrentId = Game.Id;
                    InjectCount++;
                    _ = await this.ShowMessageAsync("成功", $"累计给你注入了 {InjectCount} 次!");

                }
                else
                {
                    Inject = false;
                }
                

            };
       
        }
        private void MetroWindow_Initialized(object sender, EventArgs e)
        {
            try
            {
               // initTimer();
                var RegPath = Registry.CurrentUser.OpenSubKey(@"Software\FuckTp",true);
                if(RegPath == null)
                {
                    RegPath =  Registry.CurrentUser.CreateSubKey(@"Software\FuckTp");

                }
                RegPath.SetValue("Path", System.AppDomain.CurrentDomain.BaseDirectory);

                
            }
            catch (Exception)
            {
                Debug.WriteLine("???");
              
            }
        

            //RunProgressFromVm();

        }
        private async void RunProgressFromVm()
        {
            var controller = await this.ShowProgressAsync( "Progress from VM", "Progressing all the things, wait 3 seconds");
            controller.SetIndeterminate();

            //await Task.Delay(3000);

            await controller.CloseAsync();
        }
        private async Task ConfirmShutdown()
        {
            var mySettings = new MetroDialogSettings
            {
                AffirmativeButtonText = "退出",
                NegativeButtonText = "取消",
                AnimateShow = true,
                AnimateHide = false
            };

            var result = await this.ShowMessageAsync("退出应用程序?",
                                                     "您确定要退出应用程序吗?",
                                                     MessageDialogStyle.AffirmativeAndNegative, mySettings);

            if (result == MessageDialogResult.Affirmative)
            {
                Application.Current.Shutdown();
            }
        }


        private void MetroWindow_Closing(object sender, System.ComponentModel.CancelEventArgs e)
        {
            if (e.Cancel) return;
            e.Cancel = true;

            // We have to delay the execution through BeginInvoke to prevent potential re-entrancy
            _ = Dispatcher.BeginInvoke(new Action(async () => await this.ConfirmShutdown()));
        }

        private void Button_Click(object sender, RoutedEventArgs e)
        {

            var DriverLoader = new KernelServiceLoader();
            DriverLoader.SetServiceName("disk");
            DriverLoader.SetOwnership(false);
            //timer.Start();
        }
    }
}

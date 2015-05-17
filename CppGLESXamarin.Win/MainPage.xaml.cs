using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices.WindowsRuntime;
using Windows.Foundation;
using Windows.Foundation.Collections;
using Windows.UI.Core;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;
using Windows.UI.Xaml.Controls.Primitives;
using Windows.UI.Xaml.Data;
using Windows.UI.Xaml.Input;
using Windows.UI.Xaml.Media;
using Windows.UI.Xaml.Navigation;

using CppGLESBridge_Win;

// The Blank Page item template is documented at http://go.microsoft.com/fwlink/?LinkId=234238

namespace CppGLESXamarin.Win
{
    /// <summary>
    /// An empty page that can be used on its own or navigated to within a Frame.
    /// </summary>
    public sealed partial class MainPage : Page
    {
        private GLESSurface gs;

        public MainPage()
        {
            this.InitializeComponent();

            gs = new GLESSurface(swapPanel);

            CoreWindow window = Window.Current.CoreWindow;

            window.VisibilityChanged += (CoreWindow sender, VisibilityChangedEventArgs args) => { gs.OnVisibilityChanged(sender, args); };
            swapPanel.SizeChanged += (object sender, SizeChangedEventArgs e) => { gs.OnSwapChainPanelSizeChanged(sender, e); };
            this.Loaded += (object sender, RoutedEventArgs e) => { gs.OnPageLoaded(sender, e); };
        }
    }
}

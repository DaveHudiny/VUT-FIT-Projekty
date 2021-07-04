using Chleba.ViewModels;
using Chleba.Views;
using System;
using System.Collections.Generic;
using Xamarin.Forms;

namespace Chleba
{
    public partial class AppShell : Xamarin.Forms.Shell
    {
        public AppShell()
        {
            InitializeComponent();
            Routing.RegisterRoute(nameof(CalenderPage), typeof(CalenderPage));
            Routing.RegisterRoute(nameof(Settings), typeof(Settings));
        }

    }
}

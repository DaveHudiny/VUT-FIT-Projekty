using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using Xamarin.Forms;
using Xamarin.Forms.Xaml;

namespace Chleba.Views
{
    [XamlCompilation(XamlCompilationOptions.Compile)]
    public partial class WindowsPhoneSettingsPage : ContentPage
    {
        public WindowsPhoneSettingsPage()
        {
            InitializeComponent();
        }


        private async void Button1_clicked(object sender, EventArgs e)
        {
            await Navigation.PushAsync(new EventDetail(-1, true));
        }

        private async void Button2_clicked(object sender, EventArgs e)
        {
            await Navigation.PushAsync(new EventDetail(-2, true));
        }

        private async void Button3_clicked(object sender, EventArgs e)
        {
            await Navigation.PushAsync(new EventDetail(-3, true));
        }

        private async void Button4_clicked(object sender, EventArgs e)
        {
            await Navigation.PushAsync(new EventDetail(-4, true));
        }
        private async void Button5_clicked(object sender, EventArgs e)
        {
            await Navigation.PushAsync(new EventDetail(-5, true));
        }
        private async void Button6_clicked(object sender, EventArgs e)
        {
            await Navigation.PushAsync(new EventDetail(-6, true));
        }
        private async void Button7_clicked(object sender, EventArgs e)
        {
            await Navigation.PushAsync(new EventDetail(-7, true));
        }
        private async void Button8_clicked(object sender, EventArgs e)
        {
            await Navigation.PushAsync(new EventDetail(-8, true));
        }
        private async void Button9_clicked(object sender, EventArgs e)
        {
            await Navigation.PushAsync(new EventDetail(-9, true));
        }
    }
}
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
    public partial class Settings : ContentPage
    {
        public Settings()
        {
            InitializeComponent();
        }

        private async void EventSettings(object sender, EventArgs e)
        {
            await Navigation.PushAsync(new WindowsPhoneSettingsPage());
        }

        private async void Delete_data(object sender, EventArgs e)
        {
            await Navigation.PushAsync(new Page1());
        }
    }
}
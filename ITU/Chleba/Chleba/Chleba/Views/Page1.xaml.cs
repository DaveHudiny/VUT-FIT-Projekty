using Chleba.Models;
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
    public partial class Page1 : ContentPage
    {
        public Page1()
        {
            InitializeComponent();
        }
        async private void Yes_Button(object sender, EventArgs e)
        {
            Parser.Instance.deleteWholeList();
            await Navigation.PopAsync();
        }

        async private void No_Button(object sender, EventArgs e)
        {
            await Navigation.PopAsync();
        }

        async private void Delete_Everything(object sender, EventArgs e)
        {
            Parser.Instance.deleteWholeList();
            Parser.Instance.deleteTemplates();
            Parser.Instance.createTemplates();
            await Navigation.PopAsync();
        }

        async private void Delete_Templates_Only(object sender, EventArgs e)
        {
            Parser.Instance.deleteTemplates();
            Parser.Instance.createTemplates();
            await Navigation.PopAsync();
        }
    }
}
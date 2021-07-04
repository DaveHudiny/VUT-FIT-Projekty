using Chleba.Models;
using Chleba.ViewModels;
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
    public partial class WindowsPhonePage : ContentPage
    {
        Parser novy;
        List<Event> eventus;
        int ID;
        public WindowsPhonePage()
        {
            InitializeComponent();
            novy = Parser.Instance;
            eventus = novy.getNextNth(DateTime.Now.Year, DateTime.Now.Month, DateTime.Now.Day, DateTime.Now.Hour, 1);
            if (eventus.Count == 0)
            {
                EventButton.Text = "the way is more important than the destination";
            }
            else
            {
                EventButton.Text = eventus[0].name;
                ID = eventus[0].ID;
            }
            
        }

        override protected void OnAppearing()
        {
            eventus = novy.getNextNth(DateTime.Now.Year, DateTime.Now.Month, DateTime.Now.Day, DateTime.Now.Hour, 1);
            if (eventus.Count == 0)
            {
                string[] quotations = new string[3];
                quotations[0]= "the way is more important than the destination";
                quotations[1] = "You are the best";
                quotations[2] = "Follow yourself, not someone else";
                Random randomizer = new Random();
                
                EventButton.Text = quotations[randomizer.Next(0, 3)];

            }
            else
            {
                EventButton.Text = eventus[0].name;
                ID = eventus[0].ID;
            }
            B1.Text = Parser.Instance.getByID(-1).name;
            B2.Text = Parser.Instance.getByID(-2).name;
            B3.Text = Parser.Instance.getByID(-3).name;
            B4.Text = Parser.Instance.getByID(-4).name;
            B5.Text = Parser.Instance.getByID(-5).name;
            B6.Text = Parser.Instance.getByID(-6).name;
            B7.Text = Parser.Instance.getByID(-7).name;
            B8.Text = Parser.Instance.getByID(-8).name;
            B9.Text = Parser.Instance.getByID(-9).name;
        }
        
        private async void EventDetailOpen(object sender, EventArgs e)
        {
            if(eventus.Count==0)
            {
                return;
            }
            await Navigation.PushAsync(new EventDetail(eventus[0].ID));
        }
        private async void Button1_clicked(object sender, EventArgs e)
        {
            await Navigation.PushAsync(new AddEvent(-1));
        }

        private async void Button2_clicked(object sender, EventArgs e)
        {
            await Navigation.PushAsync(new AddEvent(-2));
        }

        private async void Button3_clicked(object sender, EventArgs e)
        {
            await Navigation.PushAsync(new AddEvent(-3));
        }

        private async void Button4_clicked(object sender, EventArgs e)
        {
            await Navigation.PushAsync(new AddEvent(-4));
        }
        private async void Button5_clicked(object sender, EventArgs e)
        {
            await Navigation.PushAsync(new AddEvent(-5));
        }
        private async void Button6_clicked(object sender, EventArgs e)
        {
            await Navigation.PushAsync(new AddEvent(-6));
        }
        private async void Button7_clicked(object sender, EventArgs e)
        {
            await Navigation.PushAsync(new AddEvent(-7));
        }
        private async void Button8_clicked(object sender, EventArgs e)
        {
            await Navigation.PushAsync(new AddEvent(-8));
        }
        private async void Button9_clicked(object sender, EventArgs e)
        {
            await Navigation.PushAsync(new AddEvent(-9));
        }
    }
}
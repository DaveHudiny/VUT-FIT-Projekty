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
    public partial class AddEvent : ContentPage
    {
        Event localEvent;

        public AddEvent(DateTime date)
        {
            InitializeComponent();
            DatePicker.Date = date;
        }

        public AddEvent(int ID)
        {
            InitializeComponent();
            ID_label.Text = ID.ToString();
            this.localEvent = Parser.Instance.getByID(ID);
            DatePicker.Date = DateTime.Today;
            TimePicker.Time = DateTime.Now.TimeOfDay;
            if(this.localEvent==null)
            {
                return;
            }
            if (localEvent.name != null)
                EventNameInput.Text = localEvent.name;
            else EventNameInput.Text = "";
            if (localEvent.eventType != null)
                EventPicker.SelectedItem = localEvent.eventType;
            else EventPicker.SelectedItem = "Custom";
            PriorityPicker.SelectedIndex = localEvent.priority;
            if (localEvent.description != null)
                EventDescription.Text = localEvent.description;
            else EventDescription.Text = "";
            Switcherer.IsToggled = localEvent.isPayment;
            if(localEvent.paymentValue.ToString()!=null)
            {
                EntryAmount.Text = localEvent.paymentValue.ToString();
            }
            if(localEvent.paymentCurrency!= null)
            { 
                CurrencyPicker.SelectedItem = localEvent.paymentCurrency; 
            }
            
        }
        public AddEvent()
        {
            InitializeComponent();
        }
        private async void ButtonFinishAdd(object sender, EventArgs e)
        {
            await Navigation.PopAsync();
        }

        private void Switch_Toggled(object sender, ToggledEventArgs e)
        {
            if (e.Value == false)
            {
                EntryAmount.IsVisible = false;
                CurrencyPicker.IsVisible = false;
            }
            else
            {
                EntryAmount.IsVisible = true;
                CurrencyPicker.IsVisible = true;
            }
        }
    }
}
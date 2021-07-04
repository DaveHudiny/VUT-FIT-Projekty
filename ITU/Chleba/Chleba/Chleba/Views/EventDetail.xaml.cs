using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using Xamarin.Forms;
using Xamarin.Forms.Xaml;
using Chleba.Models;
using Chleba.ViewModels;

namespace Chleba.Views
{
    [XamlCompilation(XamlCompilationOptions.Compile)]
    public partial class EventDetail : ContentPage
    {
        Event localEvent;

        public EventDetail(int ID, bool isTemplate)
        {
            InitializeComponent();
            if(isTemplate)
            {
                ID_label.Text = ID.ToString();
                this.localEvent = Parser.Instance.getByID(ID);
                EventNameInput.Text = localEvent.name;
                DatePicker.IsVisible = false;
                TimePicker.IsVisible = false;
                EventPicker.SelectedItem = localEvent.eventType;
                PriorityPicker.SelectedIndex = localEvent.priority;
                EntryAmount.Text = localEvent.paymentValue.ToString();
                CurrencyPicker.SelectedItem = localEvent.paymentCurrency;
                EventDescription.Text = localEvent.description;
                Switcherer.IsToggled = localEvent.isPayment;
                Remover.IsVisible = false;
            }
            

        }
        public EventDetail(int ID)
        {
            InitializeComponent();
            ID_label.Text = ID.ToString();
            this.localEvent = Parser.Instance.getByID(ID);
            EventNameInput.Text = localEvent.name;
            DatePicker.Date = localEvent.DT;
            TimePicker.Time = localEvent.DT.TimeOfDay;
            EventPicker.SelectedItem = localEvent.eventType;
            EntryAmount.Text = localEvent.paymentValue.ToString();
            CurrencyPicker.SelectedItem = localEvent.paymentCurrency;
            EventDescription.Text = localEvent.description;
            Switcherer.IsToggled = localEvent.isPayment;
            PriorityPicker.SelectedIndex = localEvent.priority;
        }
        public EventDetail()
        {
            InitializeComponent();
            DataFlowViewModel.Instance.ID = -500;
            localEvent = new Event();
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

        async private void Delete_Event(object sender, EventArgs e)
        {
            await Navigation.PopAsync();
        }

        async private void Change_Event(object sender, EventArgs e)
        {
            
            await Navigation.PopAsync();
        }
    }
}
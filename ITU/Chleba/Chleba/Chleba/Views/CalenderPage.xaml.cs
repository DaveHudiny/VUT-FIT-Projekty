using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using Xamarin.Forms;
using Xamarin.Forms.Xaml;
using Chleba.ViewModels;
using Chleba.Models;

namespace Chleba.Views
{
    [XamlCompilation(XamlCompilationOptions.Compile)]
    public partial class CalenderPage : ContentPage
    {
        public CalenderPage()
        {
            InitializeComponent();
            year = DateTime.Now.Year;
            month = DateTime.Now.Month;
            oldDate = DateTime.Today;
        }

        async public void Button_Add(object sender, EventArgs e)
        {
            await Navigation.PushAsync(new AddEvent((DateTime)dater.SelectedDate));
        }



        override protected void OnAppearing()
        {
            if(Eventor.IsVisible)
            {
                DataFlowViewModel.Instance.listViewLoadDay(DataFlowViewModel.Instance.year, DataFlowViewModel.Instance.month, dater.SelectedDate.Value.Day);
            }
            else
            {
                DataFlowViewModel.Instance.listViewLoad(DataFlowViewModel.Instance.year, DataFlowViewModel.Instance.month);
            }
            
            MainListView.ItemsSource = DataFlowViewModel.Instance.list;
        }

        async private void MainListView_ItemTapped(object sender, ItemTappedEventArgs e)
        {
            var Selected = e.Item as ListViewTemplate;
            await Navigation.PushAsync(new EventDetail(DataFlowViewModel.Instance.list[(Selected.OrderNumber)-1].ID));
            ((ListView)sender).SelectedItem = null;
        }

        private DateTime oldDate;
        public void DateChanged(object sender, EventArgs e)
        {

            if(DateTime.Compare(oldDate, (DateTime)dater.SelectedDate.Value)==0)
            {
                DataFlowViewModel.Instance.listViewLoad(DataFlowViewModel.Instance.year, DataFlowViewModel.Instance.month);
                Eventor.IsVisible = false;
                oldDate=oldDate.AddYears(-1000);
                MainListView.ItemsSource = DataFlowViewModel.Instance.list;
                return;
            }
            
            DataFlowViewModel.Instance.listViewLoadDay(DataFlowViewModel.Instance.year, DataFlowViewModel.Instance.month, dater.SelectedDate.Value.Day);
            MainListView.ItemsSource = DataFlowViewModel.Instance.list;
            oldDate = dater.SelectedDate.Value;
                Eventor.IsVisible = true;
            
        }
        int year;
        int month;
        public void MonthChangedLeft(object sender, EventArgs e)
        {
            month--;
            if (month == 0)
            {
                month = 12;
                year--;
            }
            DataFlowViewModel.Instance.listViewLoad(year, month);
            MainListView.ItemsSource = DataFlowViewModel.Instance.list;
            Eventor.IsVisible = false;
        }
        public void MonthChangedRight(object sender, EventArgs e)
        {
            month = (month + 1) % 13;
            if (month == 0)
            {
                month++;
                year++;
            }
            DataFlowViewModel.Instance.listViewLoad(year, month);
            MainListView.ItemsSource = DataFlowViewModel.Instance.list;
            Eventor.IsVisible = false;
        }
    }
}
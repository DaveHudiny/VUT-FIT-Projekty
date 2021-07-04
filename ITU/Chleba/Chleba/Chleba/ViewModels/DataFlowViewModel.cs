using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.ComponentModel;
using System.Runtime.CompilerServices;
using System.Text;
using System.Windows.Input;
using Chleba.Models;
using Xamarin.Forms;

namespace Chleba.ViewModels
{
    public class DataFlowViewModel: INotifyPropertyChanged
    {
        Parser parser;

        private string _actualEvent;
        public string actualEvent { get { return _actualEvent; } 
            set { _actualEvent = value; OnPropertyChanged(); } }
        public int actualEventId { get; set; }
        private DateTime? _date;
        public DateTime? Date
        {
            get
            {
                return _date;
            }
            set
            {
                _date = value;
                OnPropertyChanged(nameof(Date));
            }
        }
        private ObservableCollection<XamForms.Controls.SpecialDate> attendances;
        public ObservableCollection<XamForms.Controls.SpecialDate> Attendances
        {
            get
            {
                return attendances;
            }
            set
            {
                attendances = value;
                OnPropertyChanged(nameof(Attendances));
            }
        }

        public Command DateChosen
        {
            get
            {
                return new Command((obj) =>
                {
                    DateChanged();
                });
            }
        }

        public int month;
        public int year;

        private static readonly DataFlowViewModel instance = new DataFlowViewModel();

       
        static DataFlowViewModel()
        {
        }

        public static DataFlowViewModel Instance
        {
            get
            {
                return instance;
            }
        }

        public DataFlowViewModel()
        {
            parser = Parser.Instance;
            month = DateTime.Now.Month;
            year = DateTime.Now.Year;
            list = new ObservableCollection<ListViewTemplate>();
            Date = DateTime.Now;
            Attendances = new ObservableCollection<XamForms.Controls.SpecialDate>();
            List<Event> pom = parser.getNextNth(DateTime.Today.Year, DateTime.Today.Month, DateTime.Today.Day, DateTime.Now.Hour, 1);
            if(pom.Count>0)
            {
                actualEvent = pom[0].name;
            }
            else
            {
                actualEvent = "the way is more important than the destination";
            }
            listViewLoadDay(year, month, DateTime.Now.Day);
            setButtons();
        }

        public void setButtons()
        {
            B1 = parser.getByID(-1).name;
            B2 = parser.getByID(-2).name;
            B3 = parser.getByID(-3).name;
            B4 = parser.getByID(-4).name;
            B5 = parser.getByID(-5).name;
            B6 = parser.getByID(-6).name;
            B7 = parser.getByID(-7).name;
            B8 = parser.getByID(-8).name;
            B9 = parser.getByID(-9).name;
        }
        public event PropertyChangedEventHandler PropertyChanged;
        protected virtual void OnPropertyChanged([CallerMemberName] string propertyName = null)
        {
            PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(propertyName));
        }
        public ObservableCollection<ListViewTemplate> list 
        { get; set; }

        public List<Event> events { get; set; }
        public List<Event> load(int year, int month)
        {
            return parser.getMonth(year, month);
        }

        public void listViewLoad(int year, int month)
        {
            List<Event> loaded = load(year, month);
            ObservableCollection<XamForms.Controls.SpecialDate> pom = new ObservableCollection<XamForms.Controls.SpecialDate>();
            list.Clear();
            for (int i=0; i<loaded.Count; i++)
            {
                ListViewTemplate addor = new ListViewTemplate();
                addor.EventName=loaded[i].name;
                addor.OrderNumber = i + 1;
                addor.Date = loaded[i].DT.ToString();
                addor.ID = loaded[i].ID;
                XamForms.Controls.SpecialDate aux = new XamForms.Controls.SpecialDate(loaded[i].DT);
                switch(loaded[i].priority)
                {
                    case 0:
                        addor.backgroundColor = Xamarin.Forms.Color.LightGray;
                        break;
                    case 1:
                        addor.backgroundColor = Xamarin.Forms.Color.LightGreen;
                        break;
                    case 2:
                        addor.backgroundColor = Xamarin.Forms.Color.LightYellow;
                        break;
                    case 3:
                        addor.backgroundColor = Xamarin.Forms.Color.Orange;
                        break;
                    case 4:
                        addor.backgroundColor = Xamarin.Forms.Color.Red;
                        break;
                    case 5:
                        addor.backgroundColor = Xamarin.Forms.Color.Purple;
                        break;
                }
                aux.BackgroundColor = Xamarin.Forms.Color.LightGray;
                aux.Selectable = true;
                pom.Add(aux);
                
                list.Add(addor);
            }
            Attendances = pom;
            
        }

        public void listViewLoadDay(int year, int month, int day)
        {
            List<Event> loaded = parser.getDay(year, month, day);
            list.Clear();
            for (int i = 0; i < loaded.Count; i++)
            {
                ListViewTemplate addor = new ListViewTemplate();
                addor.EventName = loaded[i].name;
                addor.OrderNumber = i + 1;
                addor.Date = loaded[i].DT.ToString();
                addor.ID = loaded[i].ID;
                switch (loaded[i].priority)
                {
                    case 0:
                        addor.backgroundColor = Xamarin.Forms.Color.LightGray;
                        break;
                    case 1:
                        addor.backgroundColor = Xamarin.Forms.Color.LightGreen;
                        break;
                    case 2:
                        addor.backgroundColor = Xamarin.Forms.Color.LightYellow;
                        break;
                    case 3:
                        addor.backgroundColor = Xamarin.Forms.Color.Orange;
                        break;
                    case 4:
                        addor.backgroundColor = Xamarin.Forms.Color.Red;
                        break;
                    case 5:
                        addor.backgroundColor = Xamarin.Forms.Color.Purple;
                        break;
                }
                list.Add(addor);
            }
        }
        public void save(string name, string type, DateTime date, int priority, string description, bool isPayment, double payValue, string payCurrency)
        {
            parser.addEvent(name, type, date, priority, description, isPayment, payValue, payCurrency);
        }

        public ICommand MonthChangedLeftCommand => new Command(MonthChangedLeft);
        public ICommand MonthChangedRightCommand => new Command(MonthChangedRight);

        public ICommand DateChangedCommand => new Command(DateChanged);
        public void MonthChangedLeft()
        {
            month--;
            if (month == 0)
            {
                month = 12;
                year--;
            }
            listViewLoad(year, month);
        }
        public void MonthChangedRight()
        {
            month = (month + 1) % 13;
            if (month == 0)
            {
                month++;
                year++;
            }
            listViewLoad(year, month);
        }

        public void DateChanged()
        {
            listViewLoadDay(Date.Value.Year,
                Date.Value.Month, Date.Value.Day);
        }

        private string _name;
        public string name { get { return _name; } set { _name = value; OnPropertyChanged(); } }
        private bool _isToggled;
        public bool isToggled { get { return _isToggled; } set { _isToggled = value; OnPropertyChanged(); } }
        private double _payment;
        public double payment { get { return _payment; } set { _payment = value; OnPropertyChanged(); } }
        private int _priority;
        public int priority { get { return _priority; } set { _priority = value; OnPropertyChanged(); } }
        private string _type;
        public string type { get { return _type; } set { _type = value; OnPropertyChanged(); } }
        private string _currency;
        public string currency { get { return _currency; } set { _currency = value; OnPropertyChanged(); } }

        private DateTime _dateDay;
        public DateTime dateDay { get { return _dateDay; } set { _dateDay = value; OnPropertyChanged(); } }

        private TimeSpan _timeDay;
        public TimeSpan timeDay { get { return _timeDay; } set { _timeDay = value; OnPropertyChanged(); } }
        private int _ID;
        public int ID { get { return _ID; } set { _ID = value; OnPropertyChanged(); } }

        private string _description;
        public string description { get { return _description; } set { _description = value; OnPropertyChanged(); } }

        public ICommand changeEventCommand => new Command(Change_Event);
        public ICommand deleteEventCommand => new Command(Delete_Event);
        public ICommand addEventCommand => new Command(Add_Event);

        public void Change_Event()
        {
            double money = 0;

            if (isToggled)
            {
                money = payment;
            }
            int prior = priority;
            DateTime date = dateDay.Date + timeDay;
            if (ID == -500)
            {
                Parser.Instance.addEvent(name, type, date,
                    prior, description, isToggled, money, currency);
            }
            else
            {
                Parser.Instance.changeEvent(name, type, date,
                    prior, description, isToggled, money, currency, ID);
            }
            setButtons();
            OnPropertyChanged("B1");
        }
        public void Delete_Event()
        {
            if (ID == -500)
            {
                return;
            }
            Parser.Instance.deleteEvent(ID);
            List<Event> pom = parser.getNextNth(DateTime.Today.Year, DateTime.Today.Month, 
                DateTime.Today.Day, DateTime.Now.Hour, 1);
            if (pom.Count > 0)
            {
                actualEvent = pom[0].name;
            }
            else
            {
                actualEvent = "the way is more important than the destination";
            }
            listViewLoad(year, month);
        }

        public void Add_Event()
        {
            double money = 0;
            if(isToggled)
            {
                money = payment;
            }
            DateTime date= dateDay.Date + timeDay;
            int prior = priority;
            Parser.Instance.addEvent(name, type, date,
                    prior, description, isToggled, money, currency);
            listViewLoad(year, month);
            OnPropertyChanged();
        }

        private string _B1;

        public string B1 { get { return _B1; } set { _B1 = value; OnPropertyChanged(); } }

        private string _B2;

        public string B2 { get { return _B2; } set { _B2 = value; OnPropertyChanged(); } }

        private string _B3;

        public string B3 { get { return _B3; } set { _B3 = value; OnPropertyChanged(); } }

        private string _B4;

        public string B4 { get { return _B4; } set { _B4 = value; OnPropertyChanged(); } }

        private string _B5;

        public string B5 { get { return _B5; } set { _B5 = value; OnPropertyChanged(); } }

        private string _B6;

        public string B6 { get { return _B6; } set { _B6 = value; OnPropertyChanged(); } }

        private string _B7;

        public string B7 { get { return _B7; } set { _B7 = value; OnPropertyChanged(); } }

        private string _B8;

        public string B8 { get { return _B8; } set { _B8 = value; OnPropertyChanged(); } }

        private string _B9;

        public string B9 { get { return _B9; } set { _B9 = value; OnPropertyChanged(); } }
    }
}

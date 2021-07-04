using System;
using System.Collections.Generic;
using System.IO;
using Newtonsoft.Json;

namespace Chleba.Models
{
    class Parser
    {

        private static readonly Parser instance = new Parser();

        // Explicit static constructor to tell C# compiler
        // not to mark type as beforefieldinit
        static Parser()
        {
        }

        private Parser()
        {
            loadData();
            

        }

        public static Parser Instance
        {
            get
            {
                return instance;
            }
        }

        public List<Event> al { get; set; } = new List<Event>();
        int UID { get; set; } = 0;


        ~Parser()
        {
            saveData();
        }
        public void loadData()
        {
            string fileName = Path.Combine(Environment.GetFolderPath(Environment.SpecialFolder.LocalApplicationData), "ITUdata.txt");
            if (File.Exists(fileName) && new FileInfo(fileName).Length > 0)
            {
                string json = File.ReadAllText(fileName);
                this.al = JsonConvert.DeserializeObject<List<Event>>(json);
                foreach (Event e in this.al)
                {
                    if (e.ID >= UID)
                        UID = e.ID + 1;
                }
            }
            else
                createTemplates();

        }

        public void createTemplates()
        {
            for (int iii = -1; iii >= -10; iii--)
            {
                al.Add(new Event(iii));
            }
            saveData();
        }

        public void saveData()
        {
            string fileName = Path.Combine(Environment.GetFolderPath(Environment.SpecialFolder.LocalApplicationData), "ITUdata.txt");
            string json = JsonConvert.SerializeObject(this.al);
            File.WriteAllText(fileName, json);
        }

        public void addEvent(string name, string type, DateTime date,
                             int priority, string description, bool isPayment, double payValue,
                             string payCurrency)
        {
            this.al.Add(new Event(name, type, date, priority, description, isPayment, payValue, payCurrency, UID++));
            this.al.Sort((x, y) => DateTime.Compare(x.DT, y.DT));
            this.saveData();
        }


        public void deleteEvent(int ID)
        {
            int foundAt = al.FindIndex((x) => x.ID == ID);
            if (foundAt != -1)
            al.RemoveAt(foundAt);
            this.saveData();
        }
        public List<Event> getMonth(int year, int month)
        {
            List<Event> returnList = new List<Event>();
            foreach (Event e in this.al)
            {
                if (e.DT.Year == year && e.DT.Month == month)
                    returnList.Add(e);
            }

            return returnList;
        }
        public List<Event> getDay(int year, int month, int day)
        {
            List<Event> returnList = new List<Event>();
            foreach (Event e in this.al)
            {
                if (e.DT.Year == year && e.DT.Month == month && e.DT.Day == day)
                    returnList.Add(e);
            }

            return returnList;
        }

        public List<Event> getNextNth(int year, int month, int day, int hour, int n)
        {
            List<Event> returnList = new List<Event>();
            foreach (Event e in this.al)
            {
                if (e.DT.Year >= year && e.DT.Month >= month && e.DT.Day >= day && e.DT.Hour >= hour)
                {
                    returnList.Add(e);
                    --n;
                    if (n == 0)
                        break;
                }
            }
            return returnList;
        }
        public void changeEvent(string name, string type, DateTime date,
                                int priority, string description, bool isPayment, double payValue,
                                string payCurrency, int ID)
        {
            int foundAt = al.FindIndex((x) => x.ID == ID);
            if (foundAt != -1)
            {
                al.RemoveAt(foundAt);
                this.al.Add(new Event(name, type, date, priority, description, isPayment, payValue, payCurrency, ID));
                this.al.Sort((x, y) => DateTime.Compare(x.DT, y.DT));
                this.saveData();
            }
        }
        public Event getByID(int ID)
        {
            Event found = al.Find((x) => x.ID == ID);
            return found;
        }

        public void deleteWholeList()
        {
            for (int iii = 0; iii < al.Count; iii++)
            {
                if (al[iii].ID >= 0)
                {
                    al.RemoveAt(iii);
                    --iii;
                }
            }
        }

        public void deleteTemplates()
        {
            for (int iii = 0; iii < al.Count; iii++)
            {
                if (al[iii].ID < 0)
                {
                    al.RemoveAt(iii);
                    --iii;
                }
            }
        }

    }

    public class Event
    {
        public string name { get; set; } = "Custom Event";
        public string eventType { get; set; } = "";
        public DateTime DT { get; set; } = new DateTime(0);
        public int priority { get; set; } = 0;
        public string description { get; set; } = "";
        public bool isPayment { get; set; } = false;
        public double paymentValue { get; set; } = 0;
        public string paymentCurrency { get; set; } = "EUR";
        public int ID { get; set; } = -99;
        public Event(string name, string type, DateTime date,
                      int p, string desc, bool isPay, double payVal,
                      string payCurr, int ID)
        {
            this.name = name;
            this.eventType = type;
            this.DT = date;
            this.priority = p;
            this.description = desc;
            this.isPayment = isPay;
            this.paymentValue = payVal;
            this.paymentCurrency = payCurr;
            this.ID = ID;

        }
        public Event(int ID)
        {
            this.ID = ID;
        }
        public Event()
        { }

        public void printEvent()
        {
            Console.Write(JsonConvert.SerializeObject(this) + "\n");
        }
    }
}

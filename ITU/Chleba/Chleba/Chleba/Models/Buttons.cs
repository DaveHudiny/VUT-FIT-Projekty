using System;
using System.Collections.Generic;
using System.Text;

namespace Chleba.Models
{
    public class Buttons
    {
        private string[] buttonNames = new string[] {"Custom", "Custom" , "Custom" , "Custom" , "Custom" , "Custom" , "Custom" , "Custom", "Custom" };
        public string getName(int index)
        {
            return buttonNames[index];
        }
        public void changeName(int index, string newName)
        {
            buttonNames[index] = newName;
        }
    }
}

#include <vector>
#include <iostream>
#include <fstream>
#include <string.h>
#include <vector>
#include <algorithm>
using namespace std;
void returnFalse(bool *outFalse)
{
*outFalse = false;
}
void returnTrue(bool *outTrue)
{
*outTrue = true;
}
void And(bool a, bool b, bool c, bool *d)
{
*d = a && b && c;
}
void returnStringAhoj(string *ahoj)
{
*ahoj = "Ahoj\n";
}
void returnStringSvete(string *svet)
{
*svet = "svete\n";
}
void chooseBy(bool boolean, string in1, string in0, string *out)
{
if(boolean){  *out = in0;}else{  *out = in1;}
}
void printString(string input)
{
std::cout << input;
}

int main()
{bool outFalse;
bool outTrue;
bool a;
bool b;
bool c;
bool d;
string ahoj;
string svet;
bool boolean;
string in1;
string in0;
string out;
string input;
returnFalse(&outFalse);
c=outFalse;
returnFalse(&outFalse);
b=outFalse;
returnTrue(&outTrue);
a=outTrue;
And(a, b, c, &d);
boolean=d;
returnStringAhoj(&ahoj);
in1=ahoj;
returnStringSvete(&svet);
in0=svet;
chooseBy(boolean, in1, in0, &out);
input=out;
printString(input);

}
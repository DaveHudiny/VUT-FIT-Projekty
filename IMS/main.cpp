#include <iostream>
#include <fstream>
#include "CA.h"
#include "globals.h"
#include "auxilary.h"


int main(int argc, char *argv[])
{
  using namespace std;
  cout << "Size of map (NxN)" << endl << "N: ";
  unsigned int n;

  cin >> n; 

  cout << "Number of generations to simulate" << endl << "g: ";
  
  unsigned int g;
  cin >> g;

  string answer = "n";
  bool printable = false;

  cout << "Print map?" << endl << "Print [y/n]: ";
  cin >> answer;
  if(answer[0] == 'y')
  {
    printable = true;
  }
  //____________________________________________________________________________________________
  // Base inicializace
  // Inicializace automatu
  CA automat{n};
  // Infikace 0.0747 procent lidi, na kazdy zdroj budou tri jeho sousedi
  automat.infectPercentageInit(0.000747, 3);
  // navakcinuj 80 procent lidi, kazdy má naočkované 3 kámoše
  //- sila vaccinace == 0.9 pro 0.4, 0.7 pro 0.4 a 0.4 pro 0.2 poctu vakcinovanych
  automat.vaccinatePercentageInit(0.80, 3, {0.9,0.7,0.4}, {0.4,0.4,0.2});
  //stejna semantika jako ^^
  automat.immunePercentageInit(0.25, 2, {0.6,0.4}, {0.3,0.7});

  bool run = false;

  do{
    //___________________________________________________________________________________________
    // 
    if (printable){
      std::cout << automat;
    }
    double originalContag = Contagiousness;
    double originalSpreaderProb = SuperSpreaderProb;
    for (unsigned int i=0;i<g;i++){
      automat.step(printable);
      if(InfectedCounter.back()/(n*n) > 0.01)
      {

        Contagiousness -= 0.0065 * NormalDis(mt);
        SuperSpreaderProb -= 0.0055 * NormalDis(mt);
      }
      else
      {
        if(Contagiousness <= originalContag)
        {
          Contagiousness += 0.005;
          
        }
        if(SuperSpreaderProb <= originalSpreaderProb)
        {
          SuperSpreaderProb += 0.005;
        }
      }
    }
    //_____________________________________________________________________________________________
    // zakladni statistiky vypsane na stdout
    
    cout << "Death counts:\n"; 
    for (auto val:DeathCounter){
      cout << val << " ";
    }
    cout << endl;

    cout << "Infected counts:\n";
    for (auto val:InfectedCounter){
      cout << val << " ";
    }
    cout << endl;
    cout << "Symptomless counts:\n";
    for(auto val:NoSymptomCounter){
      cout << val << " ";
    }
    cout << endl;
    cout << "Symptomatic counts:\n";
    for(auto val:SymptomCounter){
      cout << val << " ";
    }
    cout << endl;
    cout << "Risk (can die) counts:\n";
    for(auto val:RiskCounter){
      cout << val << " ";
    }
    cout << endl;
    cout << endl;
    cout << "Do you wish to continue?" << endl << "Continue [y/n]: ";
    cin >> answer;
    if(answer[0] == 'y')
    {
      run = true;
      cout << "How many generations?" << endl << "g: ";
      cin >> g;
    }
    else
    {
      run = false;
    }
    
  }while(run);

  ofstream myfile;
  myfile.open ("statistica.txt");
  myfile << "Deaths Infected" << endl;
  
  for(unsigned int i = 0; i < DeathCounter.size() && i < InfectedCounter.size(); i++)
  {
    myfile << DeathCounter[i] << " " << InfectedCounter[i] << endl;
  }
  myfile.close();
  
  //___________________________________________________________________________________________
  // Zde vypis jak zhruba to decayu podle zadane hodnoty (mortality coef odvozen od infection coef)
  
  // 
  // Human h{};  
  // int timestepDays = 10;
  // h.setImmunCoefs(0.8); // zadavas infection coef, druhy je odvozeny
  // h.setVaccinCoefs(0.95); // ^^
  // for (int i=0;i<10;i++){
  //   std::cout << "Deacay step: " << i << "("<<timestepDays*i<<") days"<<std::endl;
  //   std::cout << "Vacc inf/mort c: " << h.getVaccinInfectionCoef() << "/" 
  //             << h.getImmunInfectionCoef()  << std::endl;
  //   std::cout << "Vacc mor/mort c: " << h.getVaccinMortalityCoef() << "/"
  //             << h.getImmunMortalityCoef()  << std::endl;
  //   std::cout << "____________________________" << std::endl;
  //   h.decayDefense();
  // }
  return 0;
}
#include <vector>
#include <algorithm>
#include <iostream>
#include <string>
#include <iomanip>

#include "globals.h"

bool checkVectorsLenght(std::vector<double> first, std::vector<double> second){
  return (first.size() == second.size());
}

bool summs2one(std::vector<double> toCheck){
  double tmp{};
  for (auto val : toCheck){
    tmp+=val;
  }

  return (tmp>0.98 &&  tmp<1.02);
}

std::vector<int> getShuffledVector(int desiredSize){
  std::vector<int> v(desiredSize);

  for (size_t i = 0; i < v.size(); ++i)
      v[i] = i;

  std::shuffle(v.begin(), v.end(), mt);
  return v;
}


double clip(double n, double lower, double upper) {
  return std::max(lower, std::min(n, upper));
}

template <typename T>
void printStatVector(std::ostream &os, std::string name, std::vector<T> vector){
  os << name << ":" <<std::endl;
  for (int i=0;i<vector.size();i++)
    os  << vector[i] << " ";
  os << std::endl;
}

//cols == true  vypis do sloupcu
//jinak vypis do radku
void printStats(std::ostream &os, bool cols){
  os << std::setprecision(2);
  if (cols){
    os << "Death Infected NoSymptomCounter SymptomCounter RiskCounter MeanVaccInf MeanVaccMort MeanImmInf MeanImmMort TotalVacc TotalImmune\n";
    for (int i=0;i<InfectedCounter.size();i++){
      os << DeathCounter[i] << " "
        << InfectedCounter[i] << " "
        << NoSymptomCounter[i] << " "
        << SymptomCounter[i] << " "
        << RiskCounter[i] << " "
        << MeanVaccinationInfectionCoef[i] << " "
        << MeanVaccinationMortalityCoef[i] << " "
        << MeanImmunityInfectionCoef[i] << " "
        << MeanImmunityMortalityCoef[i] << " "
        << TotalVaccinated[i] << " "
        << TotalImmuned[i] << std::endl;
    }
  }
  else{
    printStatVector<>(os, "Death", DeathCounter);
    printStatVector<>(os, "Infected", InfectedCounter);
    printStatVector<>(os, "NoSymptomCounter", NoSymptomCounter);
    printStatVector<>(os, "SymptomCounter", SymptomCounter);
    printStatVector<>(os, "RiskCounter", RiskCounter);
    printStatVector<>(os, "MeanVaccInf", MeanVaccinationInfectionCoef);
    printStatVector<>(os, "MeanVaccMort", MeanVaccinationMortalityCoef);
    printStatVector<>(os, "MeanImmInf", MeanImmunityInfectionCoef);
    printStatVector<>(os, "MeanImmMort", MeanImmunityMortalityCoef);
    printStatVector<>(os, "TotalVacc", TotalVaccinated);
    printStatVector<>(os, "TotalImmune", TotalImmuned);
  }
}


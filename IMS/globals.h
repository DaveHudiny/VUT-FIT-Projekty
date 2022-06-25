#ifndef GLOBALS_H 
#define GLOBALS_H
#include <random>
#include <vector>

//GLOBALNI PROMENNE AUTOMATU
//Smrtnost celkova pokud neexistuje imunita, vakcinace
extern double Mortality;

//Rychlost upadku ucinku obran v exponencialni casti 
extern double VaccineDecayInfection;
extern double ImmunityDecayInfection;
extern double VaccineDecayMortality;
extern double ImmunityDecayMortality; 

//Rychlost upadku ucinku obran v linearni casti
extern double VaccineLinearDecayInfection;
extern double ImmunityLinearDecayInfection;
extern double VaccineLinearDecayMortality;
extern double ImmunityLinearDecayMortality;

//Bod kdy se z linearni casti stava exponencialni pro upadek obran
extern double VaccineLinearInfectionThreshold;
extern double ImmunityLinearInfectionThreshold;
extern double VaccineLinearMortalityThreshold;
extern double ImmunityLinearMortalityThreshold;

//Sila okolo ktere jsou pocatecni ucinky obran
extern double VaccineStartInfection;
extern double ImmunityStartInfection;
extern double VaccineStartMortality;
extern double ImmunityStartMortality;

//Stredni odchylka toho jaka je pocatecni sila obran
extern double VaccineStartSTD;
extern double ImmunityStartSTD;

//Kdy booster na zaklade obrany proti infekcnosti (zhruba simulace mesice boosteru)
extern double RevaccinationInfectionBasedThreshold;
//Zda se ma automaticky revakcinovat
extern bool AutoRevaccinate;
//Sance ze se revakcinujete v danem kroku -- rychlost ockovacich center
extern double RevaccinationSpeed;

//Nakazlivost nemoci
extern double Contagiousness;
//Sance superspreadra a kolikrat silnejsi maji nakazlivost
extern double SuperSpreaderProb;
extern double SuperSpreaderMultiCoef;

//sance na pridani nahodneho souseda z mapy(trosku breakujeme lokalitu)
extern double RandomNeighbourChance;

// Pomer koeficientu prechodu stage1 nemoce -> stage 2 nemoci vuci stage 2 nemoc->stage 3 nemoc
// S tim ze se stale zachovava mortalita: Mortality*(1-VaccinaMortalCoef)*(1-ImmuneMortalCoef)
extern double InfectionStateTransitionRatio;

//Vektor nakazlivosti dle faze (Celkova nakazlivost se rozlozi)
extern std::vector<double> ContagiousnessByState;
//Rozlozeni pravdepodobnosti pocatecnich fazi nemoci (kolik procent je stage1, stage 2, stage 3)
extern std::vector<double> InitInfectionStateProbDistr;


//GLOBALNI STATISTIKY
// mrtvi, infikovani celkem, bez stage1,2,3 celkem za casovy krok
extern std::vector<int> DeathCounter;
extern std::vector<int> InfectedCounter;
extern std::vector<int> NoSymptomCounter;
extern std::vector<int> SymptomCounter;
extern std::vector<int> RiskCounter;

// Vakcinovani/immunizovani (dle thresholdu nize) celkem v casovem kroku
extern std::vector<int> TotalVaccinated;
extern std::vector<int> TotalImmuned;

extern double TotalStatVaccinationMortalityThreshold;
extern double TotalStatImmunedMortalityThreshold;

// Prumerne koeficienty obran ve spolecnosti v casovem kroku
extern std::vector<double> MeanVaccinationMortalityCoef;
extern std::vector<double> MeanVaccinationInfectionCoef;
extern std::vector<double> MeanImmunityMortalityCoef;
extern std::vector<double> MeanImmunityInfectionCoef;

//RANDOM GENERATORY
extern std::mt19937 mt;
extern std::uniform_real_distribution<> PercentageDis;
extern std::uniform_int_distribution<> IntegerDis;
extern std::normal_distribution<> NormalDis;

#endif
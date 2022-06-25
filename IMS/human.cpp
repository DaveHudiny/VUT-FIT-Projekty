#include <iostream>

#include "human.h"
#include "globals.h"
#include "auxilary.h"

Human::Human(const Human& orig){
  this->m_currentState = orig.m_currentState;
  this->m_nextState = orig.m_nextState;
  this->m_neighbours = orig.m_neighbours;
  this->m_vaccinationInfectionCoef = orig.m_vaccinationInfectionCoef;
  this->m_vaccinationMortalityCoef = orig.m_vaccinationMortalityCoef;
  this->m_immunityMortalityCoef = orig.m_immunityMortalityCoef;
  this->m_immunityInfectionCoef = orig.m_immunityInfectionCoef;
  this->m_contagiousnessCoeff = orig.m_contagiousnessCoeff;
}

void Human::step()
{ 
  // nejdrive udelame nemocenske veci dle casti nemoci (pokud sme)
  if (this->isIll())
  {
    switch (m_currentState){
      case Ill:
        infectedStep(Symptomatic, ContagiousnessByState[0]);
        break;
      case Symptomatic:
        infectedStep(HardCovRisk, ContagiousnessByState[1]);
        break;
      case HardCovRisk:
        for (auto human : this->m_neighbours)
          human->tryInfect(m_contagiousnessCoeff*ContagiousnessByState[2]);
        if (PercentageDis(mt) < Mortality)
          m_nextState = Dead;
        else
          this->getCured();
        break;
      default:
        break;
    }
  }
  // spadnuti obran a revakcinace
  decayDefense();
  if (AutoRevaccinate)
    this->autoRevactination();
}

void Human::autoRevactination(){
  // revakcinujem jen jiz vakcinovane, zdrave a dle rychlosti revakcinace
  if (!this->isIll() &&
      m_vaccinationInfectionCoef < RevaccinationInfectionBasedThreshold && 
      m_vaccinationInfectionCoef > 0 &&
      PercentageDis(mt) < RevaccinationSpeed){
    this->vaccinate(); 
  }
}

void Human::infectedStep(HumanState nextState, double contagiousnessRatio){
  double continueInfectionChance{};
  double deathLoweringCoef = (1-m_immunityMortalityCoef)*(1-m_vaccinationMortalityCoef);
  // pokusime se nakazit okolni + potencialni random kamarady na mape
  for (auto human : this->m_neighbours)
    human->tryInfect(m_contagiousnessCoeff*contagiousnessRatio);
  // vypocitame koeficent potřebný k získaní šance přechodu z 1./2. fáze nemoci na následující
  // aby byla zachovana mortalita: Mortality*(1-immunityMortCoef)*(1-immunityMortCoef)
  // ty take snizuji sanci prechodu do dalsich stavu nemoci
  if(m_currentState == Ill){
    continueInfectionChance = InfectionStateTransitionRatio/(Mortality*std::sqrt(deathLoweringCoef));
  }
  else if(m_currentState == Symptomatic){
    continueInfectionChance = (1/InfectionStateTransitionRatio)/(Mortality*std::sqrt(deathLoweringCoef));
  }
  
  // dopocitani casti spolecne oboum fazim a random
  continueInfectionChance = Mortality*deathLoweringCoef*continueInfectionChance;
  if (PercentageDis(mt) < continueInfectionChance)
    m_nextState = nextState;
  else
    this->getCured();
}

void Human::getCured(){
  // Pokud se vylecime z nemoci ziskame imunitu
  m_nextState = Healthy;
  m_immunityInfectionCoef = ImmunityStartInfection + NormalDis(mt) * ImmunityStartSTD;
  m_immunityMortalityCoef = ImmunityStartMortality + NormalDis(mt) * ImmunityStartSTD;
  m_immunityInfectionCoef = clip(m_immunityInfectionCoef, 0, 0.97);
  m_immunityMortalityCoef = clip(m_immunityMortalityCoef, 0, 0.97);
}


void Human::tryInfect(double exposureCoef)
{
  // infektujem jen pokud to jde
  // vzorecek pro sanci je sila prsku*sila obrany vakcinaci*sila obrany imunitou
  if (this->isInfectable() &&
      PercentageDis(mt) < exposureCoef*(1-m_vaccinationInfectionCoef)*(1-m_immunityInfectionCoef)){
    this->infect();
  }
}

void Human::vaccinate(){
  m_vaccinationInfectionCoef = VaccineStartInfection + NormalDis(mt) * VaccineStartSTD;
  m_vaccinationMortalityCoef = VaccineStartMortality + NormalDis(mt) * VaccineStartSTD;
  m_vaccinationInfectionCoef = clip(m_vaccinationInfectionCoef, 0, 0.97);
  m_vaccinationMortalityCoef = clip(m_vaccinationMortalityCoef, 0, 0.97);
}

void Human::infect(){
  if (isInfectable()){
    this->m_nextState=Ill;
    if (PercentageDis(mt) < SuperSpreaderProb){
      m_contagiousnessCoeff = Contagiousness * SuperSpreaderMultiCoef;
    }
    else{
      m_contagiousnessCoeff = Contagiousness;
    }
  }
}

void Human::infect(HumanState stage){
  if (isInfectable()){
    this->m_nextState=stage;
    if (PercentageDis(mt) < SuperSpreaderProb){
      m_contagiousnessCoeff = Contagiousness * SuperSpreaderMultiCoef;
    }
    else{
      m_contagiousnessCoeff = Contagiousness;
    }
  }
}

bool Human::isInfectable()
{
  return !(this->isIll() || this->isDead());
}

bool Human::statGatherer(std::string data2gather){
  if (!data2gather.compare("Ill"))
    return isIll();
  else if (!data2gather.compare("NoSymptom"))
    return isNoSymptom();
  else if (!data2gather.compare("Symptom"))
    return isSymptom();
  else if (!data2gather.compare("Risk"))
    return isRisk();
  else if (!data2gather.compare("Dead"))
    return isDead();
  std::cout << "err spatna hodnota Human::statGatherer: " << data2gather << std::endl;
  return false;
}

void Human::addNeighbourBidirectional(Human *newNeighbour)
{
  m_neighbours.push_back(newNeighbour);
  newNeighbour->addNeighbourUnidirection(this);
}

void Human::setImmunCoefs(double newCoef){
  newCoef = clip(newCoef, 0.01, 0.97);
  m_immunityInfectionCoef = newCoef;
  int count{};
  // dopocitame kolik kroku by u pocatecnim nastaveni immunityInfection ubehlo
  // abychom ziskali newCoef, tolik kroku aplikujeme na immunityMortality s standartni pocatecni hodnotou
  if (newCoef > ImmunityLinearInfectionThreshold){
    count = ceil((newCoef - ImmunityStartInfection)/ImmunityLinearDecayInfection);
  }
  else{
    count = ceil((ImmunityStartInfection - ImmunityLinearInfectionThreshold)/ImmunityLinearDecayInfection);
    count += ceil(log(newCoef/ImmunityLinearInfectionThreshold)/log(ImmunityDecayInfection));
  }
  m_immunityMortalityCoef = ImmunityStartMortality;
  for (; count>0;count--)
    decayImmunityMortality();
}

void Human::setVaccinCoefs(double newCoef){
  newCoef = clip(newCoef, 0.01, 0.97);
  m_vaccinationInfectionCoef = newCoef;
  int count{};
  // dopocitame kolik kroku by u pocatecnim nastaveni vaccinationInfection ubehlo
  // abychom ziskali newCoef, tolik kroku aplikujeme na vaccinationMortality s standartni pocatecni hodnotou
  if (newCoef > VaccineLinearInfectionThreshold){
    count = ceil((newCoef - VaccineStartInfection)/VaccineLinearDecayInfection);
  }
  else{
    count = ceil((VaccineStartInfection - VaccineLinearInfectionThreshold)/VaccineLinearDecayInfection);
    count += ceil(log(newCoef/VaccineLinearInfectionThreshold)/log(VaccineDecayInfection));
  }
  m_vaccinationMortalityCoef = VaccineStartMortality;
  for (; count>0;count--)
    decayVaccinationMortality();
}

int Human::spreadInfection2NeigboursGuaranted(int count)
{
  int countSpread{};
  for (auto human : m_neighbours)
  {
    if (!human->isIll() && !(human->m_nextState == Ill))
    {
      human->infect(m_nextState);
      countSpread++;
      if (countSpread == count)
        break;
    }
  }
  return count - countSpread;
}

int Human::spreadImmun2Neighbours(int count)
{
  int countSpread{};
  double normalSTD = ImmunityStartSTD;
  for (auto human : m_neighbours)
  {
    if (human->getImmunInfectionCoef() == 0)
    {
      human->setImmunCoefs(m_immunityInfectionCoef+NormalDis(mt)*normalSTD);
      countSpread++;
      if (countSpread == count)
        break;
    }
  }
  return count - countSpread;
}
int Human::spreadVaccine2Neighours(int count)
{
  int countSpread{};
  double normalSTD = VaccineStartSTD;
  for (auto human : m_neighbours)
  {
    if (human->getVaccinInfectionCoef() == 0)
    {
      human->setVaccinCoefs(m_vaccinationInfectionCoef+NormalDis(mt)*normalSTD);
      countSpread++;
      if (countSpread == count)
        break;
    }
  }
  return count - countSpread;
}


int Human::spread(std::string stuff2spread, int count)
{
  if (!stuff2spread.compare("infection"))
    return spreadInfection2NeigboursGuaranted(count);
  else if (!stuff2spread.compare("immunity"))
    return spreadImmun2Neighbours(count);
  else if (!stuff2spread.compare("vaccination"))
    return spreadVaccine2Neighours(count);
  else{
    std::cout << stuff2spread << " isn't valit variable to spread" << std::endl;
    return 1;
  }
};


void Human::decayDefense(){
  this->decayVaccinationInfecion();
  this->decayVaccinationMortality();
  this->decayImmunityInfecion();
  this->decayImmunityMortality();
}

void Human::decayVaccinationInfecion(){
  if (m_vaccinationInfectionCoef > VaccineLinearInfectionThreshold)
    m_vaccinationInfectionCoef -= VaccineLinearDecayInfection;
  else 
    m_vaccinationInfectionCoef *= VaccineDecayInfection;
}
void Human::decayVaccinationMortality(){
  if (m_vaccinationMortalityCoef > VaccineLinearMortalityThreshold)
    m_vaccinationMortalityCoef -= VaccineLinearDecayMortality;
  else 
    m_vaccinationMortalityCoef *= VaccineDecayMortality;
}
void Human::decayImmunityInfecion(){
  if (m_immunityInfectionCoef > ImmunityLinearInfectionThreshold)
    m_immunityInfectionCoef -= ImmunityLinearDecayInfection;
  else 
    m_immunityInfectionCoef *= ImmunityDecayInfection;
}
void Human::decayImmunityMortality(){  
  if (m_immunityMortalityCoef > ImmunityLinearMortalityThreshold)
    m_immunityMortalityCoef -= ImmunityLinearDecayMortality;
  else 
    m_immunityMortalityCoef *= ImmunityDecayMortality;
}



std::ostream& operator<<(std::ostream& os, const Human& human){
  switch (human.m_currentState){
    case Healthy:
      os << "H";
      break;
    case Dead:
      os << "D";
      break;
    case Ill:
      os << "1";
      break;
    case Symptomatic:
      os << "S";
      break;
    case HardCovRisk:
      os << "X";
      break;
  }
  return os;
}
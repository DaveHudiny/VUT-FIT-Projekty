#include <iostream>
#include <algorithm> 
#include <vector>    
#include <cstdio>   
#include <string>

#include "CA.h"
#include "globals.h"
#include "auxilary.h"

CA::CA(unsigned int size): m_size(size){
  // vytvoreni pole
  m_data.resize(size);
  for (auto &data_row: m_data){
    data_row.resize(size);
  }
  // propojeni buněk pole
  connectMoore();
  connectRandom();
}

void CA::connectMoore(){
  Human *first, *second, *third, *fourth;
  int row_up, row_down, col_right;
  for (int col=0; col<m_size; col++){
    for (int row=0; row<m_size; row++){
      if (row==0)
        row_up = m_size-1;
      else
        row_up = row-1;
      if (row==m_size-1)
        row_down = 0;
      else
        row_down = row+1;
      if (col==m_size-1)
        col_right = 0;
      else
        col_right = col+1;

      // najdu si všechny sousedy napravo ode mě a jednoho nahoru ode mě
      // s těmi se propojím oběma směry(já na ně oni na mě) -> 
      // napojení dole a vlevo je tak implicitní
      first = &(m_data[row_up][col]);
      second = &(m_data[row_up][col_right]);
      third = &(m_data[row][col_right]);
      fourth = &(m_data[row_down][col_right]);
      
      m_data[row][col].addNeighbourBidirectional(first);
      m_data[row][col].addNeighbourBidirectional(second);
      m_data[row][col].addNeighbourBidirectional(third);
      m_data[row][col].addNeighbourBidirectional(fourth);
    }
  }
}

void CA::connectRandom(){
  int randHumanIdx, firstIdx, secondIdx;
  for (int col=0; col<m_size; col++){
    for (int row=0; row<m_size; row++){
      // pokud padne dobře náhode tak se stále znova zkouší
      // člověk napojit na dalšího obousměrně
      while (PercentageDis(mt)<RandomNeighbourChance){
        randHumanIdx = IntegerDis(mt);
        if (randHumanIdx >= m_size*m_size)
          randHumanIdx = m_size*m_size-1;
        firstIdx = randHumanIdx/m_size;
        secondIdx = randHumanIdx%m_size;
        
        m_data[row][col].addNeighbourBidirectional(&(m_data[firstIdx][secondIdx]));
      }
    }
  }
}

void CA::step(bool verbose){
  //update stavu automatu lidi
  for (auto &data_row: m_data){
    for (auto &human: data_row){
      human.update_state();
    }
  }
  //provedeni kroku automatu infekci/umrti etc.
  for (auto &data_row: m_data){
    for (auto &human: data_row){
      human.step();
    }
  }
  if (verbose){
    std::cout << *this;
  }
  this->gatherStatistics();
}

void CA::gatherStatistics(){
  gatherSingleStat(InfectedCounter, "Ill");
  gatherSingleStat(NoSymptomCounter, "NoSymptom");
  gatherSingleStat(SymptomCounter, "Symptom");
  gatherSingleStat(RiskCounter, "Risk");
  gatherSingleStat(DeathCounter, "Dead"); 
  gatherVaccinationStats();
  gatherImmunityStats();
}

void CA::gatherSingleStat(std::vector<int> &statVector, std::string stat2gather){
  int counter{};
  for (auto &data_row: m_data){
    for (auto &human: data_row){
      if (human.statGatherer(stat2gather))
        counter++;
    }
  }
  statVector.push_back(counter);
}

void CA::gatherVaccinationStats(){
  int counter{};
  double vaccinationInfectionCoef, vaccinationMortalityCoef;
  double sumInf{}, sumMort{};
  for (auto &data_row: m_data){
    for (auto &human: data_row){
      // pridam hodnoty do summatoru na meany, a zkusim jestli nahodou
      // neni clovek vakcinovany
      vaccinationInfectionCoef = human.getVaccinInfectionCoef();
      vaccinationMortalityCoef = human.getVaccinMortalityCoef();
      sumInf += vaccinationInfectionCoef;
      sumMort += vaccinationMortalityCoef;
      if (vaccinationMortalityCoef > TotalStatVaccinationMortalityThreshold)
        counter++;
    }
  }
  MeanVaccinationInfectionCoef.push_back(sumInf/(m_size*m_size));
  MeanVaccinationMortalityCoef.push_back(sumMort/(m_size*m_size));
  TotalVaccinated.push_back(counter);
}

void CA::gatherImmunityStats(){
  int counter{};
  double immunityInfectionCoef, immunityMortalityCoef;
  double sumInf{}, sumMort{};
  for (auto &data_row: m_data){
    for (auto &human: data_row){
      // pridam hodnoty do summatoru na meany, a zkusim jestli nahodou
      // neni clovek immunni
      immunityInfectionCoef = human.getImmunInfectionCoef();
      immunityMortalityCoef = human.getImmunMortalityCoef();
      sumInf += immunityInfectionCoef;
      sumMort += immunityMortalityCoef;
      if (immunityMortalityCoef > TotalStatImmunedMortalityThreshold)
        counter++;
    }
  }
  MeanImmunityInfectionCoef.push_back(sumInf/(m_size*m_size));
  MeanImmunityMortalityCoef.push_back(sumMort/(m_size*m_size));
  TotalImmuned.push_back(counter);
}

void CA::vaccinatePercentageInit(double percentage, int spreadCoeff, std::vector<double>coeffs2set, std::vector<double>percentagePerCoeff){
  if (!checkVectorsLenght(coeffs2set,percentagePerCoeff)||!summs2one(percentagePerCoeff)){
    std::cout << "Vektor koeficientu a procent nemaji stejnou velikost, nebo procenta nescitaji do 1" << std::endl;
    return;
  }
  int totalPeople = m_size*m_size;
  int normalSTD = VaccineStartSTD;
  
  //ziskani vektoru ma nahodne prohazene cisla od 0 do totalPeople -> random unique cisla
  std::vector<int> v = getShuffledVector(totalPeople);
  
  int count2vaccinate, totalVaccinated{};

  int randIdxFirst, randIdxSecond;
  for (int coefCounter=0; coefCounter < coeffs2set.size(); coefCounter++){
    // pro kazdou intenzitu koeficientu vypocitam kolik lidi navakcinovat + si pamatuju
    // kolik jsem vakcinoval celkem
    count2vaccinate = totalPeople*percentage*percentagePerCoeff[coefCounter];
    if (spreadCoeff != 0)
      count2vaccinate /= (spreadCoeff+1);
    
    totalVaccinated += count2vaccinate;

    // vakcinuji nahodne lidi
    for (int i=0;i<count2vaccinate;i++){
      randIdxFirst = v[i] / m_size;
      randIdxSecond = v[i] % m_size;
      m_data[randIdxFirst][randIdxSecond].setVaccinCoefs(coeffs2set[coefCounter]+NormalDis(mt)*normalSTD);
    }
    
  }
  // rozsir na okoli
  if (spreadCoeff != 0)
    spreadVariable(v, totalVaccinated, spreadCoeff, "vaccination");
}

void CA::immunePercentageInit(double percentage, int spreadCoeff, std::vector<double>coeffs2set, std::vector<double>percentagePerCoeff){
  if (!checkVectorsLenght(coeffs2set, percentagePerCoeff)||!summs2one(percentagePerCoeff)){
    std::cout << "Vektor koeficientu a procent nemaji stejnou velikost, nebo procenta nescitaji do 1" << std::endl;
    return;
  }
  int totalPeople = m_size*m_size;
  int normalSTD = ImmunityStartSTD;
  
  //ziskani vektoru ma nahodne prohazene cisla od 0 do totalPeople -> random unique cisla
  std::vector<int> v = getShuffledVector(totalPeople);
  
  int count2immune, totalImmuned{};

  int randIdxFirst, randIdxSecond;
  for (int coefCounter=0; coefCounter < coeffs2set.size(); coefCounter++){
    // pro kazdou intenzitu koeficientu vypocitam kolik lidi immunizovat + si pamatuju
    // kolik jsem immunizoval celkem
    count2immune = totalPeople*percentage*percentagePerCoeff[coefCounter];
    if (spreadCoeff != 0)
      count2immune /= (spreadCoeff+1);

    totalImmuned += count2immune;

    // imunizace nahodnych lidi
    for (int i=0;i<count2immune;i++){
      randIdxFirst = v[i] / m_size;
      randIdxSecond = v[i] % m_size;
      m_data[randIdxFirst][randIdxSecond].setImmunCoefs(coeffs2set[coefCounter]+NormalDis(mt)*normalSTD);
    }
    // rozsir na okoli
    
  }
  if (spreadCoeff != 0)
    spreadVariable(v, totalImmuned, spreadCoeff, "immunity");
  
}

void CA::infectPercentageInit(double percentage, int spreadCoeff){
  int totalPeople = m_size*m_size;
  
  //ziskani vektoru ma nahodne prohazene cisla od 0 do totalPeople -> random unique cisla
  std::vector<int> v(getShuffledVector(totalPeople));

  //vypocitam si kolik lidi nakazit
  int count2infect = totalPeople*percentage;
  if (spreadCoeff != 0)
      count2infect /= (spreadCoeff+1);

  int randIdxFirst, randIdxSecond;
  double randomPercentage;
  for (int i=0;i<count2infect;i++){
    randIdxFirst = v[i] / m_size;
    randIdxSecond = v[i] % m_size;
    // nakazim lidi ruznyma fazema nemoci dle globalniho prob disr. vektoru
    randomPercentage = PercentageDis(mt);
    if (randomPercentage < InitInfectionStateProbDistr[0])
      m_data[randIdxFirst][randIdxSecond].infect(Ill);
    else if (InitInfectionStateProbDistr[0] <= randomPercentage && 
             randomPercentage < 1-InitInfectionStateProbDistr[2])
      m_data[randIdxFirst][randIdxSecond].infect(Symptomatic);
    else 
      m_data[randIdxFirst][randIdxSecond].infect(HardCovRisk);
  }
  //rozir na okoli
  if (spreadCoeff != 0)
    spreadVariable(v, count2infect, spreadCoeff, "infection");

}

void CA::spreadVariable(std::vector<int> &shuffledVector, int countSources, int spreadCoeff, std::string variable){
  int randIdxFirst, randIdxSecond;
  int extra2spread{};
  // nejdriv zkousime rozsirit na okoli presne spreadCoeff
  for (int i=0;i<countSources;i++){
    randIdxFirst = shuffledVector[i] / m_size;
    randIdxSecond = shuffledVector[i] % m_size;
    extra2spread += m_data[randIdxFirst][randIdxSecond].spread(variable, spreadCoeff);
  }

  // Mnozstvi lidi ktere sme meli priradit ale nesli, zkusime rozsirit znovu, k jinym zdrojum
  for (int i=0;i<countSources;i++){
    randIdxFirst = shuffledVector[i] / m_size;
    randIdxSecond = shuffledVector[i] % m_size;
    if (extra2spread == 0)
      break;
    extra2spread = m_data[randIdxFirst][randIdxSecond].spread(variable, extra2spread);
  }
}


void CA::printVaccinationMap(){
  for (auto &human_row: m_data){
    for (auto &human: human_row){
      printf("%.2f ", human.getVaccinInfectionCoef());
    }
    printf("\n");
  }
  printf("_______________________________________\n");
}

void CA::printImmunityMap(){
  for (auto &human_row: m_data){
    for (auto &human: human_row){
      printf("%.2f ", human.getImmunInfectionCoef());
    }
    printf("\n");
  }
  printf("_______________________________________\n");
}

std::ostream& operator<<(std::ostream& os, const CA& automat)
{
    // os << "Automat is of shape "<<automat.m_size<<"x"<< automat.m_size<< std::endl;
    // os << "Automat state: " << std::endl;
    for (auto &data_row: automat.m_data){
      for (auto &human: data_row){
        os << human<< "";
      }
      os << std::endl;
    }
    os << std::endl << "________________KONEC GENERACE_______________" 
       << std::endl << std::endl;
    return os;
}



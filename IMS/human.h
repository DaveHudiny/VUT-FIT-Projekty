#ifndef HUMAN_H
#define HUMAN_H

#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <string>
#include <cmath>

#include "globals.h"

// enum stavu tridy Human Zdravy, 3 faze nemoci a smrt
enum HumanState{Healthy, Ill, Symptomatic, HardCovRisk, Dead};


class Human
{

  
  protected:
    // Soucasny a dalsi stav + seznam sousedu
    HumanState m_currentState{Healthy};
    HumanState m_nextState{Healthy};
    
    // koeficienty obran
    double m_vaccinationInfectionCoef{};
    double m_immunityInfectionCoef{};
    double m_vaccinationMortalityCoef{};
    double m_immunityMortalityCoef{};

    // jak infekcni sme pokud sme nakazeni
    double m_contagiousnessCoeff{};

    // vektor sousedu (primarne moore, ale muze byt i nejaky random v CA)
    std::vector<Human *> m_neighbours{};
    
    // Pridani do seznamu sousedu, bez zpetne vazby (druhy si me nepridava do sveho)
    void addNeighbourUnidirection(Human *newNeighbour){m_neighbours.push_back(newNeighbour);}

  public:
    Human(){}
    Human(const Human& orig);

    void update_state(){m_currentState = m_nextState;}

    // Krok cloveka -> pokud sme nemocni tak zkousime nakazit sousedy a zjistujeme, 
    // jestli jdeme do dalsiho stavu nemoci/zemreme
    // Dale revakcinace podle globalnich promennych a snizeni obran
    void step();
    // Cast kroku kdy sme infikovani a v prvnim a druhem stavu nemoci
    void infectedStep(HumanState nextState, double contagiousnessRatio);
    // Vola se pokud je Global autorevakcinace True -> zkontroluje podminky pro revakcinace 
    // a pripadne provede
    void autoRevactination();

    // Decay funkce pro obranne slozky, aby sme nebyli imunni navzdy
    void decayDefense();
    void decayVaccinationInfecion();
    void decayVaccinationMortality();
    void decayImmunityInfecion();
    void decayImmunityMortality();
    
    // pomocne dotazy na stav, pro prehlednejsi kod
    bool isIll(){return (m_currentState==Ill||m_currentState==Symptomatic||m_currentState==HardCovRisk);}
    bool isNoSymptom(){return m_currentState==Ill;}
    bool isSymptom(){return m_currentState==Symptomatic;}
    bool isRisk(){return m_currentState==HardCovRisk;}
    bool willBeIll(){return m_nextState==Ill;}
    bool isDead(){return m_nextState==Dead;}
    bool isInfectable();

    // Jednoduchy interface pro nabrani jakekoliv isSomething statistiky
    bool statGatherer(std::string data2gather);

    // Vola soused na nas, preda nam jak moc "prskl" -> my se muzeme nakazit
    // sance k nakazeni je vypoctena z obran a prsknuti
    void tryInfect(double exposureCoef);
    

    // Pridani do seznamu sousedu, ten si nas prida zpatky
    void addNeighbourBidirectional(Human *newNeighbour);

    // Navakcinuj cloveka
    void vaccinate();

    // Infikuje/infikuje rovnou urcitou casti nemoci(druha moznost uzita pri inicializaci)
    void infect();
    void infect(HumanState stage);
    // Zabije - uzito pri inicializaci automatu
    void kill(){this->m_nextState=Dead;}
    // Vyleci nemocneho (i zdraveho a mrtveho, ale na ty se to nikdy nezavola :D)
    void getCured();

    // Nastavi infekcni obranu na novy koeficient a odvodi obranu proti smrti
    void setImmunCoefs(double newCoef);
    void setVaccinCoefs(double newCoef);

    //gettery na statistiky a zjisteni zda je jedinec imunni/vakcinovany
    double getImmunInfectionCoef(){return m_immunityInfectionCoef;}
    double getVaccinInfectionCoef(){return m_vaccinationInfectionCoef;}
    double getImmunMortalityCoef(){return m_immunityMortalityCoef;}
    double getVaccinMortalityCoef(){return m_vaccinationMortalityCoef;}

    // funkce uccine 100% nakazi/immunuje/vakcinuje pocet:count v okoli
    // pokud nebylo dost v okoli + nebo random kamarad na mape k aplikaci imm/vakc/nakaz 
    // vrati pocet, ktery se nepodarilo nakazit
    int spreadImmun2Neighbours(int count);
    int spreadVaccine2Neighours(int count);
    int spreadInfection2NeigboursGuaranted(int count);
    // rozhrani abychom mohli vyuzit cokoliv vyse, dle jmena
    // citelnejsi kod v CA
    int spread(std::string stuff2spread, int count);
    // vypis cloveka 
    // H-zdravy, 1 - nemocny, S - stage 2 nemocny, X - stage 3 nemocny D - mrtvy
    friend std::ostream& operator<<(std::ostream& os, const Human& human);
};

#endif
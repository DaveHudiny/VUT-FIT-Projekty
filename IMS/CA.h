#ifndef CA_H 
#define CA_H

#include <cmath>
#include <iostream>
#include <vector>
#include <string>

#include "human.h"
#include "globals.h"


class CA 
{
  typedef std::vector<std::vector<Human>> HumanArr;
  protected:
    int m_size;
    HumanArr m_data;
  public:
    // Vytvori CA pole lidi size x size velke
    CA(unsigned int size);
    // propoji moorovo okoli
    void connectMoore();
    // napoji random dle globalni promenne pravdepodobnosti
    // funguje jako while(random<pravdepodobnost)pridej random cloveka -> jeden muze mit vic random sousedu
    void connectRandom();

    // Inicializacni funkce funguji na CELKOVĚ percentage cast populace. Přidaná lokalita pomocí spread koef => nejdříve se vakcinuje/nakazi/immunizuje
    // menší část zdrojů a ti pak nakazí spreadCoeff kamarádu, aby dohromady dali Percentage část populace.
    // Coeffs2set a percentage per coeff říkají sílu koeficientů k aplikaci a jaké části z postižených lidí to přiřadit (část vakcinovaných 
    // má slabší koeficient protože se vakcinovali už třeba v lednu)
    void vaccinatePercentageInit(double percentage, int spreadCoeff, std::vector<double>coeffs2set, std::vector<double>percentagePerCoeff);
    void immunePercentageInit(double percentage, int spreadCoeff, std::vector<double>coeffs2set, std::vector<double>percentagePerCoeff);
    void infectPercentageInit(double percentage, int spreadCoeff);

    // rozšíři vakcinaci/imun/inf, do okolí zdrojů, funkce využitá v ^^,
    // náhodný vektor je vektor využitý ve funkcích výše k tvorbě zdrojů
    // variable určuje co šířit (nakaž,imun,vakcinace)
    void spreadVariable(std::vector<int> &shuffledVector, int sourcesCount, int spreadCoeff, std::string variable);
    
    // updatuje lidi do noveho kroku a pak necha kazdeho cloveka provest krok simulace
    // nakonec sbira statistiky
    void step(bool verbose);

    // Nataha statistiky z CA do globalnich promennych
    void gatherStatistics();
    void gatherSingleStat(std::vector<int> &statVector, std::string stat2gather);
    void gatherVaccinationStats();
    void gatherImmunityStats();
    
    // Funkce na vymalovani ruznych promennych automatu jako 2d pole
    void printImmunityMap();
    void printVaccinationMap();
    friend std::ostream& operator<<(std::ostream& os, const CA& automat);
};

#endif
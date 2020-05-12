#ifndef __GRAPHE__
#define __GRAPHE__

#include <vector>
#include <string>
#include "Sommet.hpp"
#include "Arete.hpp"
#include <map>

class Graphe
{
private:
    std::vector<Sommet *> m_sommets;
    std::vector<Arete *> m_base_arretes;
    std::vector<Arete *> m_aretes;
    bool m_orientation;
    int m_ordre;
    int m_taille;

    float calculerIntermediarite(Sommet *si, Sommet *sj, Sommet *sk);

    int fact(int valeur)
    {
        int resultatFact = 1;
        for (int i = 1; i <= valeur; i++)
        {
            resultatFact *= i;
        }
        return resultatFact;
    }

    void reset_arete()
    {
        for (auto arete : m_aretes)
            SupprimerArete(arete->GetIndice());

        for (auto arete : m_base_arretes)
            m_aretes.push_back(new Arete(arete->GetIndice(), arete->GetSommet1(), arete->GetSommet2()));
    }

public:
    Graphe(std::string const &grapheFichier, std::string const &ponderationFicher = " ");
    ~Graphe();

    void Afficher() const;
    void Dessiner() const;

    void AnalyserCentralite();
    int BFS(int num_s0) const;
    std::map<std::pair<int, int>, std::pair<int, std::vector<int>>> Dijkstra(int sommet_s0, bool print = false); /* (sommet_s0, sommet_final) - > (poids,chemins) */
    std::pair<int, std::vector<int>> Dijkstra(int sommet_s0, int sommet_final);

    void AnalyseVulnerabilite(std::vector<int> const &indices_a_supprimer);
    void SupprimerArete(int indice);
    bool AnalyserConnexite() const;
    int CalculerPCC(int sommet_s0, int sommet_final, int sommetFreq, int *outFreq);
    int CalculerKConnexite();
    float CalculerInterm(int indice_sommet);
    Sommet *GetSommet(int indice) const;
    Arete *GetArete(int indice_s1, int indice_s2) const;
    void Colorer();
};

#endif

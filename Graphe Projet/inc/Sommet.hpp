#ifndef __SOMMET__
#define __SOMMET__

#include <vector>
#include <string>

#include "Point.hpp"
#include "Centralite.hpp"

class Sommet
{

private:
    int m_indice;
    std::string m_nom;
    Point m_position;
    std::vector<const Sommet *> m_sommets;
    std::string m_couleur;

    float m_centraliteDeg;
    float m_centraliteVec;
    float m_centraliteProxi;
    float m_centraliteInter;

public:
    Sommet(int m_indice, std::string const &nom, Point const &position);

    int GetIndice() const;
    std::string const &GetNom() const;
    const std::string &GetCouleur() const;
    const std::vector<const Sommet *> &GetSuccesseurs() const;
    const Point &GetPosition() const;
    int GetDegre() const;

    float GetCentraliteDeg() const;
    float GetCentraliteVec() const;
    float GetCentraliteProxi() const;
    float GetCentraliteInter() const;
    Centralite GetCentralite() const;

    bool EstAdjacentA(Sommet *sommet) const;

    void SetCouleur(std::string const &couleur);
    void SetCentraliteInter(float centraliteInter);
    void SetCentraliteDeg(float centraliteDeg);
    void SetCentraliteVec(float centraliteVec);
    void SetCentraliteProxi(float centraliteProxi);

    void AjouterSuccesseur(Sommet *sommet);
    void SupprimerSuccesseur(Sommet *sommet);
    void Afficher() const;
};

#endif

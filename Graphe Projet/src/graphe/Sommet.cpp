#include "Sommet.hpp"
#include "Logger.hpp"
#include <algorithm>

Sommet::Sommet(int indice, std::string const &nom, Point const &position) : m_indice(indice), m_nom(nom), m_position(position), m_couleur("red"), m_centraliteVec(1.0f), m_centraliteDeg(0.0f), m_centraliteInter(0.0f), m_centraliteProxi(0.0f) {}

int Sommet::GetIndice() const { return m_indice; }

std::string const &Sommet::GetNom() const { return m_nom; }

std::string const &Sommet::GetCouleur() const { return m_couleur; }

const std::vector<const Sommet *> &Sommet::GetSuccesseurs() const { return m_sommets; }

const Point &Sommet::GetPosition() const { return m_position; }

float Sommet::GetCentraliteDeg() const { return m_centraliteDeg; }

float Sommet::GetCentraliteProxi() const { return m_centraliteProxi; }

float Sommet::GetCentraliteVec() const { return m_centraliteVec; }

float Sommet::GetCentraliteInter() const { return m_centraliteInter; }

Centralite Sommet::GetCentralite() const { return Centralite(m_centraliteDeg, m_centraliteVec, m_centraliteProxi, m_centraliteInter); }

void Sommet::SetCentraliteDeg(float centraliteDeg) { m_centraliteDeg = centraliteDeg; }

void Sommet::SetCentraliteProxi(float centraliteProxi) { m_centraliteProxi = centraliteProxi; }

void Sommet::SetCentraliteVec(float centraliteVec) { m_centraliteVec = centraliteVec; }

void Sommet::SetCentraliteInter(float centraliteInter) { m_centraliteInter = centraliteInter; }

void Sommet::SetCouleur(std::string const &couleur) { m_couleur = couleur; }

int Sommet::GetDegre() const { return m_sommets.size(); }

bool Sommet::EstAdjacentA(Sommet *sommet) const { return std::find(m_sommets.begin(), m_sommets.end(), sommet) == m_sommets.end(); }

void Sommet::AjouterSuccesseur(Sommet *sommet)
{
    auto it = std::find(m_sommets.begin(), m_sommets.end(), sommet);

    if (it == m_sommets.end())
        m_sommets.push_back(sommet);
    else
        Logger::Log("Le sommet : " + sommet->GetNom() +
                        " n'a pas ete ajoute, il existe deja en tant que successeur du sommet : " + GetNom() + "\n",
                    LoggerPriority::Error);
}

void Sommet::SupprimerSuccesseur(Sommet *sommet)
{
    int it;

    for (int i = 0; i < m_sommets.size(); i++)
    {
        if (m_sommets[i]->GetIndice() == sommet->GetIndice())
        {
            it = i;
            break;
        }
    }
    m_sommets.erase(m_sommets.begin() + it);
}

void Sommet::Afficher() const
{
    std::string str = "Sommet [ " + std::to_string(m_indice) + ", " + m_nom + "," + m_position.ToString() + " ] : ";

    for (auto s : m_sommets)
        str.append(s->GetNom() + " ");

    Logger::Log(str, LoggerPriority::Info);
}
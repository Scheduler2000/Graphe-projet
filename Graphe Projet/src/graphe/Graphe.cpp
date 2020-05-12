#include "Graphe.hpp"
#include "Logger.hpp"
#include "SvgFile.hpp"
#include <algorithm>
#include <ctime>
#include <iostream>
#include <fstream>
#include <queue>
#include <math.h>
#include <stack>

Graphe::Graphe(std::string const &grapheFichier, std::string const &ponderationFicher)
{
    std::ifstream ifs(grapheFichier);
    int orientation;

    if (!ifs)
        throw std::runtime_error("Impossible d'ouvrir le fichier ' " + grapheFichier + " ' en lecture.");

    ifs >> orientation;
    if (ifs.fail())
        throw std::runtime_error("Probleme lecture orientation du graphe.");
    m_orientation = (orientation == 0) ? false : true;

    ifs >> m_ordre;
    if (ifs.fail())
        throw std::runtime_error("Probleme lecture ordre du graphe.");

    int indiceSommet, posX, posY;
    std::string nom;
    for (int i = 0; i < m_ordre; i++)
    {
        ifs >> indiceSommet >> nom >> posX >> posY;

        Sommet *sommet = new Sommet(indiceSommet, nom, Point{posX, posY});
        m_sommets.push_back(sommet);
    }

    ifs >> m_taille;
    int indiceArete, sommet1, sommet2;
    Sommet *s1;
    Sommet *s2;

    for (int i = 0; i < m_taille; i++)
    {
        ifs >> indiceArete >> sommet1 >> sommet2;

        for (auto sommet : m_sommets)
        {
            if (sommet->GetIndice() == sommet1)
                s1 = sommet;

            if (sommet->GetIndice() == sommet2)
                s2 = sommet;
        }
        s1->AjouterSuccesseur(s2);
        s2->AjouterSuccesseur(s1);
        auto arete = new Arete(indiceArete, s1, s2);
        m_aretes.push_back(arete);
        m_base_arretes.push_back(new Arete(indiceArete, s1, s2));
    }

    if (ponderationFicher != " ")
    {
        std::ifstream ifsBis(ponderationFicher);

        if (!ifsBis)
            throw std::runtime_error("Impossible d'ouvrir le fichier ' " + ponderationFicher + " ' en lecture.");

        int nbrAretes, areteIndice, poids;

        ifsBis >> nbrAretes;
        if (ifsBis.fail())
            throw std::runtime_error("Probleme lecture nombre aretes du graphe.");

        for (int i = 0; i < nbrAretes; i++)
        {
            ifsBis >> areteIndice >> poids;

            for (auto arete : m_aretes)
            {
                if (arete->GetIndice() == areteIndice)
                {
                    arete->SetPoids(poids);
                    arete->SetResetPoids(poids);
                }
            }
        }
    }
}

Graphe::~Graphe()
{
    for (auto sommet : m_sommets)
        delete sommet;

    for (auto arete : m_aretes)
        delete arete;

    for (auto arete : m_base_arretes)
        delete arete;
}

void Graphe::AnalyserCentralite()
{

    for (auto sommet : m_sommets) /* centralité de deg  */
    {
        float val = ((float)sommet->GetDegre() / (m_sommets.size() - 1));
        sommet->SetCentraliteDeg(val);
    }

    float lambda = 0.0f;
    float lambda_prec = 0.0f;
    float total_cv = 0.0f;
    do /* centralité vec*/
    {
        lambda_prec = lambda;

        for (auto s : m_sommets)
        {
            total_cv = 0.0f;

            for (auto succ : s->GetSuccesseurs())
                total_cv += succ->GetCentraliteVec();

            s->SetCentraliteVec(total_cv);
        }

        for (auto s : m_sommets)
            lambda += (float)pow(s->GetCentraliteVec(), 2);

        lambda = sqrt(lambda);

        for (auto s : m_sommets)
        {
            float centraliteVec = s->GetCentraliteVec();
            s->SetCentraliteVec(centraliteVec / lambda);
        }

    } while (lambda != lambda_prec);

    for (auto s1 : m_sommets) /* centralité de proxi */
    {
        int distance_total = 0;

        for (auto s2 : m_sommets)
            distance_total += Dijkstra(s1->GetIndice(), s2->GetIndice()).first;

        float value = (float)(m_sommets.size() - 1) / (float)distance_total;
        s1->SetCentraliteProxi(std::isinf(value) ? 0 : value);
    }

    for (auto sommet : m_sommets) /* centralité intermediaire */
    {
        sommet->SetCentraliteInter(CalculerInterm(sommet->GetIndice()));
    }
}

std::map<std::pair<int, int>, std::pair<int, std::vector<int>>> Graphe::Dijkstra(int sommet_s0, bool print)
{
    int nbrSommets = m_sommets.size();
    int distance;
    auto comp = [](const std::pair<int, int> &a, const std::pair<int, int> &b) { return a.second > b.second; };

    std::map<std::pair<int, int>, std::pair<int, std::vector<int>>> resultat;
    std::vector<int> chemin;
    std::vector<std::vector<std::pair<int, float>>> data(nbrSommets);
    std::vector<int> Longueurs(nbrSommets, std::numeric_limits<int>::max());
    std::vector<int> preds(nbrSommets, -1);
    std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, decltype(comp)> Q(comp);

    for (auto arete : m_aretes)
    {
        int indice_s1 = arete->GetSommet1()->GetIndice();
        int indice_s2 = arete->GetSommet2()->GetIndice();
        float poids = arete->GetPoids();

        data[indice_s1].push_back(std::make_pair(indice_s2, poids));
        data[indice_s2].push_back(std::make_pair(indice_s1, poids));
    }

    Longueurs[sommet_s0] = 0;
    Q.push(std::make_pair(sommet_s0, 0));

    while (!Q.empty())
    {
        int sommetProche = Q.top().first;
        int distance = Q.top().second;
        Q.pop();

        if (distance <= Longueurs[sommetProche])
        {
            for (auto const &i : data[sommetProche])
            {
                auto sommetProche2 = i.first;
                auto distance2 = i.second;

                if (Longueurs[sommetProche] + distance2 <= Longueurs[sommetProche2])
                {
                    Longueurs[sommetProche2] = Longueurs[sommetProche] + distance2;

                    preds[sommetProche2] = sommetProche;
                    Q.push(std::make_pair(sommetProche2, Longueurs[sommetProche2]));
                }
            }
        }
    }

    for (auto i = 0; i != nbrSommets; ++i)
    {
        if (print)
            std::cout << "\nChemin de : " << sommet_s0 << " vers " << i << " | Longueur-Poids = " << Longueurs[i] << std::endl;

        if (print)
            std::cout << i;

        chemin.push_back(i);
        for (auto p = preds[i]; p != -1; p = preds[p])
        {
            if (print)
                std::cout << " <- " << p;
            chemin.push_back(p);
        }
        if (print)
            std::cout << std::endl;
        std::reverse(chemin.begin(), chemin.end());

        for (int i = 0; i < chemin.size() - 1; i++)
        {
            auto arete = GetArete(chemin[i], chemin[i + 1]);
            auto poids = arete->GetPoids();

            arete->SetPoids(poids + 0.01);
        }

        if (print)
            std::cout << std::endl;

        resultat[std::make_pair(sommet_s0, i)] = {Longueurs[i] == std::numeric_limits<int>::max() ? 0 : Longueurs[i], chemin};
        chemin.clear();
    }
    return resultat;
}

std::pair<int, std::vector<int>> Graphe::Dijkstra(int sommet_s0, int sommet_final) { return Dijkstra(sommet_s0)[{sommet_s0, sommet_final}]; }

int Graphe::CalculerPCC(int sommet_s0, int sommet_final, int sommetFreq, int *outFreq)
{
    bool different = false;
    *outFreq = 0;
    int pcc = 1;
    std::vector<int> chemin_actuel;
    std::vector<int> chemin_prec;

    do
    {
        different = false;
        chemin_prec = chemin_actuel;
        chemin_actuel = Dijkstra(sommet_s0, sommet_final).second;

        if (chemin_actuel.size() == chemin_prec.size())
        {

            for (int i = 0; i < chemin_actuel.size(); i++)
            {

                if (chemin_actuel[i] != chemin_prec[i])
                {
                    different = true;
                    pcc += 1;
                }
            }
        }

        if (different || chemin_prec.size() == 0)
            for (auto s : chemin_actuel)
                if (s == sommetFreq)
                    *outFreq += 1;

    } while (different || chemin_prec.size() == 0);

    for (auto arete : m_aretes)
        arete->ResetPoids();

    return pcc;
}

float Graphe::CalculerInterm(int indice_sommet)
{
    float centralite = 0.0f;
    int freq = 0;
    int pcc = 0;
    for (auto sommet : m_sommets)
    {

        if (sommet->GetIndice() != indice_sommet)
        {
            for (auto sommet2 : m_sommets)
            {
                if (sommet2->GetIndice() != sommet->GetIndice() && sommet2->GetIndice() != indice_sommet)
                {

                    pcc = CalculerPCC(sommet->GetIndice(), sommet2->GetIndice(), indice_sommet, &freq);

                    std::cout << "(" << sommet->GetNom() << "[" << sommet->GetIndice() << "]"
                              << " , " << sommet2->GetNom() << "[" << sommet2->GetIndice() << "]"
                              << ") | PCC = " << pcc << " , freq = " << freq << std::endl;

                    centralite += (float)((float)freq / (float)pcc);
                }
            }
        }
    }
    std::cout << "Centralite : " << centralite << std::endl;

    return centralite;
}

Sommet *Graphe::GetSommet(int indice) const
{
    for (auto sommet : m_sommets)
        if (sommet->GetIndice() == indice)
            return sommet;
    return nullptr;
}

Arete *Graphe::GetArete(int indice_s1, int indice_s2) const
{

    for (auto arete : m_aretes)
        if (arete->GetSommet1()->GetIndice() == indice_s1 && arete->GetSommet2()->GetIndice() == indice_s2 ||
            arete->GetSommet1()->GetIndice() == indice_s2 && arete->GetSommet2()->GetIndice() == indice_s1)
            return arete;
    return nullptr;
}
int Graphe::BFS(int num_s0) const
{
    int parcouru = 0;

    std::queue<const Sommet *> file;

    std::vector<int> couleurs((int)m_sommets.size(), 0);

    std::vector<int> preds((int)m_sommets.size(), -1);

    file.push(m_sommets[num_s0]);
    couleurs[num_s0] = 1;
    const Sommet *s;
    while (!file.empty())
    {
        s = file.front();
        file.pop();

        for (auto succ : s->GetSuccesseurs())
        {
            if (couleurs[succ->GetIndice()] == 0)
            {
                couleurs[succ->GetIndice()] = 1;
                preds[succ->GetIndice()] = s->GetIndice();
                parcouru++;

                file.push(succ);
            }
        }
    }
    return parcouru;
}

void Graphe::AnalyseVulnerabilite(std::vector<int> const &indices_a_supprimer)
{
    std::vector<Centralite> centralites_avant;
    std::vector<Centralite> centralites_apres;
    bool connexe_avant, connexe_apres;

    connexe_avant = AnalyserConnexite();

    if (connexe_avant)
        std::cout << "avant connexe " << std::endl;
    else
        std::cout << "avant non connexe" << std::endl;

    AnalyserCentralite();
    for (auto sommet : m_sommets)
        centralites_avant.push_back(sommet->GetCentralite());

    for (auto indice : indices_a_supprimer)
        SupprimerArete(indice);

    connexe_apres = AnalyserConnexite();

    if (connexe_apres)
        std::cout << "apres connexe " << std::endl;
    else
        std::cout << "apres non connexe" << std::endl;

    AnalyserCentralite();
    for (auto sommet : m_sommets)
        centralites_apres.push_back(sommet->GetCentralite());

    time_t rawtime;
    struct tm *timeinfo;
    char buffer[80];
    time(&rawtime);
    timeinfo = localtime(&rawtime);

    strftime(buffer, sizeof(buffer), "%d-%m-%Y_%H-%M-%S", timeinfo);
    std::string path(std::string(buffer) + "_" + "analyse.txt");
    std::ofstream fichier(path);

    for (int i = 0; i < m_sommets.size(); i++)
    {
        auto deg_avant_norma = centralites_avant[i].GetDegre();
        auto deg_avant_brute = centralites_avant[i].GetDegre() * (float)((m_sommets.size() - 1));

        auto deg_apres_norma = centralites_apres[i].GetDegre();
        auto deg_apres_brute = centralites_apres[i].GetDegre() * (float)((m_sommets.size() - 1));

        auto vec_avant_norma = centralites_avant[i].GetVecteurPropre();
        auto vec_avant_brute = centralites_avant[i].GetVecteurPropre();

        auto vec_apres_norma = centralites_apres[i].GetVecteurPropre();
        auto vec_apres_brute = centralites_apres[i].GetVecteurPropre();

        auto proxi_avant_norma = centralites_avant[i].GetProximite();
        auto proxi_avant_brute = centralites_avant[i].GetProximite() / (float)((m_sommets.size() - 1));

        auto proxi_apres_norma = centralites_apres[i].GetProximite();
        auto proxi_apres_brute = centralites_apres[i].GetProximite() / (float)((m_sommets.size() - 1));

        auto inter_avant_norma = (float)(centralites_avant[i].GetIntermediaire() * 2) / (float)(pow(m_sommets.size(), 2) - 3 * m_sommets.size() + 2);
        auto inter_avant_brute = centralites_avant[i].GetIntermediaire();

        auto inter_apres_norma = (float)(centralites_apres[i].GetIntermediaire()) * 2 / (float)(pow(m_sommets.size(), 2) - 3 * m_sommets.size() + 2);
        auto inter_apres_brute = centralites_apres[i].GetIntermediaire();

        std::cout << "sommet : " + std::to_string(m_sommets[i]->GetIndice()) << std::endl;
        std::cout << "\t-Centralite av norma deg = " + std::to_string(deg_avant_norma) << " brute : " + std::to_string(deg_avant_brute) << std::endl;
        std::cout << "\t-Centralite av norma vec = " + std::to_string(vec_avant_norma) << " brute : " + std::to_string(vec_avant_brute) << std::endl;
        std::cout << "\t-Centralite av norma proxi = " + std::to_string(proxi_avant_norma) << " brute : " + std::to_string(proxi_avant_brute) << std::endl;
        std::cout << "\t-Centralite av norma inter = " + std::to_string(inter_avant_norma) << " brute : " + std::to_string(inter_avant_brute) << std::endl
                  << std::endl;

        std::cout << "\t-Centralite ap norma deg = " + std::to_string(deg_apres_norma) << " brute : " + std::to_string(deg_apres_brute) << std::endl;
        std::cout << "\t-Centralite ap norma vec = " + std::to_string(vec_apres_norma) << " brute : " + std::to_string(vec_apres_brute) << std::endl;
        std::cout << "\t-Centralite ap norma proxi = " + std::to_string(proxi_apres_norma) << " brute : " + std::to_string(proxi_apres_brute) << std::endl;
        std::cout << "\t-Centralite ap norma inter = " + std::to_string(inter_apres_norma) << " brute : " + std::to_string(inter_apres_brute) << std::endl
                  << std::endl;

        std::cout << "\t-Centralite delta norma deg = " + std::to_string(abs(deg_avant_norma - deg_apres_norma)) << " brute : " + std::to_string(abs(deg_avant_brute - deg_apres_brute)) << std::endl;
        std::cout << "\t-Centralite delta norma vec = " + std::to_string(abs(vec_avant_norma - vec_apres_norma)) << " brute : " + std::to_string(abs(vec_avant_brute - vec_apres_brute)) << std::endl;
        std::cout << "\t-Centralite delta norma proxi = " + std::to_string(abs(proxi_avant_norma - proxi_apres_norma)) << " brute : " + std::to_string(abs(proxi_avant_brute - proxi_apres_brute)) << std::endl;
        std::cout << "\t-Centralite delta norma inter = " + std::to_string(abs(inter_avant_norma - inter_apres_norma)) << " brute : " + std::to_string(abs(inter_avant_brute - inter_apres_brute)) << std::endl
                  << std::endl;

        fichier << "sommet : " + std::to_string(m_sommets[i]->GetIndice()) << std::endl;
        fichier << "\t-Centralite av norma deg = " + std::to_string(deg_avant_norma) << " brute : " + std::to_string(deg_avant_brute) << std::endl;
        fichier << "\t-Centralite av norma vec = " + std::to_string(vec_avant_norma) << " brute : " + std::to_string(vec_avant_brute) << std::endl;
        fichier << "\t-Centralite av norma proxi = " + std::to_string(proxi_avant_norma) << " brute : " + std::to_string(proxi_avant_brute) << std::endl;
        fichier << "\t-Centralite av norma inter = " + std::to_string(inter_avant_norma) << " brute : " + std::to_string(inter_avant_brute) << std::endl
                << std::endl;

        fichier << "\t-Centralite ap norma deg = " + std::to_string(deg_apres_norma) << " brute : " + std::to_string(deg_apres_brute) << std::endl;
        fichier << "\t-Centralite ap norma vec = " + std::to_string(vec_apres_norma) << " brute : " + std::to_string(vec_apres_brute) << std::endl;
        fichier << "\t-Centralite ap norma proxi = " + std::to_string(proxi_apres_norma) << " brute : " + std::to_string(proxi_apres_brute) << std::endl;
        fichier << "\t-Centralite ap norma inter = " + std::to_string(inter_apres_norma) << " brute : " + std::to_string(inter_apres_brute) << std::endl
                << std::endl;

        fichier << "\t-Centralite delta norma deg = " + std::to_string(abs(deg_avant_norma - deg_apres_norma)) << " brute : " + std::to_string(abs(deg_avant_brute - deg_apres_brute)) << std::endl;
        fichier << "\t-Centralite delta norma vec = " + std::to_string(abs(vec_avant_norma - vec_apres_norma)) << " brute : " + std::to_string(abs(vec_avant_brute - vec_apres_brute)) << std::endl;
        fichier << "\t-Centralite delta norma proxi = " + std::to_string(abs(proxi_avant_norma - proxi_apres_norma)) << " brute : " + std::to_string(abs(proxi_avant_brute - proxi_apres_brute)) << std::endl;
        fichier << "\t-Centralite delta norma inter = " + std::to_string(abs(inter_avant_norma - inter_apres_norma)) << " brute : " + std::to_string(abs(inter_avant_brute - inter_apres_brute)) << std::endl
                << std::endl;
    }
}

void Graphe::SupprimerArete(int indice)
{
    int it;

    for (int i = 0; i < m_aretes.size(); i++)
    {
        if (m_aretes[i]->GetIndice() == indice)
        {
            it = i;
            auto s1 = m_aretes[i]->GetSommet1();
            auto s2 = m_aretes[i]->GetSommet2();

            s1->SupprimerSuccesseur(s2);
            s2->SupprimerSuccesseur(s1);

            delete m_aretes[i];
            break;
        }
    }
    m_aretes.erase(m_aretes.begin() + it);
}

bool Graphe::AnalyserConnexite() const { return BFS(0) == m_sommets.size() - 1; }

int Graphe::CalculerKConnexite()
{
    if (!AnalyserConnexite())
        return 0;
}

void Graphe::Colorer()
{
    std::vector<std::string> couleurs = {"cyan", "black", "teal", "lime", "blue", "green", "yellow", "fuchsia"};
    int col = 0;
    bool colorer = false;
    std::sort(m_sommets.begin(), m_sommets.end(), [](Sommet *s1, Sommet *s2) { return s1->GetDegre() > s2->GetDegre(); });
    m_sommets[0]->SetCouleur(couleurs[col]);

    for (auto current : m_sommets)
    {
        if (current->GetCouleur() != "red")
            continue;

        if (colorer)
        {
            col++;
            colorer = false;
        }

        for (auto s : m_sommets)
        {
            if (current != s && !current->EstAdjacentA(s))
            {
                s->SetCouleur(couleurs[col]);
                colorer = true;
            }
        }
    }
}

void Graphe::Afficher() const
{
    std::cout << " graphe ";
    if (m_orientation)
        std::cout << "oriente" << std::endl
                  << " ";
    else
        std::cout << "non oriente" << std::endl
                  << " ";
    std::cout << "ordre = " << m_ordre << std::endl
              << " ";

    std::cout << "taille = " << m_taille << std::endl
              << " ";

    std::cout << "listes d'adjacence :" << std::endl;

    for (auto s : m_sommets)
    {
        s->Afficher();
        std::cout << std::endl;
    }
}

void Graphe::Dessiner() const
{
    Svgfile dessinVec;
    dessinVec.addGrid();
    for (auto sommet : m_sommets)
    {
        Point position = sommet->GetPosition();

        dessinVec.addDisk(position.GetX(), position.GetY(), 5, sommet->GetCouleur());
        dessinVec.addText(position.GetX(), position.GetY() - 10, sommet->GetNom() + "(" + std::to_string(sommet->GetIndice()) + ")");
    }

    for (auto arete : m_aretes)
    {

        auto position1 = arete->GetSommet1()->GetPosition();
        auto position2 = arete->GetSommet2()->GetPosition();
        auto position = Point((position1.GetX() + position2.GetX()) / 2, (position1.GetY() + position2.GetY()) / 2);

        std::string poids = std::to_string(arete->GetPoids());
        poids.resize(4);

        dessinVec.addLine(position1.GetX(), position1.GetY(), position2.GetX(), position2.GetY());
        dessinVec.addText(position.GetX(), position.GetY() - 13, poids + "(" + std::to_string(arete->GetIndice()) + ")");
    }
}
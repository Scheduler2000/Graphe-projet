#include "ApplicationMenu.hpp"
#include "Logger.hpp"
#include "Graphe.hpp"
#include "ConsoleExtension.hpp"
#include <iostream>

void ApplicationMenu::dessinerAscii() const
{
    std::cout << "\n  [ M E N U E ]  " << std::endl
              << std::endl;
}

void ApplicationMenu::afficherChoix() const
{

    dessinerAscii();

    std::cout << "0. exit" << std::endl;
    std::cout << "1. charger un graphe" << std::endl;
    std::cout << "2. charger un fichier de ponderation" << std::endl;
    std::cout << "3. colorer le graphe" << std::endl;
    std::cout << "4. supprimer des aretes" << std::endl;
    std::cout << "5. analyser la connexite" << std::endl;
    std::cout << "6. analyser les chemins" << std::endl;
    std::cout << "7. calculer un poids" << std::endl;
    std::cout << "8. analyser la vulnerabilite" << std::endl;
    std::cout << "9. quitter le graphe" << std::endl
              << std::endl;
}

void ApplicationMenu::Run() const
{
    int choix;
    Graphe *graphe = nullptr;
    std::string fichierGraphe;

    do
    {
        //ConsoleExtension::ClearConsole();
        afficherChoix();

        std::cout << "Choix menu : ";
        ConsoleExtension::SetConsoleTextColor(ConsoleExtension::ConsoleColor::Green);
        std::cin >> choix;
        ConsoleExtension::SetConsoleTextColor(ConsoleExtension::ConsoleColor::Default_Color);
        std::cout << std::endl;

        switch (choix)
        {

        case 0:
            std::cout << "Fermeture de l'application." << std::endl;
            delete graphe;
            graphe = nullptr;
            break;

        case 1:
            if (graphe == nullptr)
            {
                std::string fichierPonderation;
                std::cout << "Entrer le chemin d'acces au graphe : ";
                ConsoleExtension::SetConsoleTextColor(ConsoleExtension::ConsoleColor::Green);
                std::cin >> fichierGraphe;
                ConsoleExtension::SetConsoleTextColor(ConsoleExtension::ConsoleColor::Default_Color);

                std::cout << "Entrer le chemin d'acces au fichier de ponderation [ou 'NULL'] : ";
                ConsoleExtension::SetConsoleTextColor(ConsoleExtension::ConsoleColor::Green);
                std::cin >> fichierPonderation;
                ConsoleExtension::SetConsoleTextColor(ConsoleExtension::ConsoleColor::Default_Color);

                graphe = new Graphe(fichierGraphe, fichierPonderation == "NULL" ? " " : fichierPonderation);

                graphe->Afficher();
                graphe->Dessiner();
            }
            else
                Logger::Log("Veuillez d'abord quitter votre scene actuelle !", LoggerPriority::Error);

            break;

        case 2:
            if (graphe != nullptr)
            {
                std::string fichierPonderation;
                std::cout << "Entrer le chemin d'acces au fichier de ponderation [ou 'NULL'] : ";
                ConsoleExtension::SetConsoleTextColor(ConsoleExtension::ConsoleColor::Green);
                std::cin >> fichierPonderation;
                ConsoleExtension::SetConsoleTextColor(ConsoleExtension::ConsoleColor::Default_Color);

                delete graphe;

                graphe = new Graphe(fichierGraphe, fichierPonderation);
                graphe->Afficher();
                graphe->Dessiner();
            }
            else
                Logger::Log("Erreur, veuillez d'abord charger un graphe.", LoggerPriority::Error);
            break;

        case 3:
            if (graphe != nullptr)
            {
                graphe->Colorer();
                graphe->Dessiner();
            }
            else
                Logger::Log("Erreur, veuillez d'abord charger un graphe.", LoggerPriority::Error);
            break;

        case 4:
            if (graphe != nullptr)
            {
                int indice;
                std::cout
                    << "Entrer l'indice de l'arete a supprimer' : ";
                ConsoleExtension::SetConsoleTextColor(ConsoleExtension::ConsoleColor::Green);
                std::cin >> indice;
                ConsoleExtension::SetConsoleTextColor(ConsoleExtension::ConsoleColor::Default_Color);

                graphe->SupprimerArete(indice);
                graphe->Dessiner();
            }
            else
                Logger::Log("Erreur, veuillez d'abord charger un graphe.", LoggerPriority::Error);
            break;

        case 5:
            if (graphe != nullptr)
            {
                bool connexe = graphe->AnalyserConnexite();
                Logger::Log(connexe ? "Graphe connexe !" : "Graphe non connexe", connexe ? LoggerPriority::Info : LoggerPriority::Warn);
            }
            else
                Logger::Log("Erreur, veuillez d'abord charger un graphe.", LoggerPriority::Error);
            break;

        case 6:
            if (graphe != nullptr)
            {
                int indice;

                std::cout
                    << "Entrer l'indice du sommet de depart : ";
                ConsoleExtension::SetConsoleTextColor(ConsoleExtension::ConsoleColor::Green);
                std::cin >> indice;
                ConsoleExtension::SetConsoleTextColor(ConsoleExtension::ConsoleColor::Default_Color);

                graphe->Dijkstra(indice, true);
            }
            else
                Logger::Log("Erreur, veuillez d'abord charger un graphe.", LoggerPriority::Error);

            break;

        case 7:
            if (graphe != nullptr)
            {
                int depart, arrivee;

                std::cout
                    << "Entrer l'indice du sommet de depart : ";
                ConsoleExtension::SetConsoleTextColor(ConsoleExtension::ConsoleColor::Green);
                std::cin >> depart;
                ConsoleExtension::SetConsoleTextColor(ConsoleExtension::ConsoleColor::Default_Color);

                std::cout
                    << "Entrer l'indice du sommet d'arrivee : ";
                ConsoleExtension::SetConsoleTextColor(ConsoleExtension::ConsoleColor::Green);
                std::cin >> arrivee;
                ConsoleExtension::SetConsoleTextColor(ConsoleExtension::ConsoleColor::Default_Color);

                Logger::Log(std::to_string(depart) + " -> " + std::to_string(arrivee) + " = " + std::to_string(graphe->Dijkstra(depart, arrivee).first), LoggerPriority::Info);
            }
            else
                Logger::Log("Erreur, veuillez d'abord charger un graphe.", LoggerPriority::Error);
            break;
        case 8:
            if (graphe != nullptr)
            {
                int indice;
                std::vector<int> indices;

                std::cout
                    << "Entrer les indices des aretes a supprimer (-1 pour arreter) : ";
                ConsoleExtension::SetConsoleTextColor(ConsoleExtension::ConsoleColor::Green);
                do
                {
                    std::cin >> indice;
                    if (indice >= 0)
                        indices.push_back(indice);
                    else
                        break;

                } while (true);

                ConsoleExtension::SetConsoleTextColor(ConsoleExtension::ConsoleColor::Default_Color);
                graphe->AnalyseVulnerabilite(indices);

                graphe->Colorer();
                graphe->Dessiner();
            }
            else
                Logger::Log("Erreur, veuillez d'abord charger un graphe.", LoggerPriority::Error);

            break;

        case 9:
            if (graphe != nullptr)
            {
                delete graphe;
                graphe = nullptr;
            }
            else
                Logger::Log("Erreur, veuillez d'abord charger un graphe.", LoggerPriority::Error);
            break;
        default:

            Logger::Log("Choix du menu incorrect !", LoggerPriority::Error);

            continue;
            std::cout << std::endl;
        }
    } while (choix != 0);
}
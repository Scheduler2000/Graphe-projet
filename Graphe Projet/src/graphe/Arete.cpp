#include "Arete.hpp"

Arete::Arete(int indice, Sommet *s1, Sommet *s2) : m_indice(indice), m_sommet1(s1), m_sommet2(s2), m_poids(1), m_basePoids(1) {}

float Arete::GetPoids() const { return m_poids; }

int Arete::GetIndice() const { return m_indice; }

void Arete::ResetPoids() { m_poids = m_basePoids; }

void Arete::SetResetPoids(float poids) { m_basePoids = poids; }

void Arete::SetPoids(float poids) { m_poids = poids; }

Sommet *Arete::GetSommet1() const { return m_sommet1; }

Sommet *Arete::GetSommet2() const { return m_sommet2; }
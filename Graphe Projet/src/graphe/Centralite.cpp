#include "Centralite.hpp"

Centralite::Centralite(float deg, float vec, float proxi, float inter) : m_degre(deg), m_proximite(proxi), m_vecteurPropre(vec), m_intermediaire(inter) {}

float Centralite::GetDegre() const { return m_degre; }

float Centralite::GetVecteurPropre() const { return m_vecteurPropre; }

float Centralite::GetProximite() const { return m_proximite; }

float Centralite::GetIntermediaire() const { return m_intermediaire; }

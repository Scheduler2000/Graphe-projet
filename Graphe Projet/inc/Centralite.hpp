#if !defined(__CENTRALITE__)
#define __CENTRALITE__

class Centralite
{
private:
    float m_degre;
    float m_vecteurPropre;
    float m_proximite;
    float m_intermediaire;

public:
    Centralite(float deg, float vec, float proxi, float inter);

    float GetDegre() const;
    float GetVecteurPropre() const;
    float GetProximite() const;
    float GetIntermediaire() const;
};

#endif

#if !defined(__ARETE__)
#define __ARETE__

#include "sommet.hpp"

class Arete
{
private:
    float m_poids;
    int m_indice;
    float m_basePoids;
    Sommet *m_sommet1;
    Sommet *m_sommet2;

public:
    Arete(int indice, Sommet *s1, Sommet *s2);

    float GetPoids() const;
    int GetIndice() const;
    Sommet *GetSommet1() const;
    Sommet *GetSommet2() const;

    void ResetPoids();
    void SetResetPoids(float poids);
    void SetPoids(float poids);
};

#endif // __ARETE__

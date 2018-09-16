#include "constantes.h"

#ifndef DEF_EDITEUR
#define DEF_EDITEUR

    void editeur(SDL_Surface* ecran, int carte[][NB_BLOCS_HAUTEUR], int nvv[][NB_BLOCS_HAUTEUR], SDL_Surface *vivant, SDL_Surface *mort);
    void jouer  (SDL_Surface* ecran, int carte[][NB_BLOCS_HAUTEUR], int nvv[][NB_BLOCS_HAUTEUR], SDL_Surface *vivant, SDL_Surface *mort);
    void quadriller(SDL_Surface *ecran);
 //   void razCellules(SDL_surface *ecran);

#endif

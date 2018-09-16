#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <SDL.h>
#include <SDL_image.h>

#include "editeur.h"
#include "constantes.h"


void editeur(SDL_Surface *ecran, int carte[][NB_BLOCS_HAUTEUR], int nvv[][NB_BLOCS_HAUTEUR], SDL_Surface *vivant, SDL_Surface *mort)
{
    int quad = 1;
    SDL_Rect position;
    SDL_Event event;

    int continuer = 1, clicGaucheEnCours = 0, clicDroitEnCours = 0;
    int objetActuel = VIVANT, i = 0, j = 0;


    // Boucle infinie de l'éditeur
    while (continuer)
    {
        SDL_WaitEvent(&event);
        switch(event.type)
        {
            case SDL_QUIT:
                continuer = 0;
                break;
            case SDL_MOUSEBUTTONDOWN:
                if (event.button.button == SDL_BUTTON_LEFT)
                {
                    // On met l'objet actuellement choisi (vivant, mort...) à l'endroit du clic
                    carte[event.button.x / TAILLE_BLOC][event.button.y / TAILLE_BLOC] = objetActuel;
                    clicGaucheEnCours = 1; // On active un booléen pour retenir qu'un bouton est enfoncé
                }
                else if (event.button.button == SDL_BUTTON_RIGHT) // Le clic droit sert à effacer
                {
                    carte[event.button.x / TAILLE_BLOC][event.button.y / TAILLE_BLOC] = objetActuel;
                    clicDroitEnCours = 1;
                }
                break;
            case SDL_MOUSEBUTTONUP: // On désactive le booléen qui disait qu'un bouton était enfoncé
                if (event.button.button == SDL_BUTTON_LEFT)
                    clicGaucheEnCours = 0;
                else if (event.button.button == SDL_BUTTON_RIGHT)
                    clicDroitEnCours = 0;
                break;
            case SDL_MOUSEMOTION:
                if (clicGaucheEnCours) // Si on déplace la souris et que le bouton gauche de la souris est enfoncé
                {
                    carte[event.motion.x / TAILLE_BLOC][event.motion.y / TAILLE_BLOC] = objetActuel;
                }
                else if (clicDroitEnCours) // Pareil pour le bouton droit de la souris
                {
                    carte[event.motion.x / TAILLE_BLOC][event.motion.y / TAILLE_BLOC] = objetActuel;
                }
                break;
            case SDL_KEYDOWN:
                switch(event.key.keysym.sym)
                {
                    case SDLK_ESCAPE:
                        continuer = 0;
                        break;
              /*      case SDLK_s:
                        sauvegarderNiveau(carte);
                        break;
                    case SDLK_c:
                        chargerNiveau(carte);
                        break; */
                    case SDLK_KP1:
                        objetActuel = VIVANT;
                        break;
                    case SDLK_KP2:
                        objetActuel = MORT;
                        break;
                    case SDLK_RETURN :
                        jouer(ecran, carte, nvv, vivant, mort);
                        break;
                    case SDLK_a:   //  (touche "Q" du clavier ( qwerty))
                        if(quad)
                        {
                            quad = 0;
                        }
                        else
                        {
                            quad = 1;
                        }
                        break;


                }
                break;
        }

        // Effacement de l'écran
        SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 255, 255, 255));

        // Placement des objets à l'écran
        for (i = 0 ; i < NB_BLOCS_LARGEUR ; i++)
        {
            for (j = 0 ; j < NB_BLOCS_HAUTEUR ; j++)
            {
                position.x = i * TAILLE_BLOC;
                position.y = j * TAILLE_BLOC;

                switch(carte[i][j])
                {
                    case VIVANT:
                        SDL_BlitSurface(vivant, NULL, ecran, &position);
                        break;
                    case MORT:
                        SDL_BlitSurface(mort, NULL, ecran, &position);
                        break;

                }
            }
        }
                if(quad)
                {
                    quadriller(ecran);
                }

        // Mise à jour de l'écran
        SDL_Flip(ecran);
    }

    SDL_FreeSurface(vivant);
    SDL_FreeSurface(mort);

}

//-------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------


void jouer(SDL_Surface *ecran, int carte[NB_BLOCS_LARGEUR][NB_BLOCS_HAUTEUR], int nvv[][NB_BLOCS_HAUTEUR], SDL_Surface *vivant, SDL_Surface *mort)  // pour l'instant cette fonction coupe le jeux ...
{
    int i = 0, j =0 ;
    int continuer = 1;
    SDL_Rect position;

    // VERIFICATION DU NOMBRE DE VOISINS VIVANTS

    for(i = 0 ; i < NB_BLOCS_HAUTEUR ; i++)
    {
        for(j = 0 ; j < NB_BLOCS_LARGEUR ; i++)
        {
                if ( i == 0 && j == 0 )   // coin haut gauche
                {
                    if ( carte[i][j+1] == VIVANT)
                    {
                        nvv[i][j] = nvv[i][j]+1;
                    }
                    else if ( carte[i+1][j] == VIVANT)
                    {
                        nvv[i][j] = nvv[i][j]+1;
                    }
                }

                else if( i == 0 && j > 0 && j < (NB_BLOCS_LARGEUR - 1) )     // première ligne
                {
                    if ( carte[i][j+1] == VIVANT)
                    {
                        nvv[i][j] = nvv[i][j]+1;
                    }
                    else if ( carte[i][j-1] == VIVANT)
                    {
                        nvv[i][j] = nvv[i][j]+1;
                    }
                    else if ( carte[i+1][j] == VIVANT)
                    {
                        nvv[i][j] = nvv[i][j]+1;
                    }
                }

                else if( i == 0 && j == (NB_BLOCS_LARGEUR - 1)) // coin haut droit
                {
                    if ( carte[i][j-1] == VIVANT)
                    {
                        nvv[i][j] = nvv[i][j]+1;
                    }
                    else if ( carte[i+1][j] == VIVANT)
                    {
                        nvv[i][j] = nvv[i][j]+1;
                    }
                }

                else if ( j == 0 && i > 0 && i < (NB_BLOCS_HAUTEUR - 1) ) // première colonne
                {
                     if ( carte[i][j+1] == VIVANT)
                    {
                        nvv[i][j] = nvv[i][j]+1;
                    }
                    else if ( carte[i+1][j] == VIVANT)
                    {
                        nvv[i][j] = nvv[i][j]+1;
                    }
                    else if ( carte[i-1][j] == VIVANT)
                    {
                       nvv[i][j] = nvv[i][j]+1;
                    }
                }

                else if ( j == (NB_BLOCS_LARGEUR - 1) && i > 0 && i < (NB_BLOCS_HAUTEUR - 1) ) // dernière colonne
                {
                    if ( carte[i][j-1] == VIVANT)
                    {
                        nvv[i][j] = nvv[i][j]+1;
                    }
                    else if ( carte[i+1][j] == VIVANT)
                    {
                        nvv[i][j] = nvv[i][j]+1;
                    }
                    else if ( carte[i-1][j] == VIVANT)
                    {
                        nvv[i][j] = nvv[i][j]+1;
                    }
                }

                else if( i == ( NB_BLOCS_HAUTEUR - 1 ) && j == 0 ) // coin bas gauche
                {
                    if ( carte[i][j+1] == VIVANT)
                    {
                        nvv[i][j] = nvv[i][j]+1;
                    }
                    else if ( carte[i-1][j] == VIVANT)
                    {
                       nvv[i][j] = nvv[i][j]+1;
                    }
                }

                else if( i == (NB_BLOCS_HAUTEUR - 1) && j > 0 && j < (NB_BLOCS_LARGEUR - 1) ) // dernière ligne
                {
                    if ( carte[i][j+1] == VIVANT)
                    {
                        nvv[i][j] = nvv[i][j]+1;
                    }
                    else if ( carte[i][j-1] == VIVANT)
                    {
                        nvv[i][j] = nvv[i][j]+1;
                    }
                    else if ( carte[i-1][j] == VIVANT)
                    {
                        nvv[i][j] = nvv[i][j]+1;
                    }
                }

                else if( i == ( NB_BLOCS_HAUTEUR - 1 ) && j == (NB_BLOCS_LARGEUR - 1 ) ) //coin bas droite
                {
                    if ( carte[i][j-1] == VIVANT)
                    {
                        nvv[i][j] = nvv[i][j]+1;
                    }
                    else if ( carte[i-1][j] == VIVANT)
                    {
                       nvv[i][j] = nvv[i][j]+1;
                    }
                }

                else if ( i > 0 && i < (NB_BLOCS_HAUTEUR - 1) && j > 0 && j < (NB_BLOCS_LARGEUR - 1) ) // dans les autres cas ( au milieu )
                {
                    if ( carte[i][j+1] == VIVANT)
                    {
                        nvv[i][j] = nvv[i][j]+1;
                    }
                    else if ( carte[i][j-1] == VIVANT)
                    {
                        nvv[i][j] = nvv[i][j]+1;
                    }
                    else if ( carte[i+1][j] == VIVANT) // bug ici pourquoi ?
                    {
                        nvv[i][j] = nvv[i][j]+1;
                    }
                    else if ( carte[i-1][j] == VIVANT)
                    {
                       nvv[i][j] = nvv[i][j]+1;
                    }
                }
        }

    }

    //-------------------------------------------------------------------------------------------------
    //-------------------------------------------------------------------------------------------------
    // Modification de la carte
    while(continuer)
    {
        i = 0;
        j = 0;

    for(i = 0 ; i < NB_BLOCS_HAUTEUR ; i++)
    {
        for(j = 0 ; j < NB_BLOCS_LARGEUR ; j++)
        {

            if( nvv[i][j] > 3 )
            {
            carte[i][j] = MORT;
            }
            else if(nvv[i][j] < 2)
            {
            carte[i][j] = MORT;
            }
            else if( nvv[i][j] < 3 && nvv[i][j] > 1 )
            {
            carte[i][j] = VIVANT;
            }
        }
    }

    //---------------------------------------------------------------------------------------------
    //---------------------------------------------------------------------------------------------
    // Blittage de la carte

     for (i = 0 ; i < NB_BLOCS_HAUTEUR ; i++)
        {
            for (j = 0 ; j < NB_BLOCS_LARGEUR ; j++)
            {
                position.x = i * TAILLE_BLOC;
                position.y = j * TAILLE_BLOC;

                        if (carte[i][j] == VIVANT)
                        {
                                SDL_BlitSurface(vivant, NULL, ecran, &position);
                        }


                         else
                        {
                                SDL_BlitSurface(mort, NULL, ecran, &position);
                        }

            }
        }
    }
}
//--------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------

//void verifier(carte[][NB_BLOCS_HAUTEUR])
//--------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------

void quadriller(SDL_Surface *ecran)
{
SDL_Surface *vertical = NULL, *horizontal = NULL;
vertical = SDL_CreateRGBSurface(SDL_HWSURFACE, 1, HAUTEUR_FENETRE, 32, 0, 0, 0, 0);
horizontal = SDL_CreateRGBSurface(SDL_HWSURFACE, LARGEUR_FENETRE, 1, 32, 0, 0, 0, 0);
Uint32 noir = SDL_MapRGB(ecran->format, 0, 0, 0);
SDL_FillRect(vertical, NULL, noir);
SDL_FillRect(horizontal, NULL, noir);
SDL_Rect position;
position.x = 0;
position.y = 0;
int i = 0;
int j = 0;

for(i = 0 ; i < (NB_BLOCS_LARGEUR + 1) ; i++)
{
    position.y = 0;
    SDL_BlitSurface(vertical, NULL, ecran, &position);
    position.x = ( i * TAILLE_BLOC);
}

for(j = 0 ; j < (NB_BLOCS_HAUTEUR + 1) ; j++)
{
    position.x = 0;
    SDL_BlitSurface(horizontal, NULL, ecran, &position);
    position.y = ( j * TAILLE_BLOC);
}

}

//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------



/*void razCellules(SDL_surface *ecran)
{

}*/

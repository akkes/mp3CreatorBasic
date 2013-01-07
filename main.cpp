#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL_image.h>
#include <fmod/fmod.h>
#define NB_Max 100
#define Y_SLIDE_V 100
#define X_PAUSE 480
#define Y_PAUSE 30
#include <windows.h>

int avant (SDL_Surface *ecran , SDL_Surface *rectn , int xx )
{


    SDL_Rect rec ;
    rec.x=xx+1;
    rec.y=Y_SLIDE_V;
    SDL_BlitSurface(rectn, NULL, ecran, &rec);
    SDL_Flip(ecran);
    return rec.x ;



}
unsigned int arriere (SDL_Surface *ecran , SDL_Surface *rectn , int xx )
{


    SDL_Rect rec ;
    rec.x=xx-1;
    rec.y=Y_SLIDE_V;
    SDL_BlitSurface(rectn, NULL, ecran, &rec);
    SDL_Flip(ecran);
    return rec.x ;



}
int volume_clic(SDL_Surface *ecran ,SDL_Surface *rectn ,int xx ,FMOD_CHANNELGROUP *canal ,SDL_Event event) // change les coordonnées du bouton volume et le volume du son joué
{
    SDL_Rect rec ;
    rec.x = event.button.x-8;
    rec.y=Y_SLIDE_V;
    SDL_BlitSurface(rectn, NULL, ecran, &rec);
    SDL_Flip(ecran);
    float volume ;
    volume = (rec.x-300)*0.0025;
    FMOD_ChannelGroup_SetVolume(canal,  volume);
    return rec.x ;

}
SDL_Surface *pause(FMOD_CHANNELGROUP *canal, SDL_Surface *ecran) //mets la musique en pause
{
     int change ;
     SDL_Surface *Bouton=NULL ;
     SDL_Event eve ;
     SDL_Rect pos;
     pos.x=X_PAUSE;
     pos.y=Y_PAUSE;
     Bouton = SDL_CreateRGBSurface(SDL_HWSURFACE, 40, 40, 32, 0, 0, 0, 0);
     Bouton=SDL_LoadBMP("play.bmp");
     SDL_BlitSurface(Bouton, NULL, ecran, &pos);
     SDL_Flip(ecran);



     FMOD_ChannelGroup_SetPaused(canal, 1);


                      change=1;
                      while (change==1)
                      {
                          SDL_WaitEvent(&eve);
                          if(eve.type==SDL_MOUSEBUTTONUP)
                          {
                              change=0;
                          }
                      }


                      return Bouton ;
}
SDL_Surface *jouer(FMOD_CHANNELGROUP *canal, SDL_Surface *ecran) //relance la musique
{

     SDL_Surface *Bouton=NULL ;
     SDL_Event eve ;
     SDL_Rect pos;
     pos.x=X_PAUSE;
     pos.y=Y_PAUSE;
     Bouton = SDL_CreateRGBSurface(SDL_HWSURFACE, 40, 40, 32, 0, 0, 0, 0);
     Bouton=SDL_LoadBMP("pause.bmp");
     SDL_BlitSurface(Bouton, NULL, ecran, &pos);
     SDL_Flip(ecran);


     FMOD_ChannelGroup_SetPaused(canal, 0);
     return Bouton ;
}

SDL_Rect temps (SDL_Rect cur) // fais avancer un objet de coor x en fonction du temps
{
    int Tempsactuel =0 ; // variable du SDL_ticks
    int tempsPrecedent =0;
      Tempsactuel = SDL_GetTicks();
if (Tempsactuel - tempsPrecedent > 1000)
{
cur.x=cur.x+1;
    tempsPrecedent = Tempsactuel;
     SDL_Delay(15);
}
return cur ;
}

int main(int argc, char *argv[])
{
    SDL_Surface *ecran = NULL , *rectangle= NULL ,*rectn= NULL ,*play=NULL , *derou=NULL, *bouton_rond=NULL  ;
    // bouton_rond bouton de deroulement
    // rectangle : barre de volume
    // rectn : bouton volume
    // play : bouton pause/play
    // derou : barre de deroulement
    SDL_Rect rec , barre ,pla , cur , bou;
    // bou : coor bouton de deroulement
    // rec : coor bouton volume
    // cur : coor barre de deroulement
    // pla : coor bouton pause
    // barre coor barre de volume
    bou.x=200;
    bou.y=290;
    cur.x=200;
    cur.y=300;
    rec.x = 492; //coordonnées du bouton volume
    rec.y = 100 ;
    barre.x=300; // coordonnées de la barre de volume
    barre.y=rec.y+10;
    pla.x=480; // coordonnées du bouton pause
    pla.y=30;
    bool t=0; // variable de la pause
    FMOD_SYSTEM *system; // system son de crysis
    FMOD_SOUND *musique; // son de crysis
    FMOD_RESULT resultat; // resultat init FMOD
    int continuer= 1; // variable de fin de programme
    float volume = 0.5; // volume du canal 1
    SDL_Event event ; // evennement principal
    FMOD_System_Create(&system); // creation systeme son
    FMOD_System_Init(system, 1, FMOD_INIT_NORMAL, NULL); // initialisation du systeme son
    char liste [NB_Max]="" ;
    int xx ;
    FILE* fichier = NULL;
    char caractereActuel[100] ="";
    fichier = fopen("liste.txt", "r");
    if (fichier != NULL)
    {
       fgets(caractereActuel, NB_Max, fichier);
       fclose(fichier);

    }


    resultat = FMOD_System_CreateSound(system, ("%s",caractereActuel) , FMOD_SOFTWARE | FMOD_2D | FMOD_CREATESTREAM, 0, &musique); // test d'iinitialisation

    if (resultat != FMOD_OK) // si il y a un bug dans l'initialisation du son
    {
        fprintf(stderr, "Impossible de lire le fichier \n");
        exit(EXIT_FAILURE);
    }
    SDL_Init(SDL_INIT_VIDEO);
    SDL_EnableKeyRepeat(10, 10); // repetition de touche ( lié au double buff)

    ecran = SDL_SetVideoMode(1000, 600 , 32, SDL_HWSURFACE|SDL_DOUBLEBUF);
    SDL_WM_SetCaption("Programme ISN", NULL);
bouton_rond = SDL_CreateRGBSurface(SDL_HWSURFACE, 27, 27, 32, 0, 0, 0, 0);
derou = SDL_CreateRGBSurface(SDL_HWSURFACE, 600, 13, 32, 0, 0, 0, 0); // creation des surfaces voir debut du main pour les différentes variable
rectangle = SDL_CreateRGBSurface(SDL_HWSURFACE, 800, 10, 32, 0, 0, 0, 0);
rectn = SDL_CreateRGBSurface(SDL_HWSURFACE, 60, 30, 32, 0, 0, 0, 0);
play = SDL_CreateRGBSurface(SDL_HWSURFACE, 40, 40, 32, 0, 0, 0, 0);
    // Coloration de la surface ecran en bleu-vert
    SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format,0, 0, 0)); // remplissage en noir de la fenetre
bouton_rond=SDL_LoadBMP("rond.bmp");
derou=SDL_LoadBMP("derou2.bmp"); // chargement des images BMP
rectangle = SDL_LoadBMP("metal.bmp");
rectn = SDL_LoadBMP("Sans titre.bmp");
play= SDL_LoadBMP("play.bmp");

SDL_BlitSurface(derou, NULL, ecran, &cur); //affichage a l'ecran des surface
SDL_BlitSurface(bouton_rond,NULL,ecran, &bou);
SDL_BlitSurface(rectangle, NULL, ecran, &barre);
SDL_BlitSurface(play, NULL, ecran, &pla);
SDL_BlitSurface(rectn, NULL, ecran, &rec);


FMOD_System_PlaySound(system, FMOD_CHANNEL_FREE, musique, 0, NULL); // initialisation du systeme son
FMOD_CHANNELGROUP *canal; // création du canal
FMOD_System_GetMasterChannelGroup(system, &canal); //assignation du canal
FMOD_ChannelGroup_SetPaused(canal, 1); // mise en pause de la musique

    SDL_Flip(ecran);
    while (continuer) // tant que le programme est en route
{


    SDL_PollEvent(&event);
    switch(event.type)
    {
        case SDL_QUIT: // condition de fin de programme
            continuer = 0;
            break;
        case SDL_KEYDOWN:
                switch(event.key.keysym.sym)
                {
                    case SDLK_RIGHT: // Flèche droite
                    xx = rec.x;
                     rec.x = avant (ecran , rectn , xx);

                        break;
                    case SDLK_LEFT: // Flèche gauche
                        xx= rec.x;
                     rec.x = arriere (ecran , rectn , xx);
                        break;

                }
                break;
                 case SDL_MOUSEBUTTONUP: // evennement clic souris

                  if (event.button.x>480 && event.button.x<520 && event.button.y>30 && event.button.y<70 && t==0)
                 {
                      play=jouer(canal,ecran);


                      t=1; // variable assignées a la pause ; t=1 la musique est jouée
                 }
                 break;
                 case SDL_MOUSEBUTTONDOWN:
                  if (event.button.x>480 && event.button.x<520 && event.button.y>30 && event.button.y<70 && t==1)
                 {
                     play=pause(canal,ecran) ;
                     t=0; // variable assignées a la pause ; t=0 la musique est en pause

                 }
                  if (event.button.x<700 && event.button.x>300 && event.button.y>110 && event.button.y<120 )
                 {

                     xx=rec.x;
                     rec.x = volume_clic(ecran , rectn , xx , canal,event); // fonction qui change les coordonnée du bouton et le volume du canal

                 }
                 break;
    }
    if (t==1)
    {
       bou=temps(bou);
       SDL_BlitSurface(bouton_rond, NULL, ecran, &bou);

    }

    SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 0, 0, 0));
    SDL_BlitSurface(derou, NULL, ecran, &cur);
    SDL_BlitSurface(bouton_rond,NULL,ecran, &bou);

        SDL_BlitSurface(rectangle,NULL,ecran,&barre);
         SDL_BlitSurface(rectn, NULL, ecran, &rec);
         SDL_BlitSurface(play, NULL, ecran, &pla);
          SDL_Flip(ecran);


}
    SDL_FreeSurface (rectangle);
    SDL_FreeSurface(bouton_rond);
    SDL_FreeSurface(derou);
    SDL_FreeSurface (rectn);
    SDL_FreeSurface(play);

    SDL_Quit();

    return EXIT_SUCCESS;
}



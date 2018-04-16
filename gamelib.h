#include <stdio.h>

enum Tipo_zona{cucina= 1,soggiorno,rimessa,strada,lungo_lago,uscita_campeggio};
enum Tipo_oggetto{cianfrusaglia=0,bende,coltello,pistola,benzina,adrenalina};
enum Stato_Giocatore{ vivo,ferito,morto};

struct Giocatore{
enum Stato_Giocatore stato;
struct Zona* posizione;
enum Tipo_oggetto zaino[6];
};

struct Zona{
enum Tipo_zona zona;
enum Tipo_oggetto oggetto;
struct Zona* zona_successiva;
};

typedef struct Zona stZona;
typedef struct Giocatore stGiocatore;

void crea_mappa();
void gioca();
void termina_gioco();
void regole();
int controllo_input();

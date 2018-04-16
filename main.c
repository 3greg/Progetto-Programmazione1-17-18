#include <stdio.h>
#include <stdlib.h>
#include "gamelib.h"

int main (){
int scelta;
extern int end_mappa;
	system("clear");
		do{
      	printf("\t\t**************************************************************************** \n");
      	printf("\t\t*                                                                          *\n");
      	printf("\t\t*                                GIESON                                    *\n");
      	printf("\t\t*                               IL GIOCO                                   *\n");
      	printf("\t\t*                                                                          *\n");
        printf("\t\t*                           1 --> Crea Mappa                               *\n");
        printf("\t\t*                           2 --> Gioca                                    *\n");
        printf("\t\t*                           3 --> Termina Gioco                            *\n");
        printf("\t\t*                           4 --> Regole                                   *\n");
      	printf("\t\t****************************************************************************\n");
        printf("\n");
        printf("--> SCEGLI UNA DELLE SEGUENTE AZIONI\n");
			scanf("%d", &scelta);
			if(controllo_input()==0){
				system("clear");
			switch(scelta){
					case 1:
						printf("AZIONE SELEZIONATA --> CREA MAPPA\n");
						crea_mappa();
						break;
					case 2:
						printf("AZIONE SELEZIONATA --> GIOCA\n");
						if(end_mappa){
							gioca();
						}else{
							printf("ATTENZIONE --> CREARE MAPPA\n");
						}
						break;
					case 3:
						printf("AZIONE SELEZIONATA --> TERMINA GIOCO\n");
						termina_gioco();
						break;
					case 4:
						regole(); // --> aggiunto l'opzione di visionare le regole del gioco
						break;
					default:
						printf("ATTENZIONE --> VALORE INSERITO NON VALIDO\n");
						break;
					}
			}else{
				continue;
			}

		}while(scelta!=3);
	}

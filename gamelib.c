#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "gamelib.h"
#include <string.h>


static int conta_benz=4; //variabile che conta i turni rimasti quando utilizzo benzina
static int conta;
static int close;

int end_mappa=0;


static int mat[6][6] = {  	{30,20,40,0,0,10  },  //matrice probabilita
												 		{20,10,10,30,0,30 },
											   		{20,10,30,0,30,10 },
											 	 		{80,0,10,0,10,10  },
											   		{70,0,10,0,20,10  },
											 	 		{90,0,10,0,0,0    } };




//static stZona* app= NULL;
static stZona* prima_zona= NULL;
static stZona* ultima_zona= NULL;

static stGiocatore marzia;
static stGiocatore giacomo;
static stGiocatore *giocatore;

//funzioni menu crea mappa
static void ins_zona();
static void canc_zona();
static void stampa_mappa();
static void chiudi_mappa();

static int oggetto_zona(); //funzione che determina l'oggetto in una zona

//funzioni menu gioca
static int avanza();
static int mostra_oggetto(int *turno_1, int *mostrato_g, int *mostrato_m);
static int prendi_oggetto();
static int cura();
static int usa_adrenalina();
int uso_adrenalina=0; //variabile che utilizzo per verificare se adrenalina e'in uso


static void change_turno(int *turno_2);

static int gieson();
static void difesa();
static int difesa_pistola();
static int difesa_coltello();
static int difesa_benzina();
int uso_benzina=0; //variabileche utilizzo per verificare se benzina é stata usata

static void vittoria();
static void sconfitta();
static void stampa_oggetto(enum Tipo_oggetto ogg);
static void stampa_zona(enum Tipo_zona zona);



void regole(){

printf("\t\t\t██████╗ ███████╗ ██████╗  ██████╗ ██╗     ███████╗\n");
printf("\t\t\t██╔══██╗██╔════╝██╔════╝ ██╔═══██╗██║     ██╔════╝\n");
printf("\t\t\t██████╔╝█████╗  ██║  ███╗██║   ██║██║     █████╗  \n");
printf("\t\t\t██╔══██╗██╔══╝  ██║   ██║██║   ██║██║     ██╔══╝  \n");
printf("\t\t\t██║  ██║███████╗╚██████╔╝╚██████╔╝███████╗███████╗\n");
printf("\t\t\t╚═╝  ╚═╝╚══════╝ ╚═════╝  ╚═════╝ ╚══════╝╚══════╝\n");


printf("\t\t******************************************************************************* \n");
printf("\t\t*                                                                             *\n");
printf("\t\t*        Benvenuto nel gioco ... ti illusteró brevemente le regole!!          *\n");
printf("\t\t*                                                                             *\n");
printf("\t\t* Una persona prima di iniziare il gioco crea una mappa che due giocatori     *\n");
printf("\t\t* non conoscono , tale mappa dovrá contenere minimo 8 zone, ogni zona ha un   *\n");
printf("\t\t* oggetto, l'ultima zona della mappa dovrá essere l'uscita del campeggio.     *\n");
printf("\t\t* L'obbiettivo é sopravvivere alla comporsa di Gieson, ti potrai difendere    *\n");
printf("\t\t* con diverse armi :coltello, pistola e benzina ; quest'ultima scaccia        *\n");
printf("\t\t* Gieson per 4 mosse (cumulative).Puoi usufruire di adrenalina che permette   *\n");
printf("\t\t* al giocatore di compiere due mosse consecutive. Prima di prendere un oggetto*\n");
printf("\t\t* devi mostrarlo, una volta mostrato potrai prenderlo.                        *\n");
printf("\t\t*                                                                             *\n");
printf("\t\t*                    	che il gioco abbia inizio!!	              						  *\n");
printf("\t\t*                                                                             *\n");
printf("\t\t*******************************************************************************\n");
}
void crea_mappa(){
	static int scelta1;
	system("clear");
	if(end_mappa!=1){
	do{
    printf("\t\t**************************************************************************** \n");
    printf("\t\t*                                                                          *\n");
    printf("\t\t*\t\tScegli tra una delle seguenti opzioni:\t\t\t   *");
    printf("\n\t\t*\t\t1.Crea nuova zona       \t\t\t\t   *");
    printf("\n\t\t*\t\t2.Cancella ultima zona       \t\t\t\t   *");
    printf("\n\t\t*\t\t3.Mostra mappa \t\t\t\t\t\t   *");
    printf("\n\t\t*\t\t4.Termina creazione mappa \t\t\t\t   *");
    printf("\n\t\t**************************************************************************\n\n");
		scanf("%d", &scelta1);
		if(controllo_input()==0){
			system("clear");
			switch(scelta1){
					case 1:
						printf("#1 --> Crea nuova zona della mappa\n\n");
						ins_zona(); // --> opzione 1 inserire nuova zona
						break;
					case 2:
						printf("#2 --> Cancella Ultima Terra\n\n");
						canc_zona(); // --> opzione 2 cancellazione ultima zona
						break;
					case 3:
						printf("#3 --> Stampa mappa\n\n");
						stampa_mappa(); // opzione 3 --> stampa mappa creata
						break;
					case 4:
						printf("#4 --> Termina Creazione Percorso\n\n");
						chiudi_mappa(); // opzione 4 --> chiudi mappa
						break;
					default:
						printf("ATTENZIONE! VALORE INSERITO NON VALIDO!\n\n"); // se l'utente inserire un valore diverso da quelli sopra elencati verrá visualizzato un messaggio di errore
						break;
					}
		}else{
			continue;
		}
		}
		while(!((scelta1>0) && (scelta1<5)) || !(end_mappa==1)); //il ciclo viene iterato finche l'utente non inserisce un valore valido
	}else{
		printf("ATTENZIONE --> MAPPA GIÀ CREATA\n");
	}
}


static int oggetto_zona(enum Tipo_zona tzona){  //tzona --> tipo zona come parametro
int riga_oggetto[6]; //array di 6 oggetto --> di ogni riga
int uscita_ogg; //numero che mi serve per determinare l'oggetto che sara presente nella zona

for(int j=0;j<6;j++){
		riga_oggetto[j]=mat[tzona-1][j];
}

time_t ti;
srand((unsigned) time(&ti));
uscita_ogg=(rand()%101);
int flag= 1;

while(flag){ // ciclo finche non esce un valore valido
	switch(tzona){
		case 1:
			if((uscita_ogg>0)&&(uscita_ogg<=10)){
				flag= 0;
				return adrenalina;
			}else if((uscita_ogg>10)&&(uscita_ogg<=30)){
				flag= 0;
				return bende;
			}else if ((uscita_ogg>30)&&(uscita_ogg<=60)){
				flag= 0;
				return cianfrusaglia;
			}else if((uscita_ogg>60)&&(uscita_ogg<=100)){
				flag= 0;
				return coltello;
			}else{
				flag= 1;
				uscita_ogg=(rand()%101);
			}
			break;

		case 2 :
			if((uscita_ogg>0)&&(uscita_ogg<=10)){
				flag= 0;
				return bende;
			}else if((uscita_ogg>10)&&(uscita_ogg<=20)){
				flag= 0;
				return coltello;
			}else if ((uscita_ogg>20)&&(uscita_ogg<=40)){
				flag= 0;
				return cianfrusaglia;
			}else if((uscita_ogg>40)&&(uscita_ogg<=70)){
				flag= 0;
				return pistola;
			}else if ((uscita_ogg>70)&&(uscita_ogg<=100)){
				flag= 0;
				return adrenalina;
			}else{
				flag=1;
				uscita_ogg=(rand()%101);
			}
			break;

		case 3:
			if((uscita_ogg>0)&&(uscita_ogg<=10)){
				flag= 0;
				return bende;
			}else if((uscita_ogg>10)&&(uscita_ogg<=20)){
				flag= 0;
				return adrenalina;
			}else if ((uscita_ogg>20)&&(uscita_ogg<=40)){
				flag= 0;
				return cianfrusaglia;
			}else if((uscita_ogg>40)&&(uscita_ogg<=70)){
				flag= 0;
				return coltello;
			}else if ((uscita_ogg>70)&&(uscita_ogg<=100)){
				flag= 0;
				return benzina;
			}else{
					flag=1;
					uscita_ogg=(rand()%101);
			}
			break;

		case 4:
			if((uscita_ogg>0)&&(uscita_ogg<=10)){
				flag= 0;
				return coltello;
			}else if((uscita_ogg>10)&&(uscita_ogg<=20)){
				flag= 0;
				return benzina;
			}else if ((uscita_ogg>20)&&(uscita_ogg<=100)){
				flag= 0;
				return strada;
			}else{
				flag=1;
				uscita_ogg=(rand()%101);
			}
			break;

		case 5:
			if((uscita_ogg>0)&&(uscita_ogg<=10)){
				flag= 0;
				return coltello;
			}else if((uscita_ogg>10)&&(uscita_ogg<=30)){
				flag= 0;
				return benzina;
			}else if ((uscita_ogg>30)&&(uscita_ogg<=100)){
				flag= 0;
				return cianfrusaglia;
			}else{
				flag= 1;
				uscita_ogg=(rand()%101);
			}
			break;



		case 6:
			if((uscita_ogg>0)&&(uscita_ogg<=10)){
				flag= 0;
				return coltello;
			}else if((uscita_ogg>10)&&(uscita_ogg<=100)){
				flag= 0;
				return cianfrusaglia;
			}else{
				flag= 1;
				uscita_ogg=(rand()%101);
			}
			break;
		}
}
return 0;
}



static void ins_zona(){ //funzione per inserimento nuova zona della mappa

if (prima_zona== NULL){
		prima_zona=(stZona*) malloc(sizeof(stZona));    //crea la prima cella di memoria
		ultima_zona = prima_zona;   										//in questo caso prima e ultima zona coincidono
	}else{
			ultima_zona->zona_successiva = (stZona*) malloc(sizeof(stZona));  //se la mappa non é vuota ne aggiungo un'altra
			ultima_zona = ultima_zona->zona_successiva;                  			// ultima_terra quindi e' la terra che sto aggiungendo
			ultima_zona->zona_successiva = NULL;                         			// puntatore a terra_successiva messo a NULL
	}
	static int scelta2;
	do{
    printf("\t\t****************************************************************************\n");
    printf("\t\t*                                                                          *\n");
    printf("\t\t*\t\tScegli che tipo di zona inserire:\t\t\t   *");
    printf("\n\t\t*\t\t1.Cucina \t\t\t\t\t\t   *");
    printf("\n\t\t*\t\t2.Soggiorno       \t\t\t\t\t   *");
		printf("\n\t\t*\t\t3.Rimessa\t\t\t\t\t\t   *");
    printf("\n\t\t*\t\t4.Strada\t\t\t\t\t\t   *");
    printf("\n\t\t*\t\t5.Lungo lago\t\t\t\t\t\t   *");
    printf("\n\t\t*\t\t6.Uscita campeggio\t\t\t\t\t   *");
    printf("\n\t\t**************************************************************************\n\n");
		scanf("%d", &scelta2);    //scelgo che tipo di terra INSERIRE
		if(controllo_input()==0){
			conta=conta+1;

			system("clear");
			switch(scelta2){
				case 1:
					printf("--> Creazione zona: Cucina\n");
					stampa_oggetto((enum Tipo_oggetto)oggetto_zona(scelta2));
					break;


				case 2 :
					printf("--> Creazione zona: Soggiorno\n");
					stampa_oggetto((enum Tipo_oggetto)oggetto_zona(scelta2));
					break;


				case 3:
					printf("--> Creazione zona: Rimessa\n");
					stampa_oggetto((enum Tipo_oggetto)oggetto_zona(scelta2));
					break;


				case 4:
					printf("--> Creazione zona: Strada\n");
					stampa_oggetto((enum Tipo_oggetto)oggetto_zona(scelta2));
					break;



				case 5:
					printf("--> Creazione zona: Lungo lago\n");
					stampa_oggetto((enum Tipo_oggetto)oggetto_zona(scelta2));
					break;


	      case 6:
	  			printf("--> Creazione zona: Uscita campeggio\n");
					stampa_oggetto((enum Tipo_oggetto)oggetto_zona(scelta2));
	  		  break;

				}
		}else{
			continue;
				}
		}while(scelta2!=1 && scelta2!=2 && scelta2!=3 && scelta2!=4 && scelta2!=5 && scelta2!=6);
    ultima_zona->zona=scelta2; 														//il tipo zona dell'ultima zona inserita prende il tipo di scelta2
		ultima_zona->oggetto=oggetto_zona(scelta2); 					//assegno al campo oggetto --> oggetto_zona(scelta2)
}





static void canc_zona(){
    if(prima_zona==NULL){ 																					//se non ho inserito zone --> messaggio errore
      printf("ATTENZIONE --> NON SONO STATE INSERITE ZONE!\n");
    }else if(prima_zona->zona_successiva==NULL){										//ne cancello solo una
      prima_zona=NULL;
      free(prima_zona);
			printf("Cancellazione zona...\n");
    }else{
      stZona *tmp=prima_zona;																				//gestico i puntatori e le zone
      stZona *com=NULL;
			printf("Cancellazione zona...\n");
      do{
          if(tmp->zona_successiva==ultima_zona){
            com=tmp;
            break;
          }else{
            tmp=tmp->zona_successiva;
          }

      }while((tmp->zona_successiva)!=NULL);
      free(com->zona_successiva); 																//libero memoria ultima posizione
      com->zona_successiva=NULL;
      ultima_zona=com;
			conta=conta-1;																							//decremento conta che aggiorna il numero di zone inserite
    }
}



static void stampa_mappa(){
	int now=1;
  if(prima_zona==NULL){
   printf("ATTENZIONE --> MAPPA VUOTA!!\n");
 }else{
   stZona *comod=prima_zona;
   printf("-------    STRUTTURA MAPPA    -------\n");
	 printf("\n");
     do{
       printf("-------- INFO ZONA NUMERO: %d: -------\n",now);
			 printf("--> Indirizzo zona: %p\n",comod);
			 stampa_zona(comod->zona);
			 stampa_oggetto(comod->oggetto);
       printf("--> Zona successiva: %p\n",comod->zona_successiva);
       comod=comod->zona_successiva;
			 now=now+1;
			 printf("\n");
     }while(comod != NULL);
 }
}

static void chiudi_mappa(){
  if(conta<8){
  printf("ATTENZIONE --> INSERIRE ALMENO 8 ZONE!!\n");						//la mappa deve contenere almeno 8 zone
	printf("\n");
}else{
	do {
		printf("-------- Chiudi Mappa: -------\n");
		printf("1 --> SI\n");
		printf("2 --> NO\n");
		scanf("%d",&close);
		if(controllo_input()==0){
			system("clear");
			switch(close){
				case 1:
					if(ultima_zona->zona==uscita_campeggio){									//l'ultima zona della mappa deve essere uscita_campeggio
								end_mappa=1;																				// --> chiudo la mappa se é tutto ok (end_mappa=1)
								printf("ATTENZIONE --> MAPPA COMPLETATA\n");
							}else if(ultima_zona->zona!=uscita_campeggio){
								printf("ATTENZIONE --> USCITA CAMPEGGIO MANCANTE!\n");
								end_mappa=0;																				// --> non chiudo la mappa
							}
					break;
				case 2:
					printf("CHIUSURA MAPPA NON EFFETTUATA\n");
					break;
				default :
					printf("ATTENZIONE --> VALORE INSERITO NON VALIDO!!\n\n");
					break;
			}
		}else{
			continue;
		}
	}while(close!=1 && close!=2); //ciclo finche non inserisco o 1 o 2
}
}







//menu gioca
void gioca(){
	int turno;
	int vai=0; // variabile a 0 se non eseguo l'azione, a 1 se viene eseguita
	int scelta3; //variabile che serve per scegliere quale azione l'utente vuole fare
	int mostrato_giacomo=0; // flag per verificare se l'oggetto in una zona e' stato mostrato, in tal caso puo essere preso
	int mostrato_marzia=0;

	//iniziallizzo i due gioactori
	marzia.stato=vivo;
	marzia.posizione=prima_zona;
	marzia.zaino[adrenalina]=2;


	giacomo.stato=vivo;
	giacomo.posizione=prima_zona;
	giacomo.zaino[coltello]=1;



	//randomicamente scelgo chi comincia
	if(rand()%101>50){
	turno=1; //turno di giacomo
	giocatore=&giacomo;
	printf("TURNO DI --> GIACOMO\n");
			}else{
				turno=0; //turno di marzia
				giocatore=&marzia;
				printf("TURNO DI --> MARZIA\n");
			}

	do{
    printf("\t\t****************************************************************************\n");
    printf("\t\t*                                                                          *\n");
    printf("\t\t*\t\tScegli tra una delle seguenti opzioni:\t\t\t   *");
    printf("\n\t\t*\t\t1.Avanza \t\t\t\t\t\t   *");
    printf("\n\t\t*\t\t2.Mostra oggetto        \t\t\t\t   *");
    printf("\n\t\t*\t\t3.Prendi oggetto\t\t\t\t\t   *");
    printf("\n\t\t*\t\t4.Cura\t\t\t\t\t\t\t   *");
		printf("\n\t\t*\t\t5.Usa adrenalina\t\t\t\t\t   *");
    printf("\n\t\t***************************************************************************\n\n");
		scanf("%d", &scelta3);
		if(controllo_input()==0){
			system("clear");
			switch(scelta3){
					case 1:
						printf("#1 --> Avanza nella mappa\n");
						vai=avanza(); // --> opzione 1 avanza nella mappa
						break;
					case 2:
						printf("#2 --> Mostra oggetto\n");
						vai=mostra_oggetto(&turno, &mostrato_giacomo, &mostrato_marzia); // --> opzione 2 mostra oggetto presente nella zona
						break;
					case 3:
						printf("#3 --> Prendi oggetto\n");
						vai= prendi_oggetto(&turno, &mostrato_giacomo, &mostrato_marzia); // opzione 3 --> prendi oggetto presente nella zona
						break;
					case 4:
						printf("#4 --> Cura\n");
						vai=cura(); // opzione 4 --> permette uso delle bende(se presenti nello zaino), per irportare lo stato a ferito o vivo
						break;
					case 5:
						printf("#5 --> Usa adrenalina\n");
						vai=usa_adrenalina(); //opzione 5 --> permette di effettuare due mosse (-1 unita di adrenalina)
						break;
					default:
						printf("ATTENZIONE! VALORE INSERITO NON VALIDO!\n\n"); // se l'utente inserire un valore diverso da quelli sopra elencati verrá visualizzato un messaggio di errore
						vai= 0;
						break;
				}
		}else{
			continue;
		}


			if(giocatore->stato!=morto || giocatore->posizione!=NULL){ //controllo che il giocatore non sia morto o uscito
				if((vai==1)&&(uso_adrenalina>0)&&(uso_benzina==1)){ //controllo se usa benzina e adrenalina insieme
					printf("TURNI RIMANENTI CON ADRENALINA --> %d\n",uso_adrenalina); // se uso adenalina (=2) non cambio turno
					conta_benz--;
					printf("TURNI SENZA GIESON --> %d\n",conta_benz);
					if(conta_benz==0){
						uso_benzina=0;
						conta_benz=4;
					}
					uso_adrenalina--;
				}else if((vai==1)&&(uso_benzina==1)){ // controllo se usa solo benzina
								conta_benz--;
								printf("TURNI SENZA GIESON --> %d\n",conta_benz);
								if(conta_benz==0){
									uso_benzina=0;
									conta_benz=4;
								}
								change_turno(&turno);  //cambio turno
				}else if((vai==1)&&(uso_adrenalina>0)){ //controllo se usa adrenalina
					printf("TURNI RIMANENTI CON ADRENALINA --> %d\n",uso_adrenalina);
					uso_adrenalina--;
				}else if(vai==1){ //se l'azione è stata eseguita con successo controllo se gieson compare
					if(gieson()==1){
						difesa();
					}
					change_turno(&turno);
				}
			}else{
				change_turno(&turno);
			}

	}while(!((scelta3>0) && (scelta3<6)) || (1)); //il ciclo viene iterato finche l'utente non inserisce un valore valido
}

static int avanza(){
	int vai1=1; //guardo se l'azione viene eseguita con successo
	if(giocatore->posizione->zona_successiva!=NULL){
		//se la zona successiva  non e' NULL allora non sei uscito dal campeggio
		printf("AVANZAMENTO --> ZONA SUCCESSIVA!!\n");
		giocatore->posizione=giocatore->posizione->zona_successiva; // se va tutto bene il giocatore procede alla zona successiva
	}else{
		giocatore->posizione=NULL;
		printf("SEI USCITO DAL CAMPING LAKE TRASYMENO!!\n");
		vittoria();
	}
	return vai1;
}



static int mostra_oggetto(int *turno_1, int *mostrato_g, int *mostrato_m){ // aggiungere flag mostrato per la funzione prendi oggetto
	int vai2=1; // come nella funzione precedente guardo se l'azione viene esguita con successo
	printf("-------- Mostra oggetto: -------\n");
	switch (giocatore->posizione->oggetto) {

		// in ogni caso guardo che marzia o giacomo abbiamo mostrato l'oggetto
		case 0:
		if(*turno_1==0){
			*mostrato_m=1;
		}else if(*turno_1==1){
			*mostrato_g=1;
		}
		printf("Cianfrusaglia\n");
		break;


		case 1:
		if(*turno_1==0){
			*mostrato_m=1;
		}else if(*turno_1==1){
			*mostrato_g=1;
		}
		printf("Bende\n");
		break;


		case 2:
		if(*turno_1==0){
			*mostrato_m=1;
		}else if(*turno_1==1){
			*mostrato_g=1;
		}
		printf("Coltello\n");
		break;


		case 3:
		if(*turno_1==0){
			*mostrato_m=1;
		}else if(*turno_1==1){
			*mostrato_g=1;
		}
		printf("Pistola\n");
		break;


		case 4:
		if(*turno_1==0){
			*mostrato_m=1;
		}else if(*turno_1==1){
			*mostrato_g=1;
		}
		printf("Benzina\n");
		break;


		case 5:
		if(*turno_1==0){
			*mostrato_m=1;
		}else if(*turno_1==1){
			*mostrato_g=1;
		}
		printf("Adrenalina\n");
		break;
	}
	return vai2;

}





static int prendi_oggetto(int *turno_1, int *mostrato_g, int *mostrato_m){
	int vai3=1;
if(*turno_1==0 && *mostrato_m==1){  // se  e' stato mostrato da marzia
	giocatore->zaino[giocatore->posizione->oggetto]++;
	printf("OGGETTO PRESO --> ");
	switch (giocatore->posizione->oggetto){
		case 0:
		printf("Cianfrusaglia\n");
		break;
		case 1:
		printf("Bende\n");
		break;
		case 2:
		printf("Coltello\n");
		break;
		case 3:
		printf("Pistola\n");
		break;
		case 4:
		printf("Benzina\n");
		break;
		case 5:
		printf("Adrenalina\n");
		break;
	}
	giocatore->posizione->oggetto= cianfrusaglia;
	*mostrato_m=0;
	vai3=1;


}else if(*turno_1==1 && *mostrato_g==1){ // se  e' stato mostrato da giacomo
	giocatore->zaino[giocatore->posizione->oggetto]++;
	printf("OGGETTO PRESO --> ");
	switch (giocatore->posizione->oggetto){
		case 0:
		printf("Cianfrusaglia\n");
		break;
		case 1:
		printf("Bende\n");
		break;
		case 2:
		printf("Coltello\n");
		break;
		case 3:
		printf("Pistola\n");
		break;
		case 4:
		printf("Benzina\n");
		break;
		case 5:
		printf("Adrenalina\n");
		break;
	}
	giocatore->posizione->oggetto= cianfrusaglia;
	mostrato_g=0;
	vai3=1;


}else if ((*turno_1==0 && *mostrato_m==0) || (*turno_1==1 && *mostrato_g==0)){
	printf("ATTENZIONE --> OGGETTO NON MOSTRATO\n");
	vai3=0;
}
return vai3;

}


static int cura(){
	int vai4=1;
	if(giocatore->stato==ferito && giocatore->zaino[bende]>0){ //controllo lo stato del giocatore
		printf("--> CURA CON BENDE IN CORSO ...\n");
		giocatore->stato=vivo; //riporto lo stato del giocatore a vivo
		giocatore->zaino[bende]--; // tolgo una unita di bende
		vai4=1;
	}else if(giocatore->stato==vivo){  // se e' vivo non si cura ma non cambio turno
		printf("--> SALUTE AL MASSIMO\n");
		vai4=0;
	}else{
		printf("--> IMPOSSIBILE CURARSI\n");
		vai4=0;
	}
	return vai4;

}



static int gieson(){
	int uscita_geison;
	uscita_geison=(rand()%101);
	if((giocatore->posizione==NULL) || (giocatore->stato==morto)){
		return 0;
	}else if(giocatore->posizione->zona==uscita_campeggio && (uscita_geison>0 && uscita_geison<=75)){ // se il giocatore si trova in uscita campeggio possibilita di comparsa gieson --> 75%
		  printf("\n");
			printf("\t\t\t\t ▄▄▄▄▄▄▄▄▄▄▄  ▄▄▄▄▄▄▄▄▄▄▄  ▄▄▄▄▄▄▄▄▄▄▄  ▄▄▄▄▄▄▄▄▄▄▄  ▄▄▄▄▄▄▄▄▄▄▄  ▄▄        ▄ \n");
			printf("\t\t\t\t▐░░░░░░░░░░░▌▐░░░░░░░░░░░▌▐░░░░░░░░░░░▌▐░░░░░░░░░░░▌▐░░░░░░░░░░░▌▐░░▌      ▐░▌\n");
			printf("\t\t\t\t▐░█▀▀▀▀▀▀▀▀▀  ▀▀▀▀█░█▀▀▀▀ ▐░█▀▀▀▀▀▀▀▀▀ ▐░█▀▀▀▀▀▀▀▀▀ ▐░█▀▀▀▀▀▀▀█░▌▐░▌░▌     ▐░▌\n");
			printf("\t\t\t\t▐░▌               ▐░▌     ▐░▌          ▐░▌          ▐░▌       ▐░▌▐░▌▐░▌    ▐░▌\n");
			printf("\t\t\t\t▐░▌ ▄▄▄▄▄▄▄▄      ▐░▌     ▐░█▄▄▄▄▄▄▄▄▄ ▐░█▄▄▄▄▄▄▄▄▄ ▐░▌       ▐░▌▐░▌ ▐░▌   ▐░▌\n");
			printf("\t\t\t\t▐░▌▐░░░░░░░░▌     ▐░▌     ▐░░░░░░░░░░░▌▐░░░░░░░░░░░▌▐░▌       ▐░▌▐░▌  ▐░▌  ▐░▌\n");
			printf("\t\t\t\t▐░▌ ▀▀▀▀▀▀█░▌     ▐░▌     ▐░█▀▀▀▀▀▀▀▀▀  ▀▀▀▀▀▀▀▀▀█░▌▐░▌       ▐░▌▐░▌   ▐░▌ ▐░▌\n");
			printf("\t\t\t\t▐░▌       ▐░▌     ▐░▌     ▐░▌                    ▐░▌▐░▌       ▐░▌▐░▌    ▐░▌▐░▌\n");
			printf("\t\t\t\t▐░█▄▄▄▄▄▄▄█░▌ ▄▄▄▄█░█▄▄▄▄ ▐░█▄▄▄▄▄▄▄▄▄  ▄▄▄▄▄▄▄▄▄█░▌▐░█▄▄▄▄▄▄▄█░▌▐░▌     ▐░▐░▌\n");
			printf("\t\t\t\t▐░░░░░░░░░░░▌▐░░░░░░░░░░░▌▐░░░░░░░░░░░▌▐░░░░░░░░░░░▌▐░░░░░░░░░░░▌▐░▌      ▐░░▌\n");
			printf("\t\t\t\t ▀▀▀▀▀▀▀▀▀▀▀  ▀▀▀▀▀▀▀▀▀▀▀  ▀▀▀▀▀▀▀▀▀▀▀  ▀▀▀▀▀▀▀▀▀▀▀  ▀▀▀▀▀▀▀▀▀▀▀  ▀        ▀▀ \n");
			printf("\n");
			return 1;
		}else if((giocatore->stato==vivo || giocatore->stato==ferito) && (giocatore->posizione!=NULL) && (uscita_geison>0 &&uscita_geison<=30)){ //se tutti due vivi e dentro al campeggio probabilita di comparsa gieson --> 30%
			printf("\n");
			printf("\t\t\t\t ▄▄▄▄▄▄▄▄▄▄▄  ▄▄▄▄▄▄▄▄▄▄▄  ▄▄▄▄▄▄▄▄▄▄▄  ▄▄▄▄▄▄▄▄▄▄▄  ▄▄▄▄▄▄▄▄▄▄▄  ▄▄        ▄ \n");
			printf("\t\t\t\t▐░░░░░░░░░░░▌▐░░░░░░░░░░░▌▐░░░░░░░░░░░▌▐░░░░░░░░░░░▌▐░░░░░░░░░░░▌▐░░▌      ▐░▌\n");
			printf("\t\t\t\t▐░█▀▀▀▀▀▀▀▀▀  ▀▀▀▀█░█▀▀▀▀ ▐░█▀▀▀▀▀▀▀▀▀ ▐░█▀▀▀▀▀▀▀▀▀ ▐░█▀▀▀▀▀▀▀█░▌▐░▌░▌     ▐░▌\n");
			printf("\t\t\t\t▐░▌               ▐░▌     ▐░▌          ▐░▌          ▐░▌       ▐░▌▐░▌▐░▌    ▐░▌\n");
			printf("\t\t\t\t▐░▌ ▄▄▄▄▄▄▄▄      ▐░▌     ▐░█▄▄▄▄▄▄▄▄▄ ▐░█▄▄▄▄▄▄▄▄▄ ▐░▌       ▐░▌▐░▌ ▐░▌   ▐░▌\n");
			printf("\t\t\t\t▐░▌▐░░░░░░░░▌     ▐░▌     ▐░░░░░░░░░░░▌▐░░░░░░░░░░░▌▐░▌       ▐░▌▐░▌  ▐░▌  ▐░▌\n");
			printf("\t\t\t\t▐░▌ ▀▀▀▀▀▀█░▌     ▐░▌     ▐░█▀▀▀▀▀▀▀▀▀  ▀▀▀▀▀▀▀▀▀█░▌▐░▌       ▐░▌▐░▌   ▐░▌ ▐░▌\n");
			printf("\t\t\t\t▐░▌       ▐░▌     ▐░▌     ▐░▌                    ▐░▌▐░▌       ▐░▌▐░▌    ▐░▌▐░▌\n");
			printf("\t\t\t\t▐░█▄▄▄▄▄▄▄█░▌ ▄▄▄▄█░█▄▄▄▄ ▐░█▄▄▄▄▄▄▄▄▄  ▄▄▄▄▄▄▄▄▄█░▌▐░█▄▄▄▄▄▄▄█░▌▐░▌     ▐░▐░▌\n");
			printf("\t\t\t\t▐░░░░░░░░░░░▌▐░░░░░░░░░░░▌▐░░░░░░░░░░░▌▐░░░░░░░░░░░▌▐░░░░░░░░░░░▌▐░▌      ▐░░▌\n");
			printf("\t\t\t\t ▀▀▀▀▀▀▀▀▀▀▀  ▀▀▀▀▀▀▀▀▀▀▀  ▀▀▀▀▀▀▀▀▀▀▀  ▀▀▀▀▀▀▀▀▀▀▀  ▀▀▀▀▀▀▀▀▀▀▀  ▀        ▀▀ \n");
			printf("\n");
			return 1;
		}else if(((giacomo.stato==morto || marzia.stato==morto) && (uscita_geison>0 &&uscita_geison<=50))){ // se uno dei due é morto probabilita di comparsa gieson --> 50%
			printf("\n");
			printf("\t\t\t\t ▄▄▄▄▄▄▄▄▄▄▄  ▄▄▄▄▄▄▄▄▄▄▄  ▄▄▄▄▄▄▄▄▄▄▄  ▄▄▄▄▄▄▄▄▄▄▄  ▄▄▄▄▄▄▄▄▄▄▄  ▄▄        ▄ \n");
			printf("\t\t\t\t▐░░░░░░░░░░░▌▐░░░░░░░░░░░▌▐░░░░░░░░░░░▌▐░░░░░░░░░░░▌▐░░░░░░░░░░░▌▐░░▌      ▐░▌\n");
			printf("\t\t\t\t▐░█▀▀▀▀▀▀▀▀▀  ▀▀▀▀█░█▀▀▀▀ ▐░█▀▀▀▀▀▀▀▀▀ ▐░█▀▀▀▀▀▀▀▀▀ ▐░█▀▀▀▀▀▀▀█░▌▐░▌░▌     ▐░▌\n");
			printf("\t\t\t\t▐░▌               ▐░▌     ▐░▌          ▐░▌          ▐░▌       ▐░▌▐░▌▐░▌    ▐░▌\n");
			printf("\t\t\t\t▐░▌ ▄▄▄▄▄▄▄▄      ▐░▌     ▐░█▄▄▄▄▄▄▄▄▄ ▐░█▄▄▄▄▄▄▄▄▄ ▐░▌       ▐░▌▐░▌ ▐░▌   ▐░▌\n");
			printf("\t\t\t\t▐░▌▐░░░░░░░░▌     ▐░▌     ▐░░░░░░░░░░░▌▐░░░░░░░░░░░▌▐░▌       ▐░▌▐░▌  ▐░▌  ▐░▌\n");
			printf("\t\t\t\t▐░▌ ▀▀▀▀▀▀█░▌     ▐░▌     ▐░█▀▀▀▀▀▀▀▀▀  ▀▀▀▀▀▀▀▀▀█░▌▐░▌       ▐░▌▐░▌   ▐░▌ ▐░▌\n");
			printf("\t\t\t\t▐░▌       ▐░▌     ▐░▌     ▐░▌                    ▐░▌▐░▌       ▐░▌▐░▌    ▐░▌▐░▌\n");
			printf("\t\t\t\t▐░█▄▄▄▄▄▄▄█░▌ ▄▄▄▄█░█▄▄▄▄ ▐░█▄▄▄▄▄▄▄▄▄  ▄▄▄▄▄▄▄▄▄█░▌▐░█▄▄▄▄▄▄▄█░▌▐░▌     ▐░▐░▌\n");
			printf("\t\t\t\t▐░░░░░░░░░░░▌▐░░░░░░░░░░░▌▐░░░░░░░░░░░▌▐░░░░░░░░░░░▌▐░░░░░░░░░░░▌▐░▌      ▐░░▌\n");
			printf("\t\t\t\t ▀▀▀▀▀▀▀▀▀▀▀  ▀▀▀▀▀▀▀▀▀▀▀  ▀▀▀▀▀▀▀▀▀▀▀  ▀▀▀▀▀▀▀▀▀▀▀  ▀▀▀▀▀▀▀▀▀▀▀  ▀        ▀▀ \n");
			printf("\n");
			return 1;
	}
	return 0;
}


static void difesa(){
	if((giocatore->zaino[coltello]>0) || (giocatore->zaino[pistola]>0) || (giocatore->zaino[benzina]>0)){ //controllo se posso difendermi
	int scelta_difesa;
	int esito=0;
	printf("\t\t**************************************************************************** \n");
	printf("\t\t*                                                                          *\n");
	printf("\t\t*                   D I F E N D I T I   D A   G I E S O N                  *\n");
	printf("\t\t*                                                                          *\n");
	printf("\t\t*                           1 --> Coltello                                 *\n");
	printf("\t\t*                           2 --> Pistola                                  *\n");
	printf("\t\t*                           3 --> Benzina                                  *\n");
	printf("\t\t*                                                                          *\n");
	printf("\t\t****************************************************************************\n");


	do {
	scanf("%d",&scelta_difesa);
	if(controllo_input()==0){
		switch (scelta_difesa){
			case 1:
				if(giocatore->zaino[coltello]>0){
				esito=difesa_coltello();
				}else{
					printf("ATTENZIONE --> COLTELLO NON DISPONIBILE\n");
				}
			break;
			case 2:
				if(giocatore->zaino[pistola]>0){
				esito=difesa_pistola();
				}else{
					printf("ATTENZIONE --> PISTOLA NON DISPONIBILE\n");
				}
			break;
			case 3:
			if(giocatore->zaino[benzina]>0){
			esito=difesa_benzina();
			}else{
				printf("ATTENZIONE --> BENZINA NON DISPONIBILE\n");
			}
			break;
			default:
				printf("ATTENZIONE --> OGGETTO NON ESISTENTE\n");
			break;
		}
	}else{
		continue;
	}
} while(!(esito) && ((scelta_difesa!=1)||(scelta_difesa!=2)||(scelta_difesa!=3)));
}else{
	printf("NON PUOI DIFENDERTI DA GIESON --> SEI MORTO!!\n");
	giocatore->stato=morto;
}
}

// in base al tipo di difesa posso rimanere ferito o illeso
static int difesa_coltello(){
	if(giocatore->stato==ferito){
		giocatore->stato=morto;
		printf("--> SEI MORTO ...\n");
		return 1;
	}else if((giocatore->stato==vivo)&&(giocatore->zaino[coltello]!=0)){
		printf("--> SEI RIMASTO FERITO...\n");
		giocatore->stato=ferito;
		giocatore->zaino[coltello]--;
		return 1;
	}
	return 0;
}

static int difesa_pistola(){
	if((giocatore->zaino[pistola]!=0) && (giocatore->stato==vivo || giocatore->stato==ferito)){
		printf("--> SEI SOPRAVVISUTO...\n");
		giocatore->zaino[pistola]--;
		return 1;
	}else{
		printf("--> SEI MORTO...\n");
		giocatore->stato=morto;
		return 1;
	}
	return 0;
}

//con benzina invece allontano gieson per 4 mosse cumulative
static int difesa_benzina(){
	if((giocatore->zaino[benzina]!=0) && (giocatore->stato==vivo || giocatore->stato==ferito)){
		uso_benzina=1;
		printf("TURNI SENZA GIESON --> %d\n", conta_benz);
		giocatore->zaino[benzina]--;
		return 1;
	}else{
		printf("--> SEI MORTO...\n");
		giocatore->stato=morto;
		return 1;
	}
	return 0;
}


//funzione adrenalina --> non posso usare adrenalina se e' gia in uso
static int usa_adrenalina(){
if(uso_adrenalina>0){
	printf("ATTENZIONE --> ADRENALINA IN USO\n");
}else{
		if(giocatore->zaino[adrenalina]>0){
			 uso_adrenalina=2;
			 giocatore->zaino[adrenalina]--;
			 printf("USO ADRENALINA ATTIVO...\n");
			 return 1;
		}else{
			printf("ATTENZIONE --> ADRENALINA NON DISPONIBILE\n");
			uso_adrenalina=0;
			return 0;
		}
	}
	return 0;
}

void termina_gioco(){
	//libero memoria occupata dal gioco
	stZona *appoggio=prima_zona;
	stZona *comodo=NULL;

	if(prima_zona!=NULL){
		do {
			comodo=appoggio->zona_successiva;   //metto su comodo la zona successiva
			free(appoggio);
			appoggio=comodo;
		} while(comodo!=NULL);
		exit(1);
	}
}


//stampe
static void vittoria(){
	printf("\n");
	printf("\t\t\t\t██╗   ██╗ ██████╗ ██╗   ██╗    ██╗    ██╗██╗███╗   ██╗    ██╗\n");
	printf("\t\t\t\t╚██╗ ██╔╝██╔═══██╗██║   ██║    ██║    ██║██║████╗  ██║    ██║\n");
	printf("\t\t\t\t ╚████╔╝ ██║   ██║██║   ██║    ██║ █╗ ██║██║██╔██╗ ██║    ██║\n");
	printf("\t\t\t\t  ╚██╔╝  ██║   ██║██║   ██║    ██║███╗██║██║██║╚██╗██║    ╚═╝\n");
	printf("\t\t\t\t   ██║   ╚██████╔╝╚██████╔╝    ╚███╔███╔╝██║██║ ╚████║    ██╗\n");
	printf("\t\t\t\t   ╚═╝    ╚═════╝  ╚═════╝      ╚══╝╚══╝ ╚═╝╚═╝  ╚═══╝    ╚═╝\n");
	printf("\n");
}

static void sconfitta(){
	printf("\n");
	printf("\t\t ██████╗  █████╗ ███╗   ███╗███████╗     ██████╗ ██╗   ██╗███████╗██████╗ 	██╗\n");
	printf("\t\t██╔════╝ ██╔══██╗████╗ ████║██╔════╝    ██╔═══██╗██║   ██║██╔════╝██╔══██╗	██║\n");
	printf("\t\t██║  ███╗███████║██╔████╔██║█████╗      ██║   ██║██║   ██║█████╗  ██████╔╝	██║\n");
	printf("\t\t██║  ███╗███████║██╔████╔██║█████╗      ██║   ██║██║   ██║█████╗  ██████╔╝	██║\n");
	printf("\t\t██║   ██║██╔══██║██║╚██╔╝██║██╔══╝      ██║   ██║╚██╗ ██╔╝██╔══╝  ██╔══██╗	╚═╝\n");
	printf("\t\t╚██████╔╝██║  ██║██║ ╚═╝ ██║███████╗    ╚██████╔╝ ╚████╔╝ ███████╗██║  ██║	██╗\n");
	printf("\t\t ╚═════╝ ╚═╝  ╚═╝╚═╝     ╚═╝╚══════╝     ╚═════╝   ╚═══╝  ╚══════╝╚═╝  ╚═╝	╚═╝\n");
	printf("\n");
}

static void change_turno(int *turno_2){

	//gestisco i tunrni
	static int cambio_turno=1;
	if((giacomo.stato==morto) && (marzia.stato==morto)){
		sconfitta(); // se tutti 2 morti game over
		termina_gioco();
	}else if((giacomo.posizione==NULL)&&(marzia.posizione==NULL)){
		vittoria(); // se tutti due fuori vittoria
		termina_gioco();
	}else if(((giacomo.posizione==NULL)&&(marzia.stato==morto))||((marzia.posizione==NULL)&&(giacomo.stato==morto))){ //se uno fuori e uno morto stampo chi ha vinto
		if((giacomo.posizione==NULL) && (marzia.stato==morto)){
			printf("GIACOMO HA VINTO!!\n");
		}else if((marzia.posizione==NULL)&&(giacomo.stato==morto)){
			printf("MARZIA HA VINTO!!\n");
		}
		termina_gioco();
	}else{
			if((giacomo.stato==morto)||(giacomo.posizione==NULL)){ // se giacomo morto o fuori sempre turno di Marzia
				giocatore=&marzia;
				printf("TURNO DI --> MARZIA\n");
				*turno_2=0;
			}else if((marzia.stato==morto)||(marzia.posizione==NULL)){ // se marzia fuori o morta sempre turni di giacomo
				giocatore=&giacomo;
				printf("TURNO DI --> GIACOMO\n");
				*turno_2=1;
			}else{  // se sono vivi entrambi estraggo casualmente come lancio dei dadi chi comincia il turno
				if(cambio_turno%2==0){
					if(rand()%101>50){
					*turno_2=1; //turno di giacomo
					giocatore=&giacomo;
					printf("TURNO DI --> GIACOMO\n");
							}else{
								*turno_2=0; //turno di marzia
								giocatore=&marzia;
								printf("TURNO DI --> MARZIA\n");
							}
							cambio_turno=1;
				}else {
					if((*turno_2==0) && (giacomo.stato!=morto)){
						*turno_2=1;
						printf("TURNO DI --> GIACOMO\n");
						giocatore=&giacomo;
				}else if((*turno_2==1) && (marzia.stato!=morto)){
						*turno_2=0;
						printf("TURNO DI --> MARZIA\n");
						giocatore=&marzia;
				}
				cambio_turno=0;
			}
		}
	}
}


//controllo in input se inseisco valori non accettabili
int controllo_input(){
	char buff[100];
	fgets(buff,100,stdin);
		if(strlen(buff)>1){
			printf("ATTENZIONE --> VALORE INSERITO NON ACCETABILE\n");
			return 1;
		}else{
			return 0;
		}
}


//uso una funzione che mi converte il valore del Tipo oggetto nell'oggetto corrispondente
static void stampa_oggetto(enum Tipo_oggetto ogg){
	printf("--> Oggetto zona: ");
	switch (ogg){
		case 0:
		printf("Cianfrusaglia\n");
		break;
		case 1:
		printf("Bende\n");
		break;
		case 2:
		printf("Coltello\n");
		break;
		case 3:
		printf("Pistola\n");
		break;
		case 4:
		printf("Benzina\n");
		break;
		case 5:
		printf("Adrenalina\n");
		break;
}
}


//uso una funzione che mi converte il valore del Tipo zona nella zona
static void stampa_zona(enum Tipo_zona zona){
	printf("--> Tipo zona: ");
	switch (zona) {
	 case 1:
	 printf("Cucina\n");
	 break;
	 case 2:
	 printf("Soggiorno\n");
	 break;
	 case 3:
	 printf("Rimessa\n");
	 break;
	 case 4:
	 printf("Strada\n");
	 break;
	 case 5:
	 printf("Lungo lago\n");
	 break;
	 case 6:
	 printf("Uscita campeggio\n");
	 break;
	}
}

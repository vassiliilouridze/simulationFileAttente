#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define LGTABARRIVE 6
#define LGTABSERVICE 6
#define TPSSIMULATION 960

typedef struct client Client;
struct client
{
	int priorite;
	int dureeService;
	int tpsDansSysteme;
	Client *pClientSuiv;
};

void initTableauxValeurs(int tabLoiArriveeClient[], int tabLoiDesServices[]);
double calculLambda(int tabLoiArriveeClient[]);
double calculEsperanceDureeService(int tabLoiDesServices[]);
void calculStationsMaxMin(double lambda, double esperanceDS, int *stationMin, int *stationMax);
void generateurArrivees(int a, int c, int m, int *xn, Client **pClientDeb, Client**pClient, int tabLoiDesArriveesClient[]);
void calculNbStationsOptimal(int stationMin, int stationMax, double tabCoutTotal[]);
void sortieDifferentsCoutsEnvisages(int stationMin, int stationMax, double tabCoutTotal[]);

void main(void)
{
	int a = 137;
	int c = 187;
	int m = 256;
	int xn = 0;

	int stationMin;
	int stationMax;
	int station;

	double lambda;
	double esperanceDS;

	int tabLoiArriveeClient[LGTABARRIVE];
	int tabLoiDesServices[LGTABSERVICE];

	Client *pClientDeb = NULL;
	Client *pClient;

	initTableauxValeurs(tabLoiArriveeClient, tabLoiDesServices);

	int arriveTotalMaxSimulation = LGTABARRIVE * TPSSIMULATION;

	lambda = calculLambda(tabLoiArriveeClient);
	esperanceDS = calculEsperanceDureeService(tabLoiDesServices);
	calculStationsMaxMin(lambda, esperanceDS, stationMin, stationMax);

	//PAS BON A REMPLACER PAR UN POINTEUR
	Client tabStations[] = calloc(stationMax, sizeof(Client));

	double *tabCoutTotal[] = calloc(stationMax, sizeof(double));

	station = stationMin;

	while (station <= stationMax)
	{
		int temp = 0;
		Client fileAbsolue[] = calloc(arriveTotalMaxSimulation, sizeof(Client));
		Client fileRelatif[] = calloc(arriveTotalMaxSimulation, sizeof(Client));
		Client fileOrdinaire[] = calloc(arriveTotalMaxSimulation, sizeof(Client));
		int cumFileAbsolue = 0;
		int cumFileRelatif = 0;
		int cumFileOrdinaire = 0;
		int stationInnocupe = 0;

		//creerChainonClient(tabDS);

		while (temp < TPSSIMULATION)
		{
			generateurArrivees(a, c, m, &xn, &pClientDeb, &pClient, tabLoiArriveeClient);

			/*initTabStationsPourSortie(station,tabStationsPourSortie)
			et puis tout le reste pour les sorties*/

			int i = 0;

			while (i < station)
			{
				if (tabStations[i].dureeService = 0)
				{
					if (fileAbsolue != 0) {
						tabStations[i].priorite = fileAbsolue->priorite;
						tabStations[i].dureeService = fileAbsolue->dureeService;
						tabStations[i].dureeService--;
						/*client.tpsSysteme++;
						RETIRER LE POINTEUR DE LA FILE
						fileAbsolue--;*/
					}
					else
					{
						if (fileRelatif != 0)
						{

						}
						else
						{
							if (fileOrdinaire != 0)
							{

							}
							else
							{
								stationInnocupe++;
							}
						}
					}
				}
				else
				{
					/*tabStations[i].dureeService--;
					client.tpsSysteme++;*/
					//CA DEVRAIS ETRE 
					/*(*pClientStation)->dureeService--;
					(*pClientStation)->tpsSysteme--;*/
				}
				i++;
			}
			/*
			while sur les clients pour les durees totales des differentes files puis ajout aux cumul des tps dans le système
			*/
			if (temp < 20)
			{
				double coutATMin = (temp + 1)*(30 / 60) + (stationInnocupe)*(18 / 60) + (37, 5 / 60)*(cumFileAbsolue / temp) + (22, 5 / 60)*(cumFileRelatif / temp) + (22, 5 / 60)*(cumFileOrdinaire / temp);
				printf_s("cout à %s min = %lf", temp, coutATMin);
				//Sortir tabNbArrivés.length; WHY?
			}

			temp++;
		}

		tabCoutTotal[station - stationMin] = 16 * 30 + (stationInnocupe / 60) * 18 + 37, 5 * (cumFileAbsolue / 960) + 22, 5 * (cumFileRelatif / 960) + (22, 5)*(cumFileOrdinaire / 960);

		station++;

	}

	calculNbStationsOptimal(stationMin, stationMax, tabCoutTotal);
	sortieDifferentsCoutsEnvisages(stationMin, stationMax, tabCoutTotal);

	system("pause");
}

void initTableauxValeurs(int tabLoiArriveeClient[], int tabLoiDesServices[])
{
	for (int i = 0; i < 6; i++)
	{
		printf_s("Entrez la valeur des repétitions de la %d des arrivees clients : ", i);
		scanf_s("%d", &tabLoiArriveeClient[i]);
	}

	for (int i = 0; i < 6; i++)
	{
		printf_s("Entrez la valeur des repétitions de la %d des durees de service : ", i);
		scanf_s("%d", &tabLoiDesServices[i]);
	}
}

double calculLambda(int tabLoiArriveeClient[])
{
	double lambda;
	int rixi = 0;

	for (int i = 0; i < LGTABARRIVE; i++) {
		rixi += tabLoiArriveeClient[i];
	}

	lambda = rixi / LGTABARRIVE;

	return lambda;
}

double calculEsperanceDureeService(int tabLoiDesServices[])
{
	double esperance = 0;
	int i = 0;

	while (i < LGTABSERVICE)
	{
		esperance += (i + 1)*(tabLoiDesServices[i] / LGTABARRIVE);
	}

	return esperance;
}

void calculStationsMaxMin(double lambda, double esperanceDS, int *stationMin, int *stationMax)
{
	double psy = lambda * esperanceDS;
	stationMin = (int)psy;
	stationMax = 5 * 6;
}

void generateurArrivees(int a, int c, int m, int *xn, Client **pClientDeb, Client**pClient, int tabLoiDesArriveesClient[])
{
	double un = calculUnAvecXn(a, c, m, &xn);
	int tabLoiDesArriveesClients[6] = tabLoiDesArriveesClient;
	//et là ça marche bizarrement
	int nbArrivees = obtentionNbArrivees(un, tabLoiDesArriveesClients);

	int i = 0;
	int priorite;
	int dureeService;

	while (i < nbArrivees)
	{
		un = calculUnAvecXn(a, c, m, &xn);
		priorite = obtentionPriorité(un);
		un = calculUnAvecXn(a, c, m, &xn);
		dureeService = obtentioDureeService(un);

		creerChainonClient(priorite, dureeService, &pClientDeb, &pClient);

		i++;
	}
	//Tri
}

void creerChainonClient(int priorite, int dureeService, Client* *pClientDeb, Client**pClient)
{
	Client *pClientNouv = NULL;

	pClientNouv = (Client *)calloc(sizeof(Client), 1);
	if (pClientNouv == NULL) {
		printf_s("place manquante");
	}

	garnirChainonClient(priorite, dureeService, &pClientNouv);

	placementChainonShift(&pClientNouv, pClient, pClientDeb);
}

double calculUnAvecXn(int a, int c, int m, int *xn)
{
	double xnPlus1 = ((a*(*xn)) + c) % m;
	double un = xnPlus1 / m;
	*xn = xnPlus1;
	return un;
}

void calculNbStationsOptimal(int stationMin, int stationMax, double tabCoutTotal[])
{
	double coutMin = HUGE_VAL;
	int nbStationsAOuvrir = stationMin;
	int i = 0;

	while (i < stationMax - stationMin)
	{
		if (tabCoutTotal[i] < coutMin)
		{
			coutMin = tabCoutTotal[i];
			nbStationsAOuvrir = stationMin + i;
		}

		i++;
	}
	printf_s("%s", nbStationsAOuvrir);
}

void sortieDifferentsCoutsEnvisages(int stationMin, int stationMax, double tabCoutTotal[])
{
	int i = 0;

	while (i < stationMax - stationMin)
	{
		printf_s("%lf", tabCoutTotal[i]);
	}
}

int obtentionPriorité(double un)
{
	// 0 = absolu, 1 = relatif, 2 = normal
	int prioriteValeur;

	if (un < 0, 6)
	{
		prioriteValeur = 2;
	}
	else
	{
		if (un < 0, 7)
		{
			prioriteValeur = 1;
		}
		else
		{
			if (un < 1)
			{
				prioriteValeur = 0;
			}
		}
	}

	return prioriteValeur;
}

int obtentionNbArrivees(double un, int tabLoiDesArriveesClients[])
{
	int nbArrivees;
	//OH SHIT JE ME SUIS CHIE DESSUS C'EST BIEN UN TABLEAU A 2 DIMENSION POUR AVOIR LES FREQUENCES CUMULEES
	if (un < tabLoiDesArriveesClients[0])
	{
		nbArrivees = 0;
	}
	else
	{
		if (un < tabLoiDesArriveesClients[1])
		{
			nbArrivees = 1;
		}
		else
		{
			if (un < tabLoiDesArriveesClients[2])
			{
				nbArrivees = 2;
			}
			else
			{
				if (un < tabLoiDesArriveesClients[3])
				{
					nbArrivees = 3;
				}
				else
				{
					if (un < tabLoiDesArriveesClients[4])
					{
						nbArrivees = 4;
					}
					else
					{
						nbArrivees = 5;
					}
				}
			}
		}
	}

	return nbArrivees;
}

/*void tri( *file)
{
int passage = 0;
int enCours;
bool permutation = true;

while (permutation = ture)
{
permutation = false;
passage++;
enCours = 0;

while (enCours < tabArriveeMinuteCourante.length - passage)
{
if (tabArriveeMinuteCourante[enCours].dureeServiceGeneree > tabArriveeMinuteCourante[enCours + 1].dureeServiceGeneree)
{
permutation = true;
temp = tabArriveeMinuteCourante[enCours];
tabArriveeMinuteCourante[enCours] = tabArriveeMinuteCourante[enCours + 1];
tabArriveeMinuteCourante[enCours + 1] = temp;
}
enCours++;
}
}
}*/

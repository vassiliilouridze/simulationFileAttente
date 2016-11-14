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

typedef struct fileAbsolue FileAbsolue;
struct fileAbsolue
{
	Client *pClientFileAbsolue;
	FileAbsolue *pClientFileAbsolueSuiv;
};

typedef struct fileRelatif FileRelatif;
struct fileRelatif
{
	Client *pClientFileRelatif;
	FileRelatif *pClientFileRelatifSuiv;
};

typedef struct fileOrdinaire FileOrdinaire;
struct fileOrdinaire
{
	Client *pClientFileOrdinaire;
	FileOrdinaire *pClientFileOrdinaireSuiv;
};

typedef struct station Station;
struct station
{
	Client *pClientStation;
	Station *pStationSuiv;
};

void initTableauxValeurs(double tabLoiArriveeClient[][6], double tabLoiDesServices[][6]);
double calculLambda(double tabLoiArriveeClient[][6]);
double calculEsperanceDureeService(int tabLoiDesServices[]);
void calculStationsMaxMin(double lambda, double esperanceDS, int *stationMin, int *stationMax);
void generateurArrivees(int a, int c, int m, int *xn, Client **pClientDeb, Client**pClient, double tabLoiDesArriveesClient[][6], double tabLoiDesServices[][6], FileAbsolue **pDebFileAbsolue, FileRelatif **pDebFileRelatif, FileOrdinaire **pDebFileOrdinaire);
void creerChainonClient(int priorite, int dureeService, Client* *pClientDeb, Client**pClient);
double calculUnAvecXn(int a, int c, int m, int *xn);
int obtentioDureeService(double un, double tabLoiDesService[][6]);
int obtentionNbArrivees(double un, double tabLoiDesArriveesClients[][6]);
void garnirChainonClient(int priorite, int dureeService, Client **pClientNouv);
void placementChainonClient(Client **pClientNouv, Client **pClient, Client **pClientDeb);
void calculNbStationsOptimal(int stationMin, int stationMax, double tabCoutTotal[]);
void sortieDifferentsCoutsEnvisages(int stationMin, int stationMax, double tabCoutTotal[]);
void triFileAbsolue(FileAbsolue *pFileAbsolueDeb, int nbArrivees);
void triFileRelatif(FileRelatif *pFileRelatifDeb, int nbArrivees);
void triFileOrdinaire(FileOrdinaire *pFileOrdinaireDeb, int nbArrivees);

void main(void)
{
	int a = 137;
	int c = 187;
	int m = 256;
	int xn = 32;

	int stationMin = 0;
	int stationMax = 0;
	int station;

	double lambda;
	double esperanceDS;

	/*int tabLoiArriveeClient[LGTABARRIVE];
	int tabLoiDesServices[LGTABSERVICE];*/
	double tabLoiArriveeClient[2][LGTABARRIVE] = { { 5,2,3,28,12,7 },{0.087719,0.122807,0.1754386,0.6666667,0.87719298,1} };
	double tabLoiDesServices[2][LGTABSERVICE] = { { 2,3,2,11,17,22 }, {0.0350877193,0.087719,0.122807,0.3157894737,0.6140350877,1} };

	Client *pClientDeb = NULL;
	Client *pClient = NULL;
	Client *pRechClient = NULL;

	//initTableauxValeurs(tabLoiArriveeClient, tabLoiDesServices);

	int arriveTotalMaxSimulation = LGTABARRIVE * TPSSIMULATION;

	lambda = calculLambda(tabLoiArriveeClient);
	esperanceDS = calculEsperanceDureeService(tabLoiDesServices);
	calculStationsMaxMin(lambda, esperanceDS, &stationMin, &stationMax);
	
	Station *pStation = NULL;
	Station *pDebStation = NULL;
	for (int i = 0; i < stationMax; i++) {
		Station *pStationNouv = (Station *)calloc(sizeof(Station), 1);

		pStationNouv->pClientStation = NULL;

		if (pStation == NULL)
		{
			pDebStation = pStationNouv;
		}
		else
		{
			pStation->pStationSuiv = pStationNouv;
		}
		pStation = pStationNouv;
	}

	/*double tabCoutTotal[1];
	for (int i = 0; i < stationMax; i++)
	{
		tabCoutTotal[i] = calloc(1, sizeof(double), 1);
	}*/

	station = stationMin;

	while (station <= stationMax)
	{
		int temps = 0;

		FileAbsolue *pFileAbsolue = NULL;
		FileAbsolue *pDebFileAbsolue = NULL;
		for (int i = 0; i < arriveTotalMaxSimulation; i++) {
			FileAbsolue *pFileAbsoluNouv = (FileAbsolue *)calloc(sizeof(FileAbsolue), 1);

			pFileAbsoluNouv->pClientFileAbsolue = NULL;
			if (pFileAbsolue == NULL)
			{
				pDebFileAbsolue = pFileAbsoluNouv;
			}
			else
			{
				pFileAbsolue->pClientFileAbsolueSuiv = pFileAbsoluNouv;
			}
			pFileAbsolue = pFileAbsoluNouv;
		}

		FileRelatif *pFileRelatif = NULL;
		FileRelatif *pDebFileRelatif = NULL;
		for (int i = 0; i < arriveTotalMaxSimulation; i++) {
			FileRelatif *pFileRelatifNouv = (FileRelatif *)calloc(sizeof(FileRelatif), 1);

			pFileRelatifNouv->pClientFileRelatif = NULL;
			if (pFileRelatif == NULL)
			{
				pDebFileRelatif = pFileRelatifNouv;
			}
			else
			{
				pFileRelatif->pClientFileRelatifSuiv = pFileRelatifNouv;
			}
			pFileRelatif = pFileRelatifNouv;
		}

		FileOrdinaire *pFileOrdinaire = NULL;
		FileOrdinaire *pDebFileOrdinaire = NULL;
		for (int i = 0; i < arriveTotalMaxSimulation; i++) {
			FileOrdinaire *pFileOrdinaireNouv = (FileOrdinaire *)calloc(sizeof(FileOrdinaire), 1);

			pFileOrdinaireNouv->pClientFileOrdinaire = NULL;
			if (pFileOrdinaire == NULL)
			{
				pDebFileOrdinaire = pFileOrdinaireNouv;
			}
			else
			{
				pFileOrdinaire->pClientFileOrdinaireSuiv = pFileOrdinaireNouv;
			}
			pFileOrdinaire = pFileOrdinaireNouv;
		}

		int cumFileAbsolue = 0;
		int cumFileRelatif = 0;
		int cumFileOrdinaire = 0;
		int stationInnocupe = 0;

		//creerChainonClient(tabDS);

		while (temps < TPSSIMULATION)
		{
			//-->OK JUSQUE LA
			generateurArrivees(a, c, m, &xn, &pClientDeb, &pClient, tabLoiArriveeClient, tabLoiDesServices, &pDebFileAbsolue, &pDebFileRelatif, &pDebFileOrdinaire);

			/*initTabStationsPourSortie(station,tabStationsPourSortie)
			et puis tout le reste pour les sorties*/

			int i = 0;

			while (i < station)
			{
				if (pStation->pClientStation->dureeService = 0)
				{
					if (pDebFileAbsolue != NULL) {
						pStation->pClientStation->priorite = pDebFileAbsolue->pClientFileAbsolue->priorite;
						pStation->pClientStation->dureeService = pDebFileAbsolue->pClientFileAbsolue->dureeService;
						pStation->pClientStation->dureeService--;
						pStation->pClientStation->tpsDansSysteme++;
						
						/*RETIRER LE POINTEUR DE LA FILE
						fileAbsolue--;*/
					}
					else
					{
						if (pDebFileRelatif != 0)
						{

						}
						else
						{
							if (pDebFileOrdinaire != 0)
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
					pStation->pClientStation->dureeService--;
					pStation->pClientStation->tpsDansSysteme++;
				}
				i++;
			}

			pRechClient = pClientDeb;
			cumFileAbsolue = 0;
			cumFileRelatif = 0;
			cumFileOrdinaire = 0;
			while (pRechClient != NULL)
			{
				if (pRechClient->priorite = 0)
				{
					cumFileAbsolue += pRechClient->tpsDansSysteme;
				}
				else {
					if (pRechClient->priorite = 1)
					{
						cumFileRelatif += pRechClient->tpsDansSysteme;
					}
					else {
						cumFileOrdinaire += pRechClient->tpsDansSysteme;
					}
				}
				//BON ALORS MERdE HEIN ELLE A DIS QUE C'ETAIT BON MAIS ENFAITE NON ON PEUT PAS TOUT CUMULER A CHAQUE TOUR SINON ON COMPTE DE TROP ! ON NE PEUT FAIRE CA QUZ A LA FIN DE LA SIMULATION !
				pRechClient = pRechClient->pClientSuiv;
			}

			if (temps < 20)
			{
				double coutATMin = (temps + 1)*(30 / 60) + (stationInnocupe)*(18 / 60) + (37, 5 / 60)*(cumFileAbsolue / temps) + (22, 5 / 60)*(cumFileRelatif / temps) + (22, 5 / 60)*(cumFileOrdinaire / temps);
				printf_s("cout à %s min = %lf", temps, coutATMin);
				//Sortir tabNbArrivés.length; WHY?
			}

			temps++;
		}

		pRechClient = pClientDeb;
		cumFileAbsolue = 0;
		cumFileRelatif = 0;
		cumFileOrdinaire = 0;
		while (pRechClient != NULL)
		{
			if (pRechClient->priorite = 0)
			{
				cumFileAbsolue += pRechClient->tpsDansSysteme;
			}
			else {
				if (pRechClient->priorite = 1)
				{
					cumFileRelatif += pRechClient->tpsDansSysteme;
				}
				else {
					cumFileOrdinaire += pRechClient->tpsDansSysteme;
				}
			}
			pRechClient = pRechClient->pClientSuiv;
		}

		//tabCoutTotal[station - stationMin] = 16 * 30 + (stationInnocupe / 60) * 18 + 37, 5 * (cumFileAbsolue / 960) + 22, 5 * (cumFileRelatif / 960) + (22, 5)*(cumFileOrdinaire / 960);

		station++;

	}

	//calculNbStationsOptimal(stationMin, stationMax, tabCoutTotal);
	//sortieDifferentsCoutsEnvisages(stationMin, stationMax, tabCoutTotal);

	system("pause");
}
//OK
void initTableauxValeurs(double tabLoiArriveeClient[][6], double tabLoiDesServices[][6])
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
//OK
double calculLambda(double tabLoiArriveeClient[][6])
{
	double lambda;
	int rixi = 0;
	int somme = 0;

	for (int i = 0; i < LGTABARRIVE; i++) {
		rixi += i*tabLoiArriveeClient[0][i];
		somme += tabLoiArriveeClient[0][i];
	}
	lambda = (double)rixi/somme;

	return lambda;
}
//OK
double calculEsperanceDureeService(double tabLoiDesServices[][6])
{
	double esperance = 0;
	int rixi = 0;
	int somme = 0;
	int i = 0;

	while (i < LGTABSERVICE)
	{
		rixi += (i+1)*tabLoiDesServices[0][i];
		somme += tabLoiDesServices[0][i];
		i++;
	}
	esperance = (double)rixi / somme;
	
	return esperance;
}
//OK Mais surement problème d'arrondis
void calculStationsMaxMin(double lambda, double esperanceDS, int *stationMin, int *stationMax)
{
	double psy = lambda * esperanceDS;
	*stationMin = (int)psy;
	*stationMax = 5 * 6;
}

void generateurArrivees(int a, int c, int m, int *xn, Client **pClientDeb, Client**pClient, double tabLoiDesArriveesClient[][6], double tabLoiDesServices[][6], FileAbsolue **pDebFileAbsolue, FileRelatif **pDebFileRelatif, FileOrdinaire **pDebFileOrdinaire)
{
	printf_s("%d\n", *xn);
	double un = calculUnAvecXn(a, c, m, xn);
	double tabLoiDesArriveesClients[2][6];
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 6; j++)
		{
			tabLoiDesArriveesClients[i][j] = tabLoiDesArriveesClient[i][j];
		}		
	}
	double tabLoiDesService[2][6];
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 6; j++)
		{
			tabLoiDesService[i][j] = tabLoiDesServices[i][j];
		}
	}

	printf_s("%lf\n%d", un,*xn);

	int nbArrivees = obtentionNbArrivees(un, tabLoiDesArriveesClients);

	int i = 0;
	int priorite;
	int dureeService;

	while (i < nbArrivees)
	{
		un = calculUnAvecXn(a, c, m, &xn);
		priorite = obtentionPriorité(un);
		un = calculUnAvecXn(a, c, m, &xn);
		dureeService = obtentioDureeService(un, tabLoiDesService);
		//OK JUSQUE LA ET LA DEDANS QUELQUES ERREURS SUREMENT FAUT REVOIR SON PP HEIN TSSS
		creerChainonClient(priorite, dureeService, &pClientDeb, &pClient);

		i++;
	}
	//MOUAIS JE ME TAPE DES BOUCLES INFINIES LA DEDANS DONC JE ME SUIS CHIE DESSUS AU NIVEAU DU PP LORS DU PLACEMENT DES POINTEURS
	triFileAbsolue(&pDebFileAbsolue, nbArrivees);
	triFileRelatif(&pDebFileRelatif, nbArrivees);
	triFileOrdinaire(&pDebFileOrdinaire, nbArrivees);
}
//OK
double calculUnAvecXn(int a, int c, int m, int *xn)
{
	int xnPlus1 = (int)(a*(*xn) + c) % (int)m;
	double un = (double)xnPlus1 / m;
	*xn = xnPlus1;
	return un;
}
//OK
int obtentionNbArrivees(double un, double tabLoiDesArriveesClients[][6])
{
	int nbArrivees;
	
		for (int j = 0; j < 6; j++) {
			printf("\n%lf", tabLoiDesArriveesClients[1][j]);
		}
	if (un > tabLoiDesArriveesClients[1][0])
	{
		if (un > tabLoiDesArriveesClients[1][1])
		{
			if (un > tabLoiDesArriveesClients[1][2])
			{
				if (un > tabLoiDesArriveesClients[1][3])
				{
					if (un > tabLoiDesArriveesClients[1][4])
					{
						nbArrivees = 5;
					}
					else
					{
						nbArrivees = 4;
					}
				}
				else
				{
					nbArrivees = 3;
				}
			}
			else
			{
				nbArrivees = 2;
			}
		}
		else
		{
			nbArrivees = 1;
		}
	}
	else
	{
		nbArrivees = 0;
	}

	return nbArrivees;
}

//OK
int obtentioDureeService(double un, double tabLoiDesService[][6])
{
	int dureeService;

	for (int j = 0; j < 6; j++) {
		printf("\n%lf", tabLoiDesService[1][j]);
	}
	if (un > tabLoiDesService[1][0])
	{
		if (un > tabLoiDesService[1][1])
		{
			if (un > tabLoiDesService[1][2])
			{
				if (un > tabLoiDesService[1][3])
				{
					if (un > tabLoiDesService[1][4])
					{
						dureeService = 6;
					}
					else
					{
						dureeService = 5;
					}
				}
				else
				{
					dureeService = 4;
				}
			}
			else
			{
				dureeService = 3;
			}
		}
		else
		{
			dureeService = 2;
		}
	}
	else
	{
		dureeService = 1;
	}

	return dureeService;
}
//OK MOUAIS
void creerChainonClient(int priorite, int dureeService, Client* *pClientDeb, Client**pClient)
{
	Client *pClientNouv = NULL;

	pClientNouv = (Client *)calloc(sizeof(Client), 1);
	if (pClientNouv == NULL) {
		printf_s("place manquante");
	}
	//A FAIRE
	garnirChainonClient(priorite, dureeService, &pClientNouv);
	//A FAIRE ET LA PAS OUBLIER DE LE METTRE DANS LA BONNE FILE AUSSI
	placementChainonClient(&pClientNouv, pClient, pClientDeb);
}
//OK MOUAIS
void garnirChainonClient(int priorite, int dureeService, Client **pClientNouv)
{
	(*pClientNouv)->dureeService = dureeService;
	(*pClientNouv)->priorite = priorite;
	(*pClientNouv)->tpsDansSysteme = 1;
}
//OK MOUAIS
void placementChainonClient(Client **pClientNouv, Client **pClient, Client **pClientDeb)
{
	if ((*pClient) == NULL)
	{
		(*pClientDeb) = (*pClientNouv);
	}
	else
	{
		(*pClient)->pClientSuiv = (*pClientNouv);
	}
	(*pClient) = (*pClientNouv);
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
//OK
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

void triFileAbsolue(FileAbsolue *pFileAbsolueDeb, int nbArrivees)
{
	int passage = 0;
	int enCours;
	int permutation = 1;
	FileAbsolue *pFileAbsoluRech = pFileAbsolueDeb;
	FileAbsolue *pTemp = NULL;

	while (permutation = 1)
	{
		permutation = 0;
		passage++;
		enCours = 0;
		//VA FALLOIR VERIFIER QU'ON VA PAS PLUS LOIN QUE NECESSAIRE SUR LE POINTEUR
		while (enCours < nbArrivees - passage)
		{
			if (pFileAbsoluRech->pClientFileAbsolue->dureeService > pFileAbsoluRech->pClientFileAbsolueSuiv->pClientFileAbsolue->dureeService)
			{
				permutation = 1;
				pTemp = pFileAbsoluRech;
				pFileAbsoluRech = pFileAbsoluRech->pClientFileAbsolueSuiv;
				pFileAbsoluRech->pClientFileAbsolueSuiv = pTemp;
			}
			pFileAbsoluRech = pFileAbsoluRech->pClientFileAbsolueSuiv;
			enCours++;
		}
	}
}

void triFileRelatif(FileRelatif *pFileRelatifDeb, int nbArrivees)
{
	int passage = 0;
	int enCours;
	int permutation = 1;
	FileRelatif *pFileRelatifRech = pFileRelatifDeb;
	FileRelatif *pTemp = NULL;

	while (permutation = 1)
	{
		permutation = 0;
		passage++;
		enCours = 0;
		//VA FALLOIR VERIFIER QU'ON VA PAS PLUS LOIN QUE NECESSAIRE SUR LE POINTEUR
		while (enCours < nbArrivees - passage)
		{
			if (pFileRelatifRech->pClientFileRelatif->dureeService > pFileRelatifRech->pClientFileRelatifSuiv->pClientFileRelatif->dureeService)
			{
				permutation = 1;
				pTemp = pFileRelatifRech;
				pFileRelatifRech = pFileRelatifRech->pClientFileRelatifSuiv;
				pFileRelatifRech->pClientFileRelatifSuiv = pTemp;
			}
			pFileRelatifRech = pFileRelatifRech->pClientFileRelatifSuiv;
			enCours++;
		}
	}
}

void triFileOrdinaire(FileOrdinaire *pFileOrdinaireDeb, int nbArrivees)
{
	int passage = 0;
	int enCours;
	int permutation = 1;
	FileOrdinaire *pFileOrdinaireRech = pFileOrdinaireDeb;
	FileOrdinaire *pTemp = NULL;

	while (permutation = 1)
	{
		permutation = 0;
		passage++;
		enCours = 0;
		//VA FALLOIR VERIFIER QU'ON VA PAS PLUS LOIN QUE NECESSAIRE SUR LE POINTEUR
		while (enCours < nbArrivees - passage)
		{
			if (pFileOrdinaireRech->pClientFileOrdinaire->dureeService > pFileOrdinaireRech->pClientFileOrdinaireSuiv->pClientFileOrdinaire->dureeService)
			{
				permutation = 1;
				pTemp = pFileOrdinaireRech;
				pFileOrdinaireRech = pFileOrdinaireRech->pClientFileOrdinaireSuiv;
				pFileOrdinaireRech->pClientFileOrdinaireSuiv = pTemp;
			}
			pFileOrdinaireRech = pFileOrdinaireRech->pClientFileOrdinaireSuiv;
			enCours++;
		}
	}
}

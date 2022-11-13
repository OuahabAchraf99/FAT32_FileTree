#include <stdio.h>
#include <stdlib.h>	
#include <string.h>
//#include<windows.h>
//#include<conio.h>
#include "./bib.h"


   void page_gard(){
  int skip; 

printf(" ************************************************************************************************************************************************************************\n");
printf(" *");
printf("             _");
printf("                                                                                                                                                        *\n");
printf(" *            ");
printf("|_|");
printf("                                                                                                                                                       *\n");
printf(" *            ");
printf("/_/");
printf("                                                                                                                                                       *\n");
printf(" *   ");
printf("___  ___ _");
printf("                                                                                                                                                         *\n");
printf(" *  ");
printf("/ _ \\/ __| |                                        REPUBLIQUE ALGERIENNE DEMOCRATIQUE ET POPULAIRE                                                                 *\n *");
printf("                                                                                                                                                                      *\n");
printf(" * ");
printf("|  __/\\__ \\ |                                           Ecole nationale Superieure d'Informatique");
printf("                                                                    *\n");
printf(" *  ");
printf("\\___||___/_|                                                                                                                                                        *\n");
printf(" *                                                                                                                                                                      *\n");
printf(" *                                                                                                                                                                      *\n");
printf(" *                                                                                                                                                                      *\n");
printf(" *                                                                                                                                                                      *\n");
printf(" *                                                                                                                                                                      *\n");
printf(" *                                                                                                                                                                      *\n");
printf(" *                                                                                                                                                                      *\n");
printf(" *                                                                                                                                                                      *\n");
printf(" *                                                    TP SYS   \n"); 
printf(" *                                                                                                                                                                      *\n");
printf(" *                                                                                                               2019/2020                                              *\n");
printf(" *                                                                                                                                                                      *\n");
printf(" *                                                                                                                                                                      *\n");
printf(" *                                                                                                                                                                      *\n");
printf(" *                                        Realise par : OUAHAB Achraf - AICHE Mohammed Islam  - Section A Groupe 02                                                     *\n");
printf(" *                                                                                                                                                                      *\n");
printf(" *                                                                                                                                                                      *\n");
printf(" *                                                                                                                                                                      *\n");
printf(" *                                                                                                                                                                      *\n");
printf(" *                                                                                                                                                                      *\n");
printf(" ************************************************************************************************************************************************************************\n");
printf("\n\n entrer un nombre(0-9) pour continue");
scanf("%d",&skip);
}


int main()
{
	system("mode con lines=53 cols=200");
    	page_gard();
    	system("clear");
	int n,Num_sect;char disque_physique[50];char partition[50];int exit;
menu:
system("clear");
printf("           \n\n\n                                 Veuillez faire un choix dans le menu suivant :\n");
printf("\n\n\n");
printf("                 1) Afficher la liste des disques .\n\n");
printf("\n\n\n");
printf("                 2) Afficher le contenue d'un secteur du disque .\n\n");
printf("\n\n\n");
printf("                 3) Afficher la liste des fichiers/repertoires d'une partition FAT32.\n\n");
printf("\n\n\n");

	printf("Choix: ");
	scanf("%d",&n);

	switch (n)
	{
	    case 1: 
		{
		system("clear");
		Liste_Disques();
		printf("\n\n entrer un nombre(0-9) pour continue");
		// car system(pause) n'existe pas sous linux
		scanf("%d",&exit);
		goto menu;
		}
	    case 2: 
		{
		system("clear");
		printf("Lecture d'un secteur \n\n");
		printf("Donner le chemin du disque physique (/dev/sd?) 	exemple: /dev/sdb \n");
		scanf("%s",disque_physique);
		printf("Donner le numero du secteur que vous voulez lire et afficher");
		scanf("%d",&Num_sect);
		Afficher_secteur(disque_physique,Num_sect);
		printf("\n\n entrer un nombre(0-9) pour continue");
		scanf("%d",&exit);
		goto menu;
		}
	case 3: 
		{
		
		system("clear");
		printf("Afficher la liste des fichiers/repertoires d'une partition FAT32");
		printf("Donner le chemin du disque physique (/dev/sd*)");
		scanf("%s",disque_physique);
		printf("Donner le chemin de la partition (/dev/sd**), exemple: /dev/sdb1 \n");
		scanf("%s",partition);
		Entry *tab=Afficher_Fdel(disque_physique,partition);
		int i=0;
		for(i=0;i<50;i++)
		{
			printf("\n nom: %s \n",tab[i].nomCourt);
			if(tab[i].file_directory==0) printf("File/Directorry: 				FILE \n"); else printf("File/Directorry: Directory\n"); 
			printf("Taille du fichier/repertoire  				%d\n",tab[i].taille);
			printf("Numero du premier cluster : %d\n", 				tab[i].numPremClus);
			printf("Pere: %s \n\n",tab[i].pere);
			printf("_____________________________________________");
		}
		printf("\n\n entrer un nombre(0-9) pour continue");
		scanf("%d",&exit);
		goto menu;
		}
	    default: goto menu;
	}
	return 0;
}

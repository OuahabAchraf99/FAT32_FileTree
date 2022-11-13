#include <stdio.h>
#include <stdlib.h>	
#include <string.h>

typedef struct Entry{
char nomCourt[11];
int file_directory;
int taille;
int numPremClus;
char pere[11];
} Entry;
// Table d'Entry fat32
typedef struct table{
Entry *entTab;
int nbElt;
} table;


void Liste_Disques()
{
    FILE *disque = NULL;
    int n = 0,i=0;
    char alphabet[26] = {'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z'};
    char chemin_abs[9] = "/dev/sd";
    printf("La Liste Des Disques Physiques Connectes : \n");
    for (i=0;i<26;i++)
    {
	chemin_abs[7] = alphabet[i];
	disque = fopen(chemin_abs , "rb");
// verifier si le fichier sd? existe alors le disque physique liee existe
	if(disque != NULL) 
	{
		printf("%d- %s\n",n+1,chemin_abs);
		n++;
		fclose(disque);
	}
	else;
    }
    printf("On a %d Disques Connectes.\n",n);
}


char *Lire_secteur(char disque_physique[50],int Num_sect)
{
    int b,n,s;
    FILE *disque= NULL;
    int adr= 512* Num_sect;     
  
    unsigned char *buffer= malloc(512* sizeof(unsigned char));
    disque=fopen(disque_physique,"rb");
    if (disque== NULL)
    {
        printf(" Le disque n'est pas ouvert, verifier le chemin du disque");
    }
    else
    {
    	s=fseek(disque,adr,SEEK_SET);
	if(s!=0) 
	{
		printf("Erreur de deplacement");
	}
	else
	{
		n= fread(buffer,512,1,disque);
        	if(n<=0) printf("\n Erreur de fread = %d ",n);           
       		else printf("\n Lecture du secteur %d; Nombre de secteurs lus = 		 %d \n",Num_sect,n);	
	}
	
        
    }

   fclose(disque);
  
   return buffer;
}


void Afficher_secteur (char disque_physique[50],int Num_sect)
{
    int i=0,j=0;
    int Nmbre_Lignes=32;  
    unsigned char *buff= malloc(512* sizeof(unsigned char));
    buff= Lire_secteur(disque_physique,Num_sect);
    printf("Adresse    Contenu(octet de 1 à 16) \n");
    for ( i = 0; i < Nmbre_Lignes; i++)
    {
        printf(" %04d      ",Num_sect*512+16*i);
        for ( j = 0; j < 16; j++)
        {
            printf("%02X  ",buff[16*i + j] );
        }
        printf("\n");        
    }
    free(buff);
    buff=NULL;
    return;

}

Entry extractInfo(unsigned char *entry)
// en entree on prend un table de char sur 32 byte(taille d'une entree )
// et en retour on a les informations liees a cette entree
{
	Entry valRetour;
	int k=0;
	for (k=0;k<11;k++) valRetour.nomCourt[k]=entry[k];
	valRetour.taille=entry[28]+ entry[29]*0x100+ entry[30]*0x10000+ 	  entry[31]*0x1000000;
	// la taille de l'entree est sur 4byte a partir de l offset 28 en 	//little endien
	valRetour.numPremClus= entry[26]+ entry[27]*0x100+ entry[20]*0x10000+ 		  entry[21]*0x1000000;
	if((entry[11] & 0x10) ==0x00) 	valRetour.file_directory=0; 
	// si le bit 4 est 0 -> fichier sinon (1) c est une repertoire
	else valRetour.file_directory=1;
	return valRetour;
}



table *addEntries(int numSec,char partition[50])
// extraire les entrees a partir du secteur numSec
{
	unsigned char *buff=malloc(512* sizeof(unsigned char));
	Entry *tab=malloc(100*sizeof(Entry));
	unsigned char *entr=malloc(32*sizeof(unsigned char));
	table *valRet=malloc(100*sizeof(table));
	int b=0;int n= numSec;
	buff=Lire_secteur(partition,n);
			int stop=0,i=0,j=0,bool=0;
			while(!stop)
			{
				// verifier c'est l entree est vide donc on 	      			//est dans la fin de la liste des entrees
				if (buff[j]==0x00)
				{stop=1;break;}else;
				//Lire l entree
				for(i=0;i<32;i++)
				{
					entr[i]=buff[j];
					j++;	
				}
		// assurer qu'il s agit pas de format long ou fichier suprimee
				if ((entr[11]!= 0x0F ) && (entr[0]!=0xE5))
				{
					tab[b]=extractInfo(entr);
					b++;
				}else;
				if (j==512)
				{
					n++;j=0;
					buff=Lire_secteur(partition,n);
					
				}  else;
			}
valRet->entTab=tab;
valRet->nbElt=b;
return valRet;

}



Entry *Afficher_Fdel(char disque_physique[50],char partition[50])
{
	FILE *disque=NULL;
	FILE *part=NULL;
	disque=fopen(disque_physique,"rb");
	unsigned char *secBoot=malloc(512* sizeof(unsigned char));
	//unsigned char *ent=malloc(32* sizeof(unsigned char));
	Entry *EntryTab=malloc(100* sizeof(Entry));
	//Entry *temp=malloc(100* sizeof(Entry));
	table *temp=malloc(100*sizeof(table));
	char Pere[11];
	if(disque==NULL)
	{
		printf("Le disque specifiee n'existe pas");
	}
	else
	{
		part=fopen(partition,"rb");
		if(part==NULL) {printf("partition n'existe pas");}
		else
		{
// EXtraire les infos essentiel du secteur boot
			secBoot=Lire_secteur(partition,0);
			//nbre de secteur par cluster
			int nbSectParClus= secBoot[13];
			// nbre de secteur reservees
			int nbSecReserv=secBoot[14]+ secBoot[15]* 0x100 ;
			//nbre de secteur par fat
			int nbSectParFat= secBoot[36]+ secBoot[37]*0x100+ 				secBoot[38]*0x10000+ secBoot[39]*0x1000000; 
			//nbre de section fat	
			int nbFat=2 ; //dans FAT32 il ya deux section fat
			//premier secteur de donnes
			int premSectDonnes=nbSecReserv+(nbSectParFat*nbFat);
			int numSect=premSectDonnes;int a=0,i=0,j=0,k=0;
			temp=addEntries(numSect,partition);
			while(i<temp->nbElt)
			{
				EntryTab[a]=temp->entTab[i];
				i++;a++;
			}
			while(j<50)
			//on a limiter le nombre de fichier/repertoire Max a 50 			a cause de probleme de segmentation sur linux
			{
				if(EntryTab[j].file_directory==1)
				{
					i=0;
					// le pere des sous fichier/dir
					for(k=0;k<11;k++) 		   							Pere[k]=EntryTab[j].nomCourt[k];
					// le nouveau numSect pour affichier 						ses sous fich/dir
					numSect=premSectDonnes+ 				       (EntryTab[j].numPremClus-2)* 						nbSectParClus;	
					temp=addEntries(numSect,partition);	
					while(i<temp->nbElt)
					{
						EntryTab[a]=temp->entTab[i];
					for(k=0;k<11;k++)
						 EntryTab[a].pere[k]=Pere[k];	
						i++;a++;
					}
				}else;
				j++;
			}

		}
	}
	return EntryTab;
}




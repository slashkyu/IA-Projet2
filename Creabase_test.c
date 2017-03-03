#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <unistd.h>
#include <fcntl.h>




int main(int argc, char *argv[]){
	FILE *f,*f_b, *f_t;
	char c = 'a';
	char nameb[10][6], namet[10][6];
	double pourc;
	int i, f_base, f_test;
	char erreur_parametres[256];
	strcpy(erreur_parametres, "syntaxe  : ./creation base_complete\n");

	strcpy(nameb[0], "base1\0"); 
	strcpy(namet[0], "test1\0");

	strcpy(nameb[1], "base2\0"); 
	strcpy(namet[1],"test2\0");

	strcpy(nameb[2], "base3\0");
	strcpy(namet[2],"test3\0");

	strcpy(nameb[3], "base4\0");
	strcpy(namet[3],"test4\0");

	strcpy(nameb[4], "base5\0"); 
	strcpy(namet[4],"test5\0");

	strcpy(nameb[5], "base6\0"); 
	strcpy(namet[5],"test6\0");

	strcpy(nameb[6], "base7\0"); 
	strcpy(namet[6],"test7\0");

	strcpy(nameb[7], "base8\0"); 
	strcpy(namet[7],"test8\0");

	strcpy(nameb[8], "base9\0"); 
	strcpy(namet[8],"test9\0");

	strcpy(nameb[9], "base0\0"); 
	strcpy(namet[9],"test0\0");


		if(argc < 2) {
			fprintf(stderr, erreur_parametres, strlen(erreur_parametres));
			return 1;
		}


	
		//ouverture du fichiers BaseReuters
		f=fopen(argv[1],"r");
		if(f== NULL) {
			printf("Erreur ouverture fichier\n");
			return 1;
		}
		//On veut créer 10fichiers base et 10fichiers test liés.
		for(i=0; i<10; i++) {
			
			//Créer les fichiers s'ils n'existent pas vec tout les droits
			f_base=open(nameb[i], O_WRONLY | O_CREAT, S_IWUSR | S_IRUSR | S_IRGRP  | S_IROTH );
	
			f_test = open(namet[i], O_WRONLY | O_CREAT, S_IWUSR | S_IRUSR | S_IRGRP  | S_IROTH);
			
			close(f_base);
			close(f_test);

			//réouvrir les fichiers en descripteur de fichiers de type file pour faire une lecture cahracètre par characètre
			f_b=fopen(nameb[i],"w");
			f_t=fopen(namet[i],"w");

			
			c = fgetc(f);
			while(c!= EOF ){
				pourc = rand()%10; //varaible servant à calculer si le documents fera parties des 70% -de la base- ou des 30% -des tests-
				if (pourc < 7.000000) { 
					while(c!= EOF && c != '\n') {
						
						fputc(c,f_b);
						c = fgetc(f);
					}
						fputc(c,f_b);
						c=fgetc(f);
				}
				else {
					while(c!= EOF && c != '\n') {
						fputc(c,f_t);
						c = fgetc(f);
					}
						fputc(c,f_t);
						c=fgetc(f);
				}		
				
					
			}
		rewind(f);	//retour au début du fichier
		fclose(f_b);
		fclose(f_t);
		}
		return 0;
	}

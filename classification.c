#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <time.h>
#include <math.h>
//gcc -Wall -Werror -g -o exec classification.c -lm

  


void init_chaine(char tab[7]){
	int i;	
	for( i=0; i<7;i++) 
		tab[i] =0;
}

//fonction lisant une chaine de 7caracètre maximum jusqu'à rencontrer un espace, deux point, end of file ou retour chariot
int lecture_chaine(FILE *f, char tab[7]){
	int i =0;	
	char c ;
	init_chaine(tab);
	fscanf(f, "%c", &c);
	while (c !=' ' && c!=':' && c!=EOF && c != '\n'){
		tab[i] = c;
		c = fgetc(f);		
		
		i++;
		}	
	return c;			
}	



void init_matrice( int classe, int voc, double ** table){  //nom equivoque  
	int i,j ;
	for (i=0; i<classe; i++){
		for( j=0 ; j<voc ; j++){
			table[i][j]=0;
		}
	}
} 


void init_matrice_test( int classe, int voc, double ** table){  //nom equivoque  
	int i,j ;
	for (i=0; i<classe; i++){
		for( j=0 ; j<voc ; j++){
			table[i][j]=0;
		}
	}
} 


void init_matrice_max( int classe, int * table){
	int i;
	for( i =0; i< classe; i++){
		table[i] =0 ; }
}


int max(int a, int b){
if(a>b)
	return a;
return b;
}


void afficher_chaine(char *s){
	int i ;
	printf("\t");
	for( i=0; i < strlen(s); i++)
		printf("%c",s[i]);


}

//calcul du nombre total de document dans le base d'apprentissage
double calc_t(int classe, int *matrice){
	int i;
	double t=0;
	for(i=0; i< classe; i++) {
		
		
		t = matrice[i] + t;
	}
	return t;
}


//------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------
//FONCTION MAIN
int main(int argc, char *argv[]){

/* VARIABLE DE STOCKAGE DE DONNES DES FICHIERS

classe_max correspondat au nombre total de classe rencontrées dans la base d'apprentissage. On considère ici que si les documents
du fichier test ont une classe inconnue cela engendra une erreur lorsque l'on vérifiera si le programme a vu juste.
voc_max est l'indice le plus grand du mot rencontrés dans la base.
voc_max_test est l'indice le plus du mot rencontré dans le fichier test

Ces varaibles servent à consuitre les différentes matrice et tableaux.

-matrice_max correspond au nombre de documents par classe
-matrice correspondant à la présence des mots par classe : matrice[i][j] vaudra 0 si le mot j n'apparait pas dans la classe i, sinon il sera supérieur à 0.
-matrice_test correspondat à la présence des mots dans les documents du fichiers test (même principe que celle d'avant).

f est un descripteur de fichier servant à parcourir les différents fichiers

b est un booléen servant au bon fonctionnement de la boucle.

j i z sont des indices servant dans des boucles for

indice est un indices servant à parcourir les matrices.

nb_doc sert à comptabiliser le nombre de couments du deuxième fichiers
*/

	FILE *f;
	char chaine[7];
	char c = 'a';
	int etat = 0;
	int classe, classe_max=1, voc, voc_max= 1,voc_max_test=1;
	double ** matrice, **matrice_test;
	int * matrice_max, *verification;
	int b =1;
	int j, z, i;
	int indice;
	int nb_doc=0;
	char erreur_parametres[256];
	strcpy(erreur_parametres, "syntaxe  : ./classification base_d'apprentissage test\n");
	

/*VARIABLES CALCUL DE PROBABILITES
Py -> P( y = k)
Pyd -> P(y = k | d)
teta -> teta(j,k)
bon sert à connaitre le taux de réussites du programmes.
pres a pour valeur la présence du mot j dans le document d
max a pour valeur la probabilité la plus haute trouvée pour le document d
*/	

	long **teta, *Py, *Pyd, taille;
	double bon=0;
	int pres,p_max ;


if(argc < 3) {
	fprintf(stderr, erreur_parametres, strlen(erreur_parametres));
	return 1;
}







//--------------------------------------------------------
//BOUCLE DE LECTURE DU PREMIER FICHIER

	
		f=fopen(argv[1],"r");
		if(f== NULL) {
			printf("Erreur ouverture fichier\n");
			return 1;
		}
		while( b &&c!= EOF ){
			c = lecture_chaine(f, chaine);
			//afficher_chaine(chaine);
			//printf("%c",c);

			
			switch(etat){					
			case 0 ://lecture classe					
				if( c == ' '){ 
					classe = atoi(chaine);
					etat = 1;
					
					classe_max = max(classe_max,classe);
					
				}
				else {
					etat =3;
				}
			break;
			case 1 :	//lecture mot vocabulaire				
				if (c == ':'){
					etat = 2;
					voc=atoi(chaine);
					voc_max= max(voc_max,voc);	

				}
				else if( c == ' '){  // pour l'esapce en fin de ligne
					etat=1;
					}
				else{ 				//c = \n 
					etat= 0;
				}
			break;			
 			case 2 :	//lecture classe associée				
				etat = 1;

			
			break;			
			case 3 :	//detection de EOF;	
				b=0;		
			break;
			//default :
			//break;
			}
		}


		fclose(f);



//--------------------------------------------------------
//Parcours deuxième fichier test
	
		b=1;
		etat=0;
		f=fopen(argv[2],"r");
		if(f== NULL) {
			printf("Erreur ouverture fichier\n");
			return 1;
		}
		while( b && c!= EOF ){
			c = lecture_chaine(f, chaine);


			
			switch(etat){					
			case 0 ://lecture classe					
				if( c == ' '){ 
					classe = atoi(chaine);
					etat = 1;
					
					nb_doc ++;
				}
				else {
					etat =3;
				}
			break;
			case 1 :	//lecture mot vocabulaire				
				if (c == ':'){
					etat = 2;
					voc=atoi(chaine);
					voc_max_test= max(voc_max_test,voc);	

				}
				else if( c == ' '){  // pour l'esapce en fin de ligne
					etat=1;
					}
				else{ 				//c = \n 
					etat= 0;
				}
			break;			
 			case 2 :	//lecture classe associée				
				etat = 1;

			
			break;			
			case 3 :	//detection de EOF;
				b=0;		
			break;
			//default :
			//break;
			}
		}
	fclose(f);
	voc_max=max(voc_max,voc_max_test);		



//--------------------------------------------------------
//ALLOUER LES MATRICES ET TABLEAUX

	matrice = malloc(classe_max * sizeof(double*));
	for( z =0; z < classe_max; z++){
		matrice[z] = malloc( voc_max * sizeof(double));
	}	

	if(matrice == NULL) {
		printf("erreur malloc matrice base\n");
		return 1;
	}

	matrice_test = malloc(nb_doc * sizeof(double*));
	for( z =0; z < nb_doc; z++){
		matrice_test[z] = malloc( voc_max * sizeof(double));
	}

	
	if(matrice_test == NULL) {
		printf("erreur malloc matrice test\n");
		return 1;
	}

	matrice_max = malloc(classe_max * sizeof(int));
	
	if(matrice_max == NULL) {
		printf("erreur malloc matrice documents par classe\n");
		return 1;
	}

	verification = malloc(nb_doc * sizeof(int));

	if(verification == NULL) {
		printf("erreur malloc tableau verification\n");
		return 1;
	}
	


//--------------------------------------------------------
//Initialisation des matrices
	init_matrice(classe_max, voc_max, matrice);

	init_matrice_max(classe_max, matrice_max);
	

	init_matrice_test(nb_doc, voc_max, matrice_test);



	
//--------------------------------------------------------
//Remplissage des matrices

//REMPLISSAGE DES MATRICES LIEES A LA BASE D'APPRENTISSAGE
	b =1;
	etat =0;
	f=fopen(argv[1],"r");
	if(f== NULL) {
		printf("Erreur ouverture fichier\n");
		return 1;
	}
	while( b && c!= EOF ){
		c = lecture_chaine(f, chaine);

		
		switch(etat){					
		case 0 ://lecture classe					
			if( c == ' '){ 
				classe = atoi(chaine);
				etat = 1;
				matrice_max[classe-1]++;
				
			}
			else {		//fin
				etat =3;
			}
		break;
		case 1 :	//lecture mot vocabulaire				
			if (c == ':'){
				etat = 2;
				voc=atoi(chaine);
				
				matrice[classe-1][voc-1]++;
			}
			else if( c == ' '){  // pour l'esapce en fin de ligne
				etat=1;
				}
			else{ 				//c = \n 
				etat= 0;
			}
		break;			
		case 2 :	//lecture du nombre d'occurence ce qui est au final pas tres important				
			etat = 1;

	
		break;			
		case 3 :	//detection de EOF
			b=0;		
		break;
		}
	}
	fclose(f);



//-----------------------------------------------
//BOUCLE DE REMPLISSAGE DES MATRICES LIES AU TEST
	indice=0;
	b =1;
	etat =0;
	f=fopen(argv[2],"r");
	nb_doc = 0;
	if(f== NULL) {
		printf("Erreur ouverture fichier\n");
		return 1;
	}
	while( b && c!= EOF ){
		c = lecture_chaine(f, chaine);
		//afficher_chaine(chaine);
		//printf("%c",c);

		switch(etat){					
		case 0 ://lecture classe					
			if( c == ' '){ 
				classe = atoi(chaine);
				etat = 1;
				verification[nb_doc]=classe;
				nb_doc++;
			}
			else {		//fin
				etat =3;
			}
		break;
		case 1 :	//lecture mot vocabulaire				
			if (c == ':'){
				etat = 2;
				voc=atoi(chaine);
				matrice_test[indice][voc-1]++;
				
				
				
			}
			else if( c == ' '){  // pour l'esapce en fin de ligne
				etat=1;
				}
			else{ 				//c = \n 
				etat= 0;
				indice++;
				
			}
		break;			
		case 2 :	//lecture du nombre d'occurence ce qui est au final pas tres important				
			etat = 1;
			
	
		break;			
		case 3 :	//detection de EOF
			printf("fin du fichier\n");
			b=0;		
		break;
		}
	}
	fclose(f);


//------------------------------------
//CALCUL DE PROBABILITE


	teta = malloc(classe_max*sizeof(double*));
	for(i = 0; i< classe_max; i++){
		teta[i] = malloc(voc_max*sizeof(double));
		
	}

	//calcul du nombre max de documentde la base
	taille=calc_t(classe_max, matrice_max);

	//calcul des teta pour toute les classes
	for(i=0;i< classe_max; i++){
		for(j=0; j<voc_max; j++) {
			teta[i][j] = (matrice[i][j] +1)/(matrice_max[i]+2);
			
		}
	}



	Py = malloc(classe_max * sizeof(long ));

	Pyd = malloc(classe_max * sizeof(long ));

	//Calcul de P( y = k) pour toutes les classes
	for (i = 0; i<classe_max; i++){

		Py[i]=matrice_max[i]/taille;
	}



//------------------------------------------------------------------------------------------------
//calcul de P( y = k |d) pour tout les documents du fichier tests	
	for(z=0; z<nb_doc; z++){
		p_max = 0;
		for (i = 0; i<classe_max; i++){

			Pyd[i]=Py[i];
			for(j=0; j<voc_max; j++){
				if(matrice_test[z][j] !=0){
					pres =1;
				}
				else 
					pres = 0;
				
				Pyd[i]=  (Pyd[i] * ( pow(teta[i][j], pres) * (pow(1-teta[i][j],(1 - pres) ) ) ) );
			 	
				
			}
			
			
			if( Pyd[i]> Pyd[p_max]){
				
				p_max =i;
			}			
		
		}
		if( p_max+1 == verification[z]) {
			bon++;
		}
	}
		
	printf("Le programme à %f de réussite \n", bon/nb_doc);

	
	//------------------------------------------------------------------------------------------------
	//Libération de l'espace allouer
	
	for( z =0; z < classe_max; z++){
		free(matrice[z]) ;
	}	
	free(matrice);


	for( z =0; z < nb_doc; z++){
		free(matrice_test[z]);
	}
	free(matrice_test);

	free(matrice_max);
	
	free(verification);

	free(teta);

	free(Pyd);

	free(Py);
















	return 0;

}


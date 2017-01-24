#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <gmp.h>
#include "pgp.h"
#include "lire_ecrire.h"
#include "commandes.h"
#include "gestion_cles.h"
#include "types.h"

// Affiche la date et l'heure
void affiche_date_heure(){
	time_t now = time (NULL);
	struct tm tm_now = *localtime (&now);
	char s_now[sizeof "JJ/MM/AAAA HH:MM:SS"];
	strftime (s_now, sizeof s_now, "%d/%m/%Y %H:%M:%S", &tm_now);
	printf ("%s\n", s_now);
}

/*void affiche_general(){
	printf("\033[01m\nSimplified Privacy Guard - Hybrid Cryptography.\n");
	printf("Université Versailles Saint Quentin en Yvelines - 2017 \nClaire Baskevitch - Clément Caumes\n");
	printf("Date courante : ");
	affiche_date_heure();
	printf("\n\033[0m");
}*/

// Affiche la présentation générale de PGP
void affiche_general(){
	printf("\033[34m\033[01m\nSimplified Privacy Guard - Hybrid Cryptography\n");
	printf("Université Versailles Saint Quentin en Yvelines - 2017\nClaire Baskevitch - Clément Caumes\n");
	printf("Date courante : ");
	affiche_date_heure();
	printf("\n\033[0m");
}

// Affiche les commande : ./pgp -h
void affiche_commandes(){
	printf("\033[01m- Pour déchiffrer un fichier:\033[32m ./pgp -w [fichier.pgp]\033[37m Le contenu déchiffré du fichier s'affichera à l'écran\n\033[0m");
	printf("\033[01m- Pour déchiffrer un fichier:\033[32m ./pgp [fichier.pgp]\033[37m Le fichier message déchiffré sera dans \033[31m[fichier]\n\033[0m");
	printf("\033[01m- Pour signer un message:\033[32m ./pgp -s [fichier]\n\033[0m");
	printf("\033[01m- Pour transférer le contenu de \033[31m[fic1.pgp]\033[37m dans \033[31m[fic2.pgp]\033[37m : \033[32m ./pgp -ka [fic1.pgp] [fic2.pgp]\n\033[0m");
	printf("\n\033[0m");
}

// Retourne la taille des clés voulu par l'utilisateur
int demande_taille_cles(){
	char choix[16];int i;
	printf("\033[01mChoisissez la taille de votre clé RSA :\n");
	printf("	1)	512 bits - Low commercial grade, fast but less secure\n");
	printf("	2)	768 bits - High commercial grade, medium speed, good security\n");
	printf("	3)	1024 bits - Military grade, slow, highest security\n");
	printf("	4)	2048 bits - Restricted by the E.U & U.S governments\n");
	printf("Choisissez 1, 2, 3 ou 4 : ");
	scanf("%s",choix);
	if(strlen(choix)>=2) quitte_demande_invalide();
	if(!isdigit(choix[0])) quitte_demande_invalide();
	if((atoi(choix)==0)||(atoi(choix)>=5)){
		quitte_demande_invalide();
	}
	if(atoi(choix)==1) i=512;
	else if(atoi(choix)==2) i=768;
	else if(atoi(choix)==3) i=1024;
	else i=2048;
	printf("\033[01mVous avez choisi une clé de %d bits.\n\n\033[0m",i);
	return i;
}

// Ecriture dans un fichier
void affiche_action_pgp(char* nom){
	printf("\033[01mLe fichier à envoyer est sous le nom : \033[31m%s\n\n\033[0m",nom);
}

void ecrit_bordure_sup_rsa_pub(FILE* f){
	fprintf(f,"-----BEGIN PGP PUBLIC KEY BLOCK-----\n");
}

void ecrit_bordure_sup_rsa_priv(FILE* f){
	fprintf(f,"-----BEGIN PGP PRIVATE KEY BLOCK-----\n");
}

void ecrit_bordure_sup_m_sig(FILE* f){
	fprintf(f,"-----BEGIN PGP SIGNED MESSAGE-----\n");
}

void ecrit_bordure_sup_id(FILE* f,char* s1,char* s2,char* s3){
	fprintf(f,"-----IDENTITY-----\n");
	fprintf(f,"%s %s %s\n",s1,s2,s3);
}

void ecrit_bordure_inf_m_sig(FILE* f){
	fprintf(f,"\n-----BEGIN PGP SIGNATURE-----\n");
}

void ecrit_bordure_inf_rsa_pub(FILE* f){
	fprintf(f,"-----END PGP PUBLIC KEY BLOCK-----\n");
}

void ecrit_bordure_inf_rsa_priv(FILE* f){
	fprintf(f,"-----END PGP PRIVATE KEY BLOCK-----\n");
}

void affiche_dechiffrement(char* nomFichier){
	printf("AFFICHAGE FICHIER DECHIFFRE\n\n");
}

void affiche_fichier_dechiffre(char* nomFichier){
	printf("\033[01mLe contenu déchiffré du fichier \033[31m%s\033[37m est : \n\n\033[0m",nomFichier);
	affiche_dechiffrement(nomFichier);
}
//////////////////

// Ecrit le contenu de "origin" dans "new"
void recopie_message(FILE* origin,FILE* new){
	char c='a';
	while(c!=EOF){
		c=fgetc(origin);
		if(c!=EOF){
			fprintf(new,"%c",c);
		}
	}
}

// Affiche le contenu d'un fichier
void affiche_contenu_fic(char* nomFichier){
	FILE* f=fopen(nomFichier,"r");
	char c;
	do{
		c=fgetc(f);
		if(c!=EOF) printf("\033[34m%c",c);
	}while(c!=EOF);
	printf("\n\033[0m");
	fclose(f);
}

// Renvoie la réponse de l'utilisateur OUI/NON
int teste_reponse(char* s){
	int o1=strcmp(s,"oui"); int n1=strcmp(s,"non");
	int o2=strcmp(s,"Oui"); int n2=strcmp(s,"Oui");
	int o3=strcmp(s,"OUI"); int n3=strcmp(s,"OUI");
	int o4=strcmp(s,"OuI"); int n4=strcmp(s,"OuI");
	int o5=strcmp(s,"oUi"); int n5=strcmp(s,"oUi");
	int o6=strcmp(s,"oUI"); int n6=strcmp(s,"oUI");
	int o7=strcmp(s,"O"); int n7=strcmp(s,"N");
	int o8=strcmp(s,"o"); int n8=strcmp(s,"n");
	if((!o1)||(!o2)||(!o3)||(!o4)||(!o5)||(!o6)||(!o7)||(!o8)){
		return 1;
	}
	if((!n1)||(!n2)||(!n3)||(!n4)||(!n5)||(!n6)||(!n7)||(!n8)){
		return 2;
	}
	else return -1;
}

// Procédure qui affiche le contenu d'un fichier si nécessaire
void demande_visualisation_message(char* nomFichier){
	printf("\033[01mFichier corrompu. Voulez-vous quand même l'afficher?(o/N)");
	char buffer[256];
	scanf("%s",buffer);
	if(teste_reponse(buffer)==1) affiche_contenu_fic(nomFichier);
	else quitte_pas_probleme();
}

// Ecrit la clé privée dans le fichier secring.pgp
//A MODIFIER CAR IL FAUT QUELLE SOIT CHIFFRE
void ecrit_cle_privee(mpz_t n,mpz_t d){
	FILE* f=fopen("secring.pgp","w");
	ecrit_bordure_sup_rsa_priv(f);
	// genere et ecrit la cle ici
	char pass_phrase[256];
	cree_pass_phrase(pass_phrase); //UTILISE ICI POUR CHIFFRER
	gmp_fprintf(f,"%Zd\n",n);
	gmp_fprintf(f,"%Zd\n",d);
	ecrit_bordure_inf_rsa_priv(f);
	fclose(f);
	//printf("\033[01m\033[31m\nGénération de la clé privée terminée\n\n\033[0m");
}

// Ecrit la clé publique dans le fichier pubring.pgp
//ATTENTION PENSER AU FAIT QUE LE FICHIER DE CLEFS EST DEJA REMPLIE
//DE CONTACTS QUON NE VEUT PAS PERDRE!!!!
//A REFAIRE
void ecrit_cle_publique(char* s1,char* s2,char* s3,mpz_t n,mpz_t e){
	FILE* f=fopen("pubring.pgp","w");
	ecrit_bordure_sup_id(f,s1,s2,s3);
	ecrit_bordure_sup_rsa_pub(f);
	//
	gmp_fprintf(f,"%Zd\n",n);
	gmp_fprintf(f,"%Zd\n",e);
	//
	ecrit_bordure_inf_rsa_pub(f);
	fclose(f);
	printf("\n");
	//printf("\033[01m\033[31m\nGénération de la clé publique terminée\n\n\033[0m");
}

// Copie le contenu de fic1 dans fic2 puis supprime fic1
void transfert_fic1_fic2(char* s1,char* s2){
	FILE* f1;
	FILE* f2;
	f1=fopen(s1,"a");
	if(f1==NULL) exit(1);
	f2=fopen(s2,"r");
	if(f2==NULL) exit(1);
	fprintf(f1,"\n\n");
	char c;
	do{
		c=fgetc(f2);
		if(c!=EOF) fprintf(f1,"%c",c);
	}while(c!=EOF);
	int suppr;
	suppr=remove(s2);
	if(suppr) quitte_suppression_impossible(s2);
	printf("\033[01mLe fichier \033[31m%s\033[37m possède de nouvelle(s) clé(s)\n\n\033[0m",s1);
}

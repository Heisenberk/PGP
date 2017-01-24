#include <stdio.h>
#include <stdlib.h>
#include "pgp.h"
#include "envoi.h"
#include "reception.h"
#include "commandes.h"
#include "lire_ecrire.h"
#include "gestion_cles.h"
#include "types.h"

int main(int argc,char** argv){
	int mode=teste_commande_general(argc,argv);
	//envoyer un message non chiffré et signé
	if(mode==MODE_SIGN_NN_CHIFFRE){
		ecrit_message_non_chiffre(argc,argv);
	}
	//recevoir un message chiffré
	else if(mode==MODE_DECHIFFREMENT){
		cree_fichier_dechiffre(argv[1]);
	}
	//
	else if(mode==MODE_AFFICHAGE_DECHIFFRE){
		affiche_fichier_dechiffre(argv[2]);
	}
	//genere nouvelles cles rsa
	else if(mode==MODE_GENERATION_CLES){
		//genere_cle_privee();
		//genere_cle_publique();
		//demande_taille_cles();
		genere_cles();
	}
	//coupe fic1 et colle dans fic2
	else if(mode==MODE_TRANSFERT_CLES){
		transfert_fic1_fic2(argv[2],argv[3]);
	}
	return 0;
	
	/*verifie_authentification(argv[1]);
	return 0;*/
	
	/*unsigned char hash[16];
	demande_pass_phrase(hash);
	affiche(hash);
	char* hash1=PASSPHRASE;
	unsigned char* final=(unsigned char*)hash1;
	affiche2(hash1);
	//if(teste_egalite_hashes(hash,final)) printf("EGALITE\n");
	//else printf("INEGALITE\n");
	return 0;*/
}

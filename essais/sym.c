#include <gmp.h>
#include <stdio.h>
#include <stdlib.h>

//A REGLER ACCENT!!!
int random_int(gmp_randstate_t state){
	mpz_t n;
	mpz_init(n);
	mpz_urandomb(n,state,7);
	double i;
	i=mpz_get_d(n);
	if((i<=31)||(i>=127)) i=random_int(state);
	mpz_clear(n);
	int ii=i;
	return ii;
}

int xor(int i,int j){
	return i ^ j;
}

void encrypt(char* nomFichier1,char* nomFichier2,char* cle){
	FILE* f1=fopen(nomFichier1,"r");
	FILE* f2=fopen(nomFichier2,"w");
	int k=0;
	char c;
	do{
		c=fgetc(f1);
		if(c!=EOF){
			fprintf(f2,"%d ", xor(cle[k%16],c));
			k++;
		}
	}while(c!=EOF);
	fclose(f1);
	fclose(f2);
}

void decrypt(char* nomFichier1,char* nomFichier2,char* cle){
	FILE* f1=fopen(nomFichier1,"r");
	FILE* f2=fopen(nomFichier2,"w");
	int k=0;
	int c;
	/*do{
		fscanf(f1,"%d",&c);
		if(c!=EOF){
			fprintf(f2,"%c", xor(cle[k%16],c));
			k++;
		}
	}while(c!=EOF);*/
	int b;
	while(c!=EOF){
		fscanf(f1,"%d",&c);
		fprintf(f2,"%c",xor(cle[k%16],c));
		k++;
	}
	fclose(f1);
	fclose(f2);
}

int main(){
	gmp_randstate_t state;
	gmp_randinit_default (state);
	gmp_randseed_ui (state, (unsigned) time(NULL));

	char cleSession[17];
	cleSession[16]='\0';
	int i;
	for(i=0;i<16;i++){
		cleSession[i]=random_int(state);
	}
	printf("CLEF DE SESSION : \n%s\n",cleSession);
	/*FILE* f1=fopen("texte","r");
	FILE* f2=fopen("chiffre","w");
	int k=0;
	char c;
	do{
		c=fgetc(f1);
		if(c!=EOF){
			fprintf(f2,"%d ", xor(cleSession[k%16],c));
			//printf("%c",xor(cleSession[k%16],c));
			k++;
		}
	}while(c!=EOF);
	
	fclose(f1);
	fclose(f2);*/
	encrypt("texte","chiffre",cleSession);
	decrypt("chiffre","dechiffre",cleSession);
	gmp_randclear(state);
	return 0;
}

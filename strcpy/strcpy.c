#include<stdio.h>
void nas_strcpy(char* prvi, char* drugi){
	/*
	while (*drugi != '\0'){//proveri da li je ono na sta pokazuje drugi jednako '\0'
		*prvi = *drugi;
		prvi++;
		drugi++;
	}
	//sta jos ovde treba uraditi??? Kopirao sam do sada Ovaj string se kopira
	//moramo jos prvi string da terminiramo null terminatorom
	*prvi='\0';//tek sada je prvi kompletan string
	*/
	int i = 0;
	while(drugi[i] != '\0'){//drugi[i] kompajler prebaci na *(drugi + i)
		prvi[i] = drugi[i];
		i++;
	}
	prvi[i] = '\0';
}

void konvertuj_broj(int a, char* b){
	//hocu da funkcija konvertuje broj a u string b npr 123 -> "123"
	//ideja je da prvo vidim koliko uopste imam cifara u broju a
	int pom = a;
	int brojac = 0;
	while(pom != 0){
		pom /= 10;//pom = 12 prvi put, drugi put 1, treci put 0
		brojac++;
	}
	pom = a;
	b[brojac] = '\0';//brojac za broj 123 ce biti 3 a to znaci da b[3]='\0' sto je ok jer b[0] = '1', b[1] = '2' i b[2] = '3';
	for (int i = brojac - 1; i >=0; i--){
		b[i] = pom%10 + '0';//prvi put ovo ce biti 123%10=3, ali ne 3 nego '3'
		pom /= 10; 
	}

}

int main(){
	char a[20];//a je pokazivac na char i to prvi u nizu a
	char b[] = "Ovaj string se kopira";//b mogu da koristim kao pokazivac na prvi karakter odnosno u ovom slucaju O
	char c[20];
	nas_strcpy(a,b);
	//ocekujem da ovde a bude isti kao b;
	konvertuj_broj(123,c);
	printf("%s\n",c);
}

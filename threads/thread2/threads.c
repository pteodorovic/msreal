#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>

//struktura koju koristimo da prenesemo niti višestruke ulazne parametre
struct data {         
	int start;
	int stop; 
};

static void* sumfnc(void* arg) 
{
	//alocira se na heap-u povratna vrednost funkcije (može biti i struktura
	//ukoliko funkcija treba da vrati više vrednosti)
	int *sum = (int *) malloc(sizeof(int));         

	//pročitaj ulazne parametre iz primljene strukture struct data
	struct data inp = *(struct data *) arg;
	//saberi brojeve počevši od inp.start do inp.stop
	for (int i = inp.start; i < inp.stop; i++)
	{                 
		*sum += i;
	}
	//vrati vrednost koja će biti prihvaćena od strane thread_join
	//funkcije pozvane iz osnovne (roditelj) niti
	printf("Sum calculated in thread: %d\n", *sum);
	pthread_exit((void*)sum); 
}

//glavni program očekuje dodatni parametar N koji se koristi prilikom sabiranja
int main (int argc, char *argv[]) 
{         
	int ret;
	//povratne vrednosti iz dve niti
	int *sum1;
	int *sum2;
	//identifikatori niti 
	pthread_t thread1;
	pthread_t thread2;

	//proveri da li je prosleđen dodatni parametar prilikom poziva programa
	if (argc<2)
	{
		printf("Usage:%s N\n", argv[0]);
		exit(1);
	}
	
	//konvertuj string u celobrojni podatak koji će se koristiti
	int N = atoi(argv[1]);

	//napravi strukturu koja će biti prosleđena prvoj niti
	struct data str1;
	str1.start = 1;
	str1.stop= N/2 + 1;

	//napravi strukturu koja će biti prosleđena drugoj niti
	struct data str2;
	str2.start = N/2 + 1;
	str2.stop = N+1;

	//kreiraj niti
	ret = pthread_create(&thread1, NULL, sumfnc, &str1);     
	if(ret!=0)
	{
		printf("Failed to create thread1 ret=%d\n",ret);
		exit(1);
	}
	
	ret=pthread_create(&thread2, NULL, sumfnc, &str2);
	if(ret!=0)
	{                 
		printf("Failed to create thread2 ret=%d\n",ret);
		exit(1);
	}         
	
	//čekaj da se niti završe i preuzmi povratne vrednosti
	pthread_join(thread1,(void **)&sum1);         
	pthread_join(thread2,(void **)&sum2);

	//prikaži rezultat    
	printf("Rezultat sabiranja je %d\n",*sum1+*sum2);         

	//oslobodi memoriju rezervisanu u svakoj od niti
	free(sum1);         
	free(sum2);         
	return 0; 
} 

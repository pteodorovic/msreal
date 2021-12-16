#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
struct data
{
	int start;
	int stop;
	int sum;
};

static void* sumfnc(void* arg)
{
	struct data *inp = (struct data *) arg;
	inp->sum = 0;
	printf("Start = %d, stop=%d\n",inp->start, inp->stop-1);
	for (int i = inp->start; i < inp->stop; i++)
	{
		inp->sum += i;
	}
	printf("Rezultat sabiranja u niti je %d\n",inp->sum);
	pthread_exit(NULL);
}

int main (int argc, char *argv[])
{
	int ret;
	pthread_t thread1;
	pthread_t thread2;
	if (argc<2)
	{
		printf("Usage:%s N\n", argv[0]);
		exit(1);
	}

	int N = atoi(argv[1]);

	struct data str1;
	str1.start = 1;
	str1.stop= N/2 + 1;

	struct data str2;
	str2.start = N/2 + 1;
	str2.stop= N+1;
	ret=pthread_create(&thread1,NULL,sumfnc,&str1);
	if(ret!=0)
	{
		printf("Failed to create thread1 ret=%d\n",ret);
		exit(1);
	}
	ret=pthread_create(&thread2,NULL,sumfnc,&str2);
	if(ret!=0)
	{
		printf("Failed to create thread2 ret=%d\n",ret);
		exit(1);
	}
	pthread_join(thread1,NULL);
	pthread_join(thread2,NULL);

	printf("Rezultat sabiranja je %d\n",str1.sum + str2.sum);
	return 0;
}

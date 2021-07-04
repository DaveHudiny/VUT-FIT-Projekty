/***************************************************************
 * Autor: David Hudák
 * Testováno na: Merlin, Linux Mint (Cinnamon, 19.3)
 * Jméno souboru: proj2.c
 * Účel: Druhý projekt do předmětu IOS (Operační systémy)
 * Co to je?: Úkol na synchronizaci procesů s pomocí semaforů.
 **************************************************************/


#include <stdio.h>
#include <stdbool.h>
#include <sys/types.h> 
#include <unistd.h> 
#include <sys/mman.h>
#include <semaphore.h>
#include <stdlib.h>
#include <fcntl.h>           /* For O_* constants */
#include <sys/stat.h>        /* For mode constants */
#include <time.h>
#include <pthread.h>
#include <errno.h>
#include <string.h>
#include <pthread.h>
#include <sys/wait.h>

typedef enum {EOK, EPARAM, EWRONGPARAM} chyby;

FILE *out; // Výstupní soubor
sem_t *judgesem; // Semafor přítomnosti soudce v budově (chrání hodnotu NB - počet migrantů v budově).
sem_t *immsem; // Chrání hodnotu NE.
sem_t *session; // Slouží k extrémní ochraně hodnoty A (číslo výpisu) - reakce na problémy, na něž neznám odpovědi.
sem_t *turniket; // Chrání odpověď NC.
sem_t *canenter; // Brání migrantovi vstoupit do fáze certifikací, když není soudce v budově.
sem_t *entry; // Chrání zápis do souboru - aby zapisoval jenom jeden.
sem_t *decision; // Semafor rozhodnutí soudce.
sem_t *leave; // Semafor možnosti vybírat si certifikáty.
sem_t *ileave; // Soudce nemůže odejít, dokud se všichni nedostali do fáze certifikátů (nikdo se pak nezasekne kdesi před).
sem_t *swearer; // Synchronizace hodnoty toswear - počet lidí, kteří se už dostali do fáze po canenter - určuje, kdy soudce může začít rozhodovat.
sem_t *toexit; // Synchronizuje hodnotu exited - povolení soudci odejít.
long int *NE; // Aktuální počet přistěhovalců v budově, o kterých nebylo rozhodnuto (jenom vstoupili).
long int *NC; // Aktuální počet přistěhovalců, kteří se zaregistrovali a dosud o nich nebylo rozhodnuto.
long int *NB; // Aktuální počet přistěhovalců, kteří jsou právě v budově.
long int *A; // Akce při psaní do souboru.
long int *toswear;
long int *exited; 

int init()
{
	out = fopen("proj2.out", "w"); // Otevření výstupního souboru.
	setvbuf(out,NULL,_IONBF,1); // Způsobuje, že se na výstup tiskne v tom pořadí, v jakém procesy volají funkci fprintf
	if(out == NULL)
	{
		return -1;
	}
	NE=mmap(NULL, sizeof(long int), PROT_WRITE | PROT_READ, MAP_SHARED | MAP_ANONYMOUS, -1, 0); // Alokování sdílených dat pro proměnné
	NC=mmap(NULL, sizeof(long int), PROT_WRITE | PROT_READ, MAP_SHARED | MAP_ANONYMOUS, -1, 0); 
	NB=mmap(NULL, sizeof(long int), PROT_WRITE | PROT_READ, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
	A =mmap(NULL, sizeof(long int), PROT_WRITE | PROT_READ, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
	toswear=mmap(NULL, sizeof(long int), PROT_WRITE | PROT_READ, MAP_ANONYMOUS | MAP_SHARED, -1, 0);
	exited=mmap(NULL, sizeof(long int), PROT_WRITE | PROT_READ, MAP_ANONYMOUS | MAP_SHARED, -1, 0); // Konec alokace sdílených dat
	*NE=0; // Inicializace sdílených proměnných
	*NC=0;
	*NB=0;
	*A=0;
	*toswear=0;
	*exited=0; // Konec inicializace sdílených proměnných
	judgesem=sem_open("/xhudak03_semaphore_judge", O_CREAT | O_EXCL, 0666, 1); // Otevírání semaforu.
	if(judgesem == SEM_FAILED) // Testování, zdali byl semafor otevřen.
	{
		return -1;
	}
	immsem=sem_open("/xhudak03_semaphore_immigrant", O_CREAT | O_EXCL, 0666, 1);
	if(immsem == SEM_FAILED)
	{
		return -1;
	}
	canenter=sem_open("/xhudak03_semaphore_canenter", O_CREAT | O_EXCL, 0666, 1);
	if(canenter == SEM_FAILED)
	{
		return -1;
	}
	turniket=sem_open("/xhudak03_semaphore_turniket", O_CREAT | O_EXCL, 0666, 1);
	if(turniket == SEM_FAILED)
	{
		return -1;
	}
	entry=sem_open("/xhudak03_semaphore_entry", O_CREAT | O_EXCL, 0666, 1);
	if(entry == SEM_FAILED)
	{
		return -1;
	}
	decision=sem_open("/xhudak03_semaphore_decision", O_CREAT | O_EXCL, 0666, 1);
	if(decision == SEM_FAILED)
	{
		return -1;
	}
	leave=sem_open("/xhudak03_semaphore_leave", O_CREAT | O_EXCL, 0666, 1);
	if(leave== SEM_FAILED)
	{
		return -1;
	}
	session=sem_open("/xhudak03_semaphore_session", O_CREAT | O_EXCL, 0666, 1);
	if(session== SEM_FAILED)
	{
		return -1;
	}
	ileave=sem_open("/xhudak03_semaphore_ileave", O_CREAT | O_EXCL, 0666, 1);
	if(ileave== SEM_FAILED)
	{
		return -1;
	}
	swearer=sem_open("/xhudak03_semaphore_swearer", O_CREAT | O_EXCL, 0666, 1);
	if(swearer== SEM_FAILED)
	{
		return -1;
	}
	toexit=sem_open("/xhudak03_semaphore_toexit", O_CREAT | O_EXCL, 0666, 1);
	if(toexit== SEM_FAILED)
	{
		return -1;
	} // Konec inicializací a testů semaforů.
} // Konec funkce init

void clean() // Funkce na uklizení sdílených proměnných a semaforů.
{
	munmap(NE, sizeof(long int)); // Uvolnění paměti sdílených proměnných
	munmap(NC, sizeof(long int));
	munmap(NB, sizeof(long int));
	munmap(A , sizeof(long int));
	munmap(toswear, sizeof(long int));
	munmap(exited, sizeof(long int)); // Konec uvolňování paměti sdílenných pamětí.

	if(out!=NULL)
	{
		fclose(out);
	}
	sem_close(judgesem);// Zavírání a odlinkování semaforů.
	sem_unlink("/xhudak03_semaphore_judge");
	sem_close(immsem);
	sem_unlink("/xhudak03_semaphore_immigrant");
	sem_close(canenter);
	sem_unlink("/xhudak03_semaphore_canenter");
	sem_close(turniket);
	sem_unlink("/xhudak03_semaphore_turniket");
	sem_close(entry);
	sem_unlink("/xhudak03_semaphore_entry");
	sem_close(decision);
	sem_unlink("/xhudak03_semaphore_decision");
	sem_close(session);
	sem_unlink("/xhudak03_semaphore_session");
	sem_close(leave);
	sem_unlink("/xhudak03_semaphore_leave");
	sem_close(ileave);
	sem_unlink("/xhudak03_semaphore_ileave");
	sem_close(swearer);
	sem_unlink("/xhudak03_semaphore_swearer");
	sem_close(toexit);
	sem_unlink("/xhudak03_semaphore_toexit"); // Konec zavírání a odklinkování semaforů
}// Konec funkce clean.

void judgeProcess(long int *PI, long int JG, long int JT) // Funkce obsluhující proces soudce.
{
	long int checkin = 0;
	while(checkin<(*PI)) // Cyklus na odbavení všech migrantů (checkin je počet odbavených migrantů).
	{
		if(JG!=0) // Čekání, pokud soudce čekat má.
		{
			usleep((rand()%JG)*1000);
		}

		sem_wait(entry); // Snaha o vstup - bez omezení

		sem_wait(session);
		(*A)++;
		sem_post(session);
		fprintf(out, "%li	:	JUDGE	:	wants to enter\n", *A);

		sem_post(entry); // Konec výpisu wants to enter


		sem_wait(judgesem); // Soudce čeká na vstup do budovy - zastavuje příliv migrantů.
		sem_wait(entry);

		sem_wait(session);
		(*A)++;
		sem_post(session);

		fprintf(out, "%li	:	JUDGE	:	enters	:	%li	:	%li	:	%li\n", *A, *NE, *NC, *NB);

		sem_post(canenter); // Migranti teď mohou potvrzovat svoji přítomnost ve fází certifikací.

		sem_post(entry);



		sem_wait(immsem); // Speciální případ - soudce vejde do budovy a nikdo tam není.
		sem_wait(entry);
		if(*NE==0)
		{
			sem_post(decision); // Rozhodnutí může udělit okamžitě a stejně tak i odejít.
			sem_post(ileave);
		}
		sem_post(entry);
		sem_post(immsem);



		sem_wait(immsem);
		sem_wait(turniket);

		if(*NE!=*NC) // Soudce má vypsat hlášení o čekání na registrace migrantů.
		{
			sem_wait(entry);

			sem_wait(session);
			(*A)++;
			sem_post(session);
			sem_trywait(decision); // Jistota uzamknutí rozhodnutí.
			fprintf(out, "%li	:	JUDGE	:	waits for imm	:	%li	:	%li	:	%li\n", *A, *NE, *NC, *NB);

			sem_post(entry);
		}

		sem_post(immsem);
		sem_post(turniket);

		sem_wait(decision); // Soudce čeká, dokud se mu někde neodemkla možnost rozhodnout.
		sem_wait(turniket); // Jistota, že se už nikdo neregistruje.
		sem_post(turniket);


		sem_wait(entry); // Začátek konfirmací.

		sem_wait(session);
		(*A)++;
		sem_post(session);
		fprintf(out, "%li	:	JUDGE	:	starts confirmation	:	%li	:	%li	:	%li\n", *A, *NE, *NC, *NB);

		sem_post(entry);


		if(JT!=0) // Spí, pokud má.
		{
			usleep((rand()%JT)*1000);
		}
		
		sem_wait(immsem); // Ochrana před jakýmikoliv změnami.
		sem_wait(turniket);
		sem_wait(swearer);

		sem_wait(entry);

		sem_wait(session);
		(*A)++;
		sem_post(session);
		checkin+=*NC; // Zvýší počet odbavených migrantů - indikátor pro soudce, kdy už může přestat pracovat.


		sem_wait(toexit);
	 	/* Zvýší hodnotu exited na počet zaregistrovaných:
	 	 *	dokud se pak nedostane znovu do nuly, tak o něco později čeká na semafor ileave*/
		(*exited)=*NC;
		if(*NE!=0) // Čekání na ileave je jen do té doby, kdy někdo v budově skutečně čeká na certifikace.
		{
			sem_trywait(ileave);
		}
		sem_post(toexit);

		(*toswear)=0; // Vyčistí toswear na další kolo potvrzování.
		(*NC)=0;
		(*NE)=0;
		fprintf(out, "%li	:	JUDGE	:	ends confirmation	:	%li	:	%li	:	%li\n", *A, *NE, *NC, *NB);

		sem_post(entry);

		sem_post(swearer);
		sem_post(turniket);
		sem_post(immsem);



		sem_post(leave); // Teď si můžou migranti začít chodit pro certifikáty.

		sem_wait(ileave); // Všichni došli do fáze těsně před certifikáty nebo po ní - soudce je propuštěn.

		sem_wait(leave); // Další migranti si už nemohou chodit pro certifikáty.

		
		sem_wait(entry); // Odchod - bez omezení

		sem_wait(session);
		(*A)++;
		sem_post(session);
		fprintf(out, "%li	:	JUDGE	:	leaves	:	%li	:	%li	:	%li\n", *A, *NE, *NC, *NB);
		sem_post(entry);


		sem_post(judgesem); // Migranti odteď zase mohou vstupovat do budovy
	}//Konec while cyklu


	sem_wait(entry); // Konec práce soudce, už se jenom rozloučí.

	sem_wait(session);
	(*A)++;
	sem_post(session);
	fprintf(out, "%li	:	JUDGE	:	finishes	\n", *A);

	sem_post(entry);


	exit(0);


} // Konece funkce obsluhující proces soudce (judgeProcess).



void immigrantProcess(long int immigrantID, long int IT) // Funkce obsluhující proces migranta
{
	sem_wait(judgesem); // Migrant vchází do budovy
	sem_wait(immsem);

	sem_wait(entry);
	sem_trywait(canenter); // Zabrání předčasnému odchodu do fáze před certifikacemi.
	sem_trywait(decision); // Zabrání předčasnému rozhodnutí soudce o certifikátech.
	*NB = *NB + 1; // Navýšení počtu migrantů v budově.
	*NE = *NE + 1;
	sem_wait(session);
	(*A)++;
	sem_post(session);
	fprintf(out, "%li	:	IMM %li	:	enters	:	%li	:	%li	:	%li\n", *A, immigrantID, *NE, *NC, *NB);
	sem_post(entry);

	sem_post(immsem);
	sem_post(judgesem); // Konec vcházení do budovy


	sem_wait(immsem);
	sem_wait(turniket); // Migrant se registruje

	sem_wait(entry);
	sem_wait(session);
	(*A)++;
	sem_post(session);
	(*NC)++; // Navýšení počtu registrovaných migrantů.
	fprintf(out, "%li	:	IMM %li	:	checks	:	%li	:	%li	:	%li\n", *A, immigrantID, *NE, *NC, *NB);
	sem_post(entry);

	sem_post(turniket); // Migrant se zaregistroval
	sem_post(immsem);


	sem_wait(canenter); // Migrant vstupuje do fáze předcertifikační.
	sem_wait(swearer);

	(*toswear)++; // Navýšení počtu registrovaných, co jsou ve fázi předcertifikační.

	sem_post(swearer);
	sem_post(canenter);


	sem_wait(immsem);
	sem_wait(turniket);
	sem_wait(swearer);

	if(*toswear==*NE) // Počet lidí, co jsou registrovaní a dostali se do této fáze je totožný, jako počet migrantů, co ještě nebyli odbaveni.
	{
		sem_post(decision); // Uvolní soudcovo rozhodování.
	}

	sem_post(swearer);
	sem_post(turniket);
	sem_post(immsem);


	sem_wait(leave); // Migrant čeká, až je připuštěn ke vstupu k certifikacím.
	sem_post(leave); // Migrant uvolňuje cestu ostatním.

	sem_wait(toexit); // Migranti postupně dochází do fáze těsně před certifikacemi.
	(*exited)--; // Snížení počtu migrantů, kteří čekají přede dveřmi certifikací.
	if(*exited==0)
	{
		sem_post(ileave); // Povolení soudci odejít - pak zase zamyká semafor leave, aby nikdo jen tak bez povolení nechodil do fáze certifikací.
	}
	sem_post(toexit);


	sem_wait(entry); // Migrant chce certifikát.
	sem_wait(session);
	(*A)++;
	sem_post(session);
	fprintf(out, "%li	:	IMM %li	:	wants certificate	:	%li	:	%li	:	%li\n", *A, immigrantID, *NE, *NC, *NB);
	sem_post(entry);


	if(IT!=0) // Spánek k vyzvednutí certifikátu.
	{
		usleep(1000*(rand()%IT));
	}


	sem_wait(entry); // Migrant dostává certifikát.
	sem_wait(session);
	(*A)++;
	sem_post(session);
	fprintf(out, "%li	:	IMM %li	:	got certificate	:	%li	:	%li	:	%li\n", *A, immigrantID, *NE, *NC, *NB);
	sem_post(entry);


	sem_wait(judgesem); // Chodit do a z budovy se smí jen tehdy, když není uvnitř soudce, tudíž se čeká, než v ní soudce není.
	sem_wait(entry);

	sem_wait(session);
	(*A)++;
	sem_post(session);
	(*NB)--; // Snížení počtu přítomných v budově.
	fprintf(out, "%li	:	IMM %li	:	leaves	:	%li	:	%li	:	%li\n", *A, immigrantID, *NE, *NC, *NB);

	sem_post(entry);
	sem_post(judgesem);


	exit(EXIT_SUCCESS); // Vše se zdařilo, jak má - proces je možno ukončit úspěšně.
} // Konec funkce obsluhující proces migranta (immigrantProcess()).


void processGenerator(long int *PI, long int IG, long int IT) // Funkce generující procesy migrantů.
{
	long int count = *PI;
	for(long int i=0; i<count; i++) // For cyklus postupně generující migranty.
	{
		pid_t pid = fork(); // Rozdělení se na generátor a nového migranta.

		if(pid==0) // Pro proces migranta.
		{

			sem_wait(entry);
			sem_wait(session);
			(*A)++;
			sem_post(session);
			fprintf(out, "%li	:	IMM %li	:	starts	\n", *A, i+1);
			sem_post(entry);
			immigrantProcess(i+1, IT); // Volá se migrant - předpokládá se, že má exit uvnitř sebe!!!!! (JINAK DOJDE K FORK BOMBĚ!!!)
			
		}
		else if(pid==-1) // Rozdvojení bylo neúspěšné.
		{
			fprintf(stderr, "Nebyl vytvoren migrant\n");
			(*PI)--; // Pokud by se nevytvořil migrant, soudce by cyklil do nekonečna, proto je nutné snížit celkový počet migrantů o jedničku.
			exit(EXIT_FAILURE);
		}
		if(IG!=0) // Pokud má, čeká na generování dalšího migranta.
		{
			int delayrand = rand() % IG;
			usleep(delayrand*1000);
		}
	} // Konec for cyklu.

	int status;
	int exitValue=EXIT_SUCCESS;
	while(wait(&status)>0) // Čeká se, dokud se nevyexitují všichni migranti.
	{
		if(!WIFEXITED(status))
		{
			fprintf(stderr, "Process skoncil spatne.\n");
			exitValue=EXIT_FAILURE;
		}
	}
	exit(exitValue); // Vrací hodnotu podle toho, jak dopadly předchozí operace.
}// Konec funkce processGenerator()


int main(int argc, char **argv)
{
	clean(); // Čištění pro jistotu.
	long int PI; // Počet imigrantů
	long int IG; // Doba generování imigranta
	long int JG; // Doba, po které soudce zase vsoupí do budovy
	long int IT; // Doba vyzvedávání certifikátu
	long int JT; // Doba, po kterou soudce uděluje ceritfikáty
	if(argc<6) // Malo paramerů (více parametrů je ignorováno)
	{
		fprintf(stderr, "Nespravny pocet parametru.\n");
		exit(EXIT_FAILURE);
	}
	PI = strtol(argv[1], NULL, 10); 
	if(PI<1) // Kontroly správnosti parametrů - analogicky pro PI, IG, JG, IT, JT.
	{
		fprintf(stderr, "Spatne zadany parametr.\n");
		exit(EXIT_FAILURE);
	}
	IG = strtol(argv[2], NULL, 10);
	if(IG < 0 || IG > 2000)
	{
		fprintf(stderr, "Spatne zadany parametr.\n");
		exit(EXIT_FAILURE);
	}
	JG = strtol(argv[3], NULL, 10);
	if(JG < 0 || JG > 2000)
	{
		fprintf(stderr, "Spatne zadany parametr.\n");
		exit(EXIT_FAILURE);
	}
	IT = strtol(argv[4], NULL, 10);
	if(IT < 0 || IT > 2000)
	{
		fprintf(stderr, "Spatne zadany parametr.\n");
		exit(EXIT_FAILURE);
	}
	JT = strtol(argv[5], NULL, 10);
	if(JT < 0 || JT > 2000)
	{
		fprintf(stderr, "Spatne zadany parametr.\n");
		exit(EXIT_FAILURE);
	} // Konec načítání a kontroly parametrů

	srand(time(NULL));
	if(init()==-1) // Inicializace - pokud se nezdařila, je třeba ji spustit znovu.
	{
		clean();
		fprintf(stderr, "Nepovedlo se inicializovat vsechny promenne a semafory. Prosim opakujte.");
		exit(EXIT_FAILURE);
	}
	sem_wait(leave); // Zamknutí leavu - migranti si pak nemohou jen tak chodit pro certifikáty, když je soudce v budově.
	sem_wait(decision); // Zamknutí rozhodnutí - soudce si nemůže vydávat rozhodnutí jen tak.
	pid_t pid2;
	pid_t pid = fork(); // Rozforkování na proces generátoru a hlavní proces.
	if(pid==0)
	{
		processGenerator(&PI, IG, IT); // Volání obslužné funkce generátoru - předpokládá se, že má vlastní exit uvnitř!!!
	}
	else if(pid==-1) // Nebylo možné vytvořit generátor.
	{
		fprintf(stderr, "Nepovedlo se vytvorit proces generatoru.\n");
		clean(); // Uklizení po sobě.
	}
	else
	{
		pid2 = fork(); // Rozforkování na proces soudce a hlavní proces.
		if(pid2==0)
		{
			judgeProcess(&PI, JG, JT); // Volání obslužné funkce pro proces soudce - musí obsahovat vlastní exit!!!!!
		}
		if(pid2==-1)
		{
			fprintf(stderr, "Nepovedlo se vytvorit proces soudce.\n");
			clean();
		}
	}
	int status=0;
	int exitValue=EXIT_SUCCESS;
	bool OK=waitpid(pid, &status, 0); // Hlavní proces čeká, až se ukončí proces generátoru.
	if(!WIFEXITED(status))
	{
		fprintf(stderr, "Proces generatoru skoncil spatne.\n");
		exitValue=EXIT_FAILURE;
	}
	OK=waitpid(pid2, &status, 0); // Hlavní proces čeká, až se ukončí proces soudce.
	if(!WIFEXITED(status))
	{
		fprintf(stderr, "Proces soudce skoncil spatne.\n");
		exitValue=EXIT_FAILURE;
	}
	clean();
	exit(EXIT_SUCCESS);
} // Konec hlavní funkce

/* Konec souboru proj2.c */
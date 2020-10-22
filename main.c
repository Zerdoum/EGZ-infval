#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <setjmp.h>
#include <inttypes.h>

#include "set.h"
#include "loi.h"


typedef uint64_t u64;
typedef __uint128_t duint_t;


// tab_num: transforme un bitmap ens en table croissante des bits mis à 1 entre 0 et max-1
// ens	notre bitmap
// max	taille de notre bitmap 
// tab	le tableau contenant les indices 
void tab_num(u64 ens, int *tab, int max)
{  
	int i,j;
	i=0;
	j=0;
	u64 m_max; // m_max est le masque
	u64 m; 
	m_max=1l<<max;
	for(m=1;m!=m_max;m<<=1)
	{
		if((m&ens)!=0)
		{
			tab[i++]=j;
		}
		j++;
	}
}

// rend le numéro d'un sous-ensemble d'un ensemble de n éléments
// le sous-ensemble est donné comme un bitmap des éléments

u64 numsubset(u64 w,int n)
{
   	int i,j; // index du parcours du triangle
   	u64 a;   // valeurs du triangle de Pascal modifié
   	u64 p;   // valeurs du triangle de Pascal
   	u64 x;  // cumul de tous les sommets parcourus
   	// initialisation
   	i=-1;
   	j=-1;
   	p=0;
   	a=0;
   	x=0;
   	while (n>0)
   	{
      	// selon la valeur du chiffre
     		switch (w&1)
      		{
        	case 0:
          		++i;           // en bas seulement
          		a=2*a-p;       // mise à jour coef de Pascal modifié 
          		p=(p*i)/(i-j); // mise à jour du coef de Pascal
          		break;
        	case 1:
          		if (j==-1) p=1; else p=(p*(i-j))/(j+1);  // p(i-1,j) en fonction de p(i-1,j-1)
          		a=2*a+p;        // mise à jour de a
          		++i; ++j; // en bas à droite 
          		if (i==j) p=1; else p=(p*i)/(i-j);  // mise à jour p(i,j) en fonction de p(i-1,j)
          		break;
      		}
      		// décaler w pour positionner le chiffre suivant
      		w>>=1;
      		// cumul de la valeur du sommet
      		x+=a;
      		--n; // compteur de boucle
   	}
   	return x;
}


// rend le sous-ensemble d'un numéro donné
u64 subsetnum(u64 x,int i) // le paramètre i sert de ligne courante
{
  	int	j; // colonne courante
  	u64	a; // Coefficients du triangle de Pascal modifié a(n,j)
  	u64	p; // Coefficients du triangle de Pascal
  	u64	w; // mot resultat

  	// initialisation
  	w=0;
  	p=1;
  	a=0;
  	j=0;
  	// recherche de la colonne j telle que a(i,j) <= "x" < a(i,j+1)
  	// et initialisation de a(i,j)
  	while (a+p<=x)
  	{
    		a+=p;
    		j++;
    		p=(p*(i-j+1))/j;
  	}
  	p=(p*j)/i;
  	a=(a+p)/2;
  	--j;
  	x-=a;
  	--i;
  	// boucle nominale
  	while ( (j>=0) && (j<i) )
  	{ // 'algorithme s'arrête dès qu'on est arrivé sur 'un des côté
    	// soit le côté j=0, alors on compléte avec des zéros
    	// soit le côté i=j, alors on complète avec des uns
    		if (x>=a) // peut-on soustraire a(i-1,j)
    		{
      			p=(p*(i-j))/i;
      			a=(a+p)/2; // mise à jour de a
      			--i; // en haut seulement
    		}
    		else
    		{
      			w|=(1ll<<i); // se souvenir que le chiffre est 1
      			a=(a-(p*(i-j))/i)/2; // mise à jour de a
      			p=(p*j)/i;  // mise à jour de p
      			--i; // en haut 
     	 		--j; // a gauche 
    		}
    		x-=a; // on soustrait ce qu'on a pu
  	}
  	// remplissage des derniers caracteres avec des zéro ou des 1
  	if (j!=-1) // selon de cote du triangle ou on arrive
  	{
    		w|=(1ll<<(i+1))-1ll;
  	}
  	return w;
}

// Fonction de soustraction.
// une table croissante en liste d'éléments
// Cette fonction sert de transformer le sous-ensemble en une suite. 
// tab: contient le sous-ensemble. 
// n: la taille du tableau tab.
void sous_tab_num(int *tab,int n)
{
	int i;
	for(i=0;i<n;i++)
	{
		tab[i]=tab[i]-i;
	}
}
//
// insère un 1 dans x à la position donnée par m = 0..010..0 où m est le masque 
u64 insere_un_bis(u64 x, u64 m)
{
    u64 mm;
    mm=m-1;
    return (x&mm) + ((x&~mm)<<1) + m;
}

// returns binomoial coefficient {n\choose p}
// not robust : suppose that n and p are nonnegative and that p <= n
uint_t binome(int n, int p)
{
	uint_t r=1;
	int i;
	int q;
	q=n-p;
	if (q<p)
	{	// {n choose p} = {n choose (n-p)}, compute the faster
		p=q;
	}
	for (i=1;i<=p;i++,n--)
	{
		r=((duint_t)r*(duint_t)n)/(duint_t)i;
	}
	return r;
}


//int_t MINI;
// Schemaenz est la dernière version qui marche 
// m=n+k tel que n est la taille de l'alphabet. et k est le poids.  
// nbr: Le numéro dont on cherche ses successeurs. 
// Le parcours du nbr commence par l'indice 0 
// ptails est la table des queues
// renvoie la dimension du carré dans lequel se trouve nbr
int schemaenz(uint_t*tab_sc,uint_t*ptails,uint_t nbr, int m, int k)
{	
    	//***************** Les variables ***************** 
	int n; // taille de l'alphabet. 
	int j; // sert à parcourir les sucesseurs qui sont à l'intérieur du schéma en Z. 
	// Les variables pour parcourir notre arbre. 
	uint_t new_min;	// La borne inférieure de l'intervale. 
	uint_t b;		// binome(m,k)
	uint_t new_max;	// La borne supérieure de l'intervalle. 
	int i;	// parcours la table des successeurs. (le i va de 0 à n-1).
	uint_t c;	// représente les coins, c'-à-d le premier successeur de chaque grand carré
	int dim;	// dimension du carré dans lequel se trouve nbr
	uint_t tail;

	i=0;
	new_min=0;
	n=m-k+1;	// taille de l'alphabet
	c=0; //min_successeurs_bis(n,k-1);
	//nbr -=	MINI;//numsubset(((1<<(k-1))-1)<<n,m-1);	
    	dim=n;
	tab_sc[0]=c+nbr; // initialiser tab_sc[0] dans notre cas tab_sc[0]=113 avec c=93 et p=0

	
	//***************** Le parcours dans l'arbre ***************** l
	b= binome(m,k);
	tail=b;
	new_max=b;		// binome(m-1,k-1)
	while(nbr>new_min)	// Le test d'arrêt: tant qu'on n'a pas atteind le nbr.  
	{
		if( nbr < new_max )
		{
			// si le nombre est dans l'intervalle [min, max[, descendre dans l'arbre
			b=((duint_t)b*(duint_t)k)/(duint_t)m;	// (m-1 choose k-1 ) = (m choose k) * k / m
			m--;
			k--;		// b == binome(m,k)
			tail=new_max;
			//printf("tail vaut %d\n",tail);
			new_max=new_min+b;
			continue;
		}
		
		while( nbr >= new_max ) 
		{	// si nbr est hors de l'intervalle, parcourir les fils vers la gauche
			c += (duint_t)b*(duint_t)(m+1)/(duint_t)(k+1); //binome(m+1,k+1);
			b=((duint_t)b*(duint_t)(m-k))/(duint_t)m;	// (m-1 choose k) = (m choose k) * (m-k) / m
			m--;
			new_min=new_max;
			new_max=new_min+b;
			i++;
			dim--;
			tab_sc[i]=c + nbr - new_min;
			if (ptails!=NULL)
			{
			       ptails[i]=tail;
			}
		}
	}
	for(j=i+1;j<n;j++)
	{	// les autres successeurs sont obtenus en ajoutant 1
		tab_sc[j] = tab_sc[j-1] + 1;
	}
	return dim;
}


#define MAX_N 60
// rend l'ensemble des successeurs de x
node_t succ_elt(uint_t x,int n, int k)
{
	node_t p;
	int i;
	uint_t t[MAX_N];

	p=NIL;
	schemaenz(t,NULL,x,n+k,k+1);
	for (i=0;i<n;i++)
	{
		include_elt(t[i],&p);
	}
	return p;

}


// rend l'ensemble des successeurs de x avec l'algorithme naif
node_t succ_elt_1(uint_t x, int n, int k)
{
	node_t p;
	int i;
	u64 m;
	u64 ens;
	i=0;
	m=1;
	p=NIL;
	ens=subsetnum(x,n+k-1); // L'ensemble contient le bitmap associé au numéro x
	while (i<n)
	{
		if ((ens&m)==0)
		{
			include_elt(numsubset(insere_un_bis(ens,m),n+k),&p);
			i++;
		}
		m<<=1;
	}
	return p;
}

node_t succ_intvl(uint_t a, uint_t b, int n, int k)
{
	int m;
	uint_t ta[MAX_N];
	uint_t tb[MAX_N];
	int i;
	int dim_a;
	uint_t tailsa[MAX_N];

	node_t p;

	m=n+k-1;
	if (a==b)
	{
		return succ_elt(a,n,k);
	}

	p=NIL;
	// successeurs de a
	dim_a=schemaenz(ta,tailsa,a,m+1,k+1);
	// successeurs de b
	schemaenz(tb,NULL,b,m+1,k+1);
	
	// inclure le schéma en z
	include_intvl(ta[n-dim_a],tb[n-1],&p);
	// inclure les autres colonnes
	for (i=0;i<n-dim_a;i++)
	{
		if ( tb[i]-ta[i] == b-a )
			include_intvl(ta[i],tb[i],&p);
		else
			include_intvl(ta[i],ta[i]+tailsa[i]-a-1,&p);
	}
	return p;
}




// successeur d'un interval avec l'algorithme naif
node_t succ_intvl_1(uint_t a, uint_t b, int n, int k)
{
	node_t p, q;
	uint_t i;
	p=NIL;
	for (i=a; i<=b;i++)
	{	// parcours tout l'intervalle et ajoute tous les successeurs des éléments
		q=succ_elt_1(i,n,k);
		include_set(q,&p);
		empty_out(&q);
	}

	return p;
}

//int affiche_succ_ele(int nbr, int)+

#define K_MAX	10000

node_t init(int n, int k)
{
	node_t q;
	uint_t min, max;
	int i;
	int j;
	int m;
	int tabens[K_MAX];
	u64 x;
	int res;
	m=n+k-1;
	min = numsubset(((1l<<k)-1)<<(n-1), m);
	max = numsubset((1l<<k)-1,m);
	q=NIL;
	for (i=min; i<=max ; i++)
	{
		// calcul du bitmapl
		x=subsetnum(i,m);
		tab_num(x,tabens,m);	// transforme le bitmap en table d'indices strict croi
		sous_tab_num(tabens,k);	// change en indice de suite 0..n-1
		res=0;
		for (j=0;j<k;j++)
		{
			res=loi(res,tabens[j]);

		}
		if (res==0)
		{
			include_elt(i-min,&q);
		}
	}
	//printf("nb de suites de somme nulle = %ld\n", cardinal(q));l
	return q;
}


// remplace pp par ses successeurs
void successeurs(node_t*pp,int n, int k)
{
	node_t e;
	set_t p1,p2;
	int m;

	uint_t total;
	uint_t courant;
	uint32_t percent,percent1;

	total=size_avl(*pp);
	m=n+k-1;
	set_min_max(0,binome(m+1,k+1)-1); //max1-min1);
	e=start_node(*pp);
	p2=NIL;
	courant=0;
	percent1=0;
	while (e!=NIL)
	{
		p1=succ_intvl(lo_bound[e],up_bound[e],n,k);
		include_set(p1,&p2);	// p2 = p2 union p1
		empty_out(&p1);		// vider p1
		e=next_node();
		courant++;
		percent=courant*100/total;
		if (percent!=percent1)
		{
			fprintf(stderr,"\e[4D%u %%",percent);
			fflush(stderr);
		}
		percent1=percent;
	}
	*pp=p2;
	fprintf(stderr,"\e[5D");
}

node_t init_to(int kk)
{
	node_t q;
	node_t p;
	uint_t min, max;
	int i;
	int j;
	int m;
	int tabens[K_MAX];
	u64 x;
	int res;
	int n;
	int k;
	n=ordre;
	k=exposant;
	m=n+k-1;
	min = numsubset(((1l<<k)-1)<<(n-1), m);
	max = numsubset((1l<<k)-1,m);
	p=NIL;
	for (i=min; i<=max ; i++)
	{
		// calcul du bitmapl
		q=NIL;
		set_min_max(0,binome(m+1,k+1)-1); //max1-min1);

		x=subsetnum(i,m);
		tab_num(x,tabens,m);	// transforme le bitmap en table d'indices strict croi
		sous_tab_num(tabens,k);	// change en indice de suite 0..n-1
		res=0;
		for (j=0;j<k;j++)
		{
			res=loi(res,tabens[j]);
		}
		if (res==0)
		{
			include_elt(i-min,&q);
		}
		//print_set(q);
		for (j=k;j<kk;j++)
		{
			successeurs(&q,n,j);
		}
		include_set(q,&p);
		printf("**[%d : %d]**",i,size_avl(p));

	}
	//printf("nb de suites de somme nulle = %ld\n", cardinal(q));l
	return p;
}

//n: L'alphabet
//x: Le numéro dont nous souhaiterons trouver ses succésseurs. 
//k: Le poids de Hamming de x
//t: La table des succésseurs 
int successeurs_bis(u64*t, u64 x, int n,int k)
{
	int i;
	u64 m;
	u64 ens;
	i=0;
	m=1;
    ens=subsetnum(x,n+k-1); // L'ensemble contient le bitmap associé au numéro x
	while (i<n)
	{
		if ((ens&m)==0)
		{
			t[i++]=numsubset(insere_un_bis(ens,m),n+k);
//insere_un_bis(x,m);   
		}
		m<<=1;
	}
   
   /* for(i=0;i<n;i++){
        printf("les successeurs sont: tab[%d]=%llx\n",i,t[i]);
    }*/
	return i;
}


// But: la fonction init_b sert à l'initialisation: c'est à dire séléctionner toutes les séquences de longueur e=exposant du groupe et de somme nulle. 
// La fonction init_b est une amélioration de la fonction init_a, ici on ne fait pas de recherche exaustive de toutes les séquence de longueur k pour selectionner celle qui ont une somme nulle. 
// Idée: une séquence de longueur n et de somme nulle s'obtient comme une séquence de longueur n-1 et de somme u dont on lui rajoute le terme -u
// Ce qu'on fait est qu'on applique une formule recursive qui dit que : 
// "l'ensemble des suites de longueur n et de somme i est égale à la réunion pour tous les éléments u du groupe de l'ensemble des suites de longueur n-1 et de somme u concaténer avec (i-u)"
// E_{n}(i)= U_{u\in G} E_{n-1}(u)+(i-u)

node_t init_b(int n,int e)
{
	set_t tp[100]; 
	set_t tq[100];
	set_t r; 
	set_t *p;
	set_t *q;
	set_t *t;
	int tN[100];
	int tM[100];
	int *N,*M,*T;

	int i;
	//int n;
	int u,v;
	int k;
	
	n=ordre;
	//k=1;

	// Initialisation des ensembles tp et tq de taille n à l'ensemble vide  
	for (i=0;i<n;i++)
	{
		tp[i]=EMPTY_SET;
		tq[i]=EMPTY_SET;
	}

	// Remplir l'ensmeble tp par les éléments successives du group tp[n]=[0],[1], ... [n-1].  

	for (i=0;i<n;i++)
	{
		// tp[i]=i
		include_elt(i,&tp[i]);
		//print_set(tp[i]);
	}

	// Remplir le tableau TM par les éléments successives du group  TM[n]={0,1,...,n-1}
	for (u=0;u<n;u++){
	tM[u]=u;
	}

	tN[0]=0;

	// On crée des copies pour les tableauxs TM et TN et des ensembles tp et tq
	N=tN;
	M=tM;
	p=tp;
	q=tq;

	// On commence par k=2 et on incrémente jusqu'à k= exposant du groupe. 
	for (k=2;k<=e;k++) 
	{
	//		printf("\n");
	//		printf("le k ici vaut %d\n",k);

	     // Remplir le tableau N[n] par la formule récursive: N[u]=N[u-1]+binome(n+k-u-1,k-1). 
		 // Le tableau N contiendra les numéros représentant le début de chaque grand carré. 	
		for (u=1;u<n;u++)   
		{
			N[u]=N[u-1]+binome(n+k-u-1,k-1);
	//		printf("N[%d] vaut %d\n",u,N[u]);
		}
	//	printf("******\n");

	//	printf("**********\n");
		for (i=0;i<n;i++)
		{
			for (u=0;u<n;u++)
			{
	//			printf("i vaut %d\n", i );
	//			printf("u vaut %d\n",u);
	//			printf("k vaut %d\n",k);
				// v=i-u modulo n 
				v=group_sub(i,u); // Pour n=5 et k=3 on fait la soustraction modulo n.
				// copier dans r la valeur de tp[i-u] autrement dit r=tp[i-u]
	//			printf("v vaut %d\n",v);
				r=copy_set(p[v]);
	//			printf("r vaut \n");
	//			print_set(r);

				// Enlever l'interval [0] de l'ensemble r, 
				// puis enlever 	  [0,1] de l'ensemble r, 
				// ...				  [0,n-1] de l'ensemble r. 

				if (u>0)  
				{
					//ôte l'intervalle [0,M[u]-1] de l'ensemble r
					exclude_intvl(0,M[u]-1,&r);
	//					printf("ote l'intervalle r vaut \n");
	//					print_set(r);
				}
				
				// Translater le noeud r par N[u]-M[u]
				// La translation revient à faire: r=r[u]+(N[u]-M[u])
				translate_set(&r,N[u]-M[u]);
					
	//			printf("Translater le noeud r par N[%d]-M[%d]\n", u,u);
	//  			print_set(r);
				
				//inclure r dans q+i c'est à dire inclure l'ensemble r à la position i de q

				include_set(r,q+i);

	//				printf("le noeud q vaut maintenant\n");
	//				print_set(q[i]);

				// libérer r 
	//			printf("r vaut\n");
	//			print_set(r);

				empty_out(&r);
	//			printf("r vaut\n");
	//			print_set(r);
			}
			if (k==e)
				break;
				
		}
		for (u=0;u<n;u++)
		{
	//		printf("rentre ici2\n");
			empty_out(&p[u]);
	//		printf("rentre ici3\n");
		}
	//	printf("***** changer les initiatisation ici **** \n");
		t=p;
		p=q;
		q=t;
		T=N;
		N=M;
		M=T;
	}
	return p[0];
}

// n = ordre du groupe
// cette fonction calcule le nombre minimal d'éléments du groupe tel que
// toute séquence de longueur n contiennent un sous-séquence de somme nulle de
// longueur inférieure ou égale à val
// Attention val doit être supérieur ou égale à l'exposant du groupe.
int egz_infval(int n,int val){
    
    set_t tp[100];
    set_t tq[100];
    set_t r;
    set_t *p;
    set_t *q;
    set_t *t;
    int tN[100];
    int tM[100];
    int *N,*M,*T;
    set_t S1;
    int ff;
    S1=EMPTY_SET;
    include_elt(0,&S1);
    int i;
    int u,v;
    int k;
    //int ff;
    
    for (i=0;i<n;i++)
    {
        tp[i]=EMPTY_SET;
        tq[i]=EMPTY_SET;
    }

    // Remplir l'ensmeble tp par les éléments successives du group tp[n]=[0],[1], ... [n-1].

    for (i=0;i<n;i++)
    {
        include_elt(i,&tp[i]);

    }

    // Remplir le tableau TM par les éléments successives du group  TM[n]={0,1,...,n-1}
    for (u=0;u<n;u++){
    	tM[u]=u;
    }

    tN[0]=0;

    // On crée des copies pour les tableauxs TM et TN et des ensembles tp et tq
    N=tN;
    M=tM;
    p=tp;
    q=tq;

    // Première boucle: le calcul des successeurs et l'inclusion de q[0]
    for (k=2;k<=val;k++)
    {
	    printf("k=%d\n",k);
	    fflush(stdout);
        for (u=1;u<n;u++)
        {
            N[u]=N[u-1]+binome(n+k-u-1,k-1);
        }

    //    p contient E_{k}
    //    q contiendra E_{k+1}
    //    calcul des E_{k+1}(i)
    // Formule de réucurrence pour calculer l'ensemble des séquence de somme i et de longueur k+1:
    // E_{k+1}(i)= U_{u \in G} E_{k}(u)+(i-u)
	uint32_t	total;
	uint32_t	current;
	uint32_t	percent, percent1;
	total=n*n;
	current=0;
	percent1=0;
        for (i=0;i<n;i++)
        {
            for (u=0;u<n;u++)
            {
		    current++;
                //printf("i vaut %d\n",i);
            //    printf("u vaut %d\n",u);
                v=group_sub(i,u);
            //    printf("v vaut %d\n",v);
                r=copy_set(p[v]);
                if (u>0)
                {
                    exclude_intvl(0,M[u]-1,&r);
    
                }
                translate_set(&r,N[u]-M[u]);
                include_set(r,q+i);
                empty_out(&r);
                percent=current*100/total;
	   	if (percent!=percent1)
	    	{
	    		fprintf(stderr,"\e[4D%u %%",current*100/total);
	    		fflush(stderr);
			percent1=percent;
	    	}

            }
            if (k==val)
                break;
        }
    	printf("\n");
    //    return 0;
        //Etape d'initialisation S1 contient à l'étape k les séquences de somme nulle et de longueur k ainsi que les successeur des numéros de l'étapes k-1.
        // On continue ainsi jusqu'à l'obtention de E_{val}(0) c-a-d jusqu'à l'obtention de toutes les séquences de longueur val et de somme nulle.
        //return 0;
        printf("k-1 vaut %d\n",k-1);
//        print_set(S1);
        //print_set(q[0]);
        include_set(q[0],&S1);
//        print_set(S1);
        successeurs(&S1,n,k); //successeur de 0 de longueur 2-1=1.
 //       printf("les successeurs au plus val sont \n");
 //       print_set(S1);
        //printf("\n");
        //include_set(q[0],&S1);
//        print_set(q[0]);
    //    return 0;
//        printf("tous les successeurs au plus val sont \n");
//        print_set(S1);
        ff=is_full(S1);
        //printf("ff vaut %d\n",ff);
        printf("k au final vaut %d\n",k);
        if(ff!=0)
	{
            printf("ff vaut %d\n",ff);
            return k;

        }
        
        for (u=0;u<n;u++)
        {
            empty_out(&p[u]);
    
        }

        t=p;
        p=q;
        q=t;
        T=N;
        N=M;
        M=T;
    }
    for (u=0;u<n;u++)
	    empty_out(p+u);
            
    // à partir de val: S1 contiendra seulement les successeurs
    
    //k=k-1;
    //printf("ff vaut %d\n",ff);
    do{
        printf("k vaut %d\n",k);
        fflush(stdout);
        successeurs(&S1,n,k);
//        printf("les successeurs sont pour k vaut %d\n",k);
//        print_set(S1);
        ff=is_full(S1);
        k++;
  //      printf("k à la fin est %d\n",k);
	
	if (k> 64-ordre)
	{
		printf("k est supérieur à %d\n",k);
		fflush(stdout);
		return k-1;
	}
	printf("nb intervalles = %u, nb free nodes = %u\n", size_avl(S1), nb_free_nodes());
	fflush(stdout);

    } while(ff==0);

    fflush(stdout);

    return k-1;
}


extern jmp_buf env;

int main()
{
	//*********************************
	//1. Tests/affichages fonctions simple 
	/*prologue(40000);
	set_t vv;
	set_t w;
	set_t qq;
	set_t tq[100];
	int o;
	for (o=0;o<4;o++)
	{
		tq[o]=EMPTY_SET;
	}
	set_t* q;
	q=tq;
	qq=NIL;
	vv=EMPTY_SET;
	w=EMPTY_SET;
	include_intvl(5,10,&vv);
	include_intvl(25,38,&vv);
	include_elt(20,&vv);
	printf("vv vaut \n");
	print_set(vv);
	printf("vv décalé de 1 vaut \n");
	translate_set(&vv,1);
	print_set(vv);
	//w=copy_set(vv);
	//print_set(w);
	include_set(vv,&qq);
	printf("qq vaut \n");
	print_set(qq);
	//include_set()
	qq=EMPTY_SET;
	for(o=0;o<4;o++){
		print_set(qq);
		include_set(qq,&tq[0]);
		print_set(tq[0]);
		empty_out(&qq);
		include_elt(o,&qq);
	}
	return 0;*/
	//*********************************
	/*printf("Hello egz!\n");
	if ((setjmp(env))!=0)
	{
		return 0;
	}*/
	/*prologue(40000);
	set_t s;
	s=EMPTY_SET;
	include_intvl(0,3,&s);
	include_intvl(6,7,&s);
	print_set(s);
	successeurs(&s,4,2);
	print_set(s);
	return 0;*/
	//prologue(10000000);
	//*********************************
	// Test rapide  succ d'un interval 
	/*	prologue(10000000);
	printf("Hello egz!\n");
	if ((setjmp(env))!=0)
	{
		return 0;
	}

	node_t s;
	s=EMPTY_SET;
	int k=3;
	int ff;
	int n;
	n=4;
	
	//printf("ff vaut %d\n",ff);
	include_intvl(0,0,&s);
	include_intvl(6,7,&s);
	include_intvl(11,11,&s);
	include_intvl(18,18,&s);
	print_set(s);
	do{
	fflush(stdout);

	successeurs(&s,n,k);
//	printf("s est \n");
//	print_set(s);
	ff=is_full(s);
//	printf("ff vaut %d\n",ff);
	k++;
	printf("k vaut %d\n",k);
	}while(ff==0);

	printf("ff vaut %d\n",ff);
	fflush(stdout);
	return 0;
*/
//	s=succ_intvl(13,17,5,3);
	//print_set(s);
//	}
//bye();
//	return 0;
	//*********************************
	/*int n;
	int k;
	n=ordre;
	//n=4;
	printf("ordre vaut %d\n",n);
	k=exposant;
	//k=3;
	printf("exposant vaut %d\n",k);
	int m=n+k-1;
	int max=numsubset((1<<k)-1,m);
	printf("max vaut %d\n", max);

	set_min_max(0,max);
	clock_t h;
	double dh;
	h=clock();


	// p=init_a(k); //init_a: la foction initialisation naive 
	clock_t h0=clock()-h;
	dh=((double)h0)/CLOCKS_PER_SEC;
	printf("[%lf sec.]\n",dh);

	int ff;
	node_t p;*/

	//*********************************

/*	// Affichage EGZ Classique avec init_b
	prologue(10000000);
	printf("Hello egz!\n");
	if ((setjmp(env))!=0)
	{
		return 0;
	}
	int n;
	int k;
	int i;
	n=ordre;
	k=4;
	set_t p;
	int ff;
	p=init_b(n,k);
	print_set(p);
	printf("\n");
	do
	{
		printf("-%d-",k);
		fflush(stdout);
		successeurs(&p,n,k);
		print_set(p);
		ff=is_full(p);
		k++;
		printf("**[%d]**",size_avl(p));
	}
	while (ff==0);
	printf("egz=%d\n",k);
	fflush(stdout);

	bye();
	*/
	//*********************************
	// Programme EGZ avec longueur des sous-séquence de somme nulle inférieur que val. 

	//*********************************
	
	prologue(1000000000);
	printf("Hello egz!\n");
	if ((setjmp(env))!=0)
	{
		return 0;
	}

	int s;
	int n=ordre;
	int val=2;
	s=egz_infval(n,val);
	printf("EGZ vaut %d\n",s);

	return 0;
/*
	int n;
	int k;
	int i;
	n=ordre;
	k=4;
	set_t p;
	int ff;
	p=init_b(n,k);
	print_set(p);
	printf("\n");
	do
	{
		printf("-%d-",k);
		fflush(stdout);
		successeurs(&p,n,k);
		print_set(p);
		ff=is_full(p);
		k++;
		printf("**[%d]**",size_avl(p));
	}
	while (ff==0);
	printf("egz=%d\n",k);
	fflush(stdout);
	
	bye();*/


	//print_set(p);
	/*
	int n;
	int k;
	int i;
	n=ordre;
	printf("ordre vaut %d\n",n);
	k=exposant;
	printf("exposant vaut %d\n",k);
	int m=n+k-1;
	int max=numsubset((1<<k)-1,m);
	printf("max vaut %d\n", max);

	set_min_max(0,max);
	clock_t h;
	double dh;
	h=clock();
	clock_t h0=clock()-h;
	dh=((double)h0)/CLOCKS_PER_SEC;
	printf("[%lf sec.]\n",dh);

	set_t tp[100]; // Contient E_e(0)
	set_t S1; 
	set_t* p_tp;
	set_t r; 
	S1=EMPTY_SET;
	//Première étape: Initialisation 

	for (i=0;i<n;i++)
	{
		tp[i]=EMPTY_SET;
	}
	for (i=0;i<n;i++)
	{
		// tp[i]=i
		include_elt(i,&tp[i]);   
		print_set(tp[i]);
	}
	//p_tp=tp;
//	print_set(tp[0]);
//	print_set(p_val);
	include_set(tp[0],&S1);
	printf("S1 contient\n");
	print_set(S1);
	successeurs(&S1,n,1);
	printf("Les successeurs de S1 sont \n");
	print_set(S1);

	int ff;
	node_t p;

	p=init_b(n,k);
	printf("\n");
	do
	{
		printf("-%d-",k);
		fflush(stdout);
		successeurs(&p,n,k);
	//	print_set(p);
		ff=is_full(p);
		k++;
		printf("**[%d]**",size_avl(p));
	}
	while (ff==0);
	printf("egz=%d\n",k);*/
//	fflush(stdout);
	
//	bye();
/*
u64 tab_succ[1000000]; // Comme ca quand on appelle la fonction succ_sing dans le main avec succ_sing(tab,8,4) tab sera déjà déclarée en variable globale.
    //int t[1000000]; 
	int t_tab_succ; // le nombre d'éléments dans la table. 
    //int premier;
   // int dernier;
    int n;
    int k;
    int m;
    int i;
    int j;
    n=4;
    k=3;
    m=n+k-1;
    
    
    int min;
    int max; 
    min=numsubset(((1<<k)-1)<<(n-1),m);	// 1...1 0...0  (le plus petit numéro)
	max=numsubset((1<<k)-1,m);		// 0...0 1...1 (le plus grand numéro)
    printf("min vaut %d\n",min);
    printf("max vaut %d\n",max);
    
	for(i=min;i<=max;i++){
         t_tab_succ=successeurs_bis(tab_succ,i,n,k);
        // printf("la valeur de la dernière case du tableau vaut %d\n",tab_succ[0]);
            printf("[%d] : ",i-22);
            for(j=0;j<t_tab_succ;j++){
                printf("%d ", tab_succ[j]-64);
                
            }
            printf("\n");

	}

   printf("*****************\n");
    for(i=0;i<max-min+1;i++){
     schemaenz(i,n+k,k+1);
       fprintf(fichier,"[%d] : ",i);
     for(j=0;j<n;j++){
        t[j]=tab_sc[j];
        fprintf(fichier,"%d  ", t[j]);
    }
     fprintf(fichier,"\n");

    }*/

 	return 0; 
 
}

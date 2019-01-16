#include <stdio.h>
#include <stdlib.h>

//最大公約数の計算
int GCD(int e, int L)
{
 int mod;

 if((e == 0)||(L == 0))
   return(0);
 while(1){
   mod = L % e;
   if(mod == 0)
     break;
   L = e;
   e = mod;
 }
 return(e);
}

//最大公約数であった場合処理を抜ける
int ecalc (int L)
{
 int e,result;

 for (e = 2; e < L; e++){
   if (GCD(e,L) == 1){
     result = e;
     break;
   }
 }
 return(result);
}

int ex_euclid(int flag)
{
 int L,e;
 int a;
 int b;        /* i=1,2,3, ... */
 int f0,f1,f2; /* f_{i-2}, f_{i-1}, f_{i} */
 int g0,g1,g2; /* g_{i-2}, g_{i-1}, g_{i} */
 int r0,r1,r2; /* r_{i-2}, r_{i-1}, r_{i} */
 int p,q,r;
 int i,j;
 int n;
 int cnt=0;

 printf("公開鍵LCMとなる素数p,qを入力 \n");
 printf("p : "); scanf("%d", &p);
 printf("q : "); scanf("%d", &q);
 L = ((p-1) * (q-1))/GCD((p-1),(q-1));
 e = ecalc(L);
 n = p * q;
 if(flag == 1){
   printf("公開鍵の表示 \n");
   printf("(e,n)=(%d,%d) \n",e,n);
 }
 r0=L;
 r1=e;
 f0=1;
 f1=0;
 g0=0;
 g1=1;

 while(r1!=0){
   cnt = cnt + 1;
   r2 = r0%r1;
   q = (r0 - r2)/r1;

   f2 = f0 - q*f1;
   g2 = g0 - q*g1;

   if(r2 == 0){
     if(flag == 2){
       printf("秘密鍵の表示 \n");
       printf("(d_e,n)=(%d,%d) \n",g1,n);
     }
     return(n);//暗号化・復号化に必要なnを返す

   }

   f0=f1;
   f1=f2;
   g0=g1;
   g1=g2;
   r0=r1;
   r1=r2;
 }
}

int convert(int n, int cc, int key){
 int i;
 int base;

 base = cc;
 //printf("base : %d \n",base);
 for(i = 1; i != key; i++){
   //printf("(%d x %d) mod %d \n",cc,base,n);
   cc = (cc * base) % n;
 }
 //printf("MOD : %d \n",cc);
 return(cc);
}

int encode(int n){
 FILE *fopen(), *fpr, *fpw;
 int B = 256;
 int B0,B1;
 int pubkey;
 int cc;
 char namer[20], namew[20];

 printf("暗号化するファイル名を入力 : "); scanf("%s",namer);
 printf("出力するファイル名を入力 : "); scanf("%s",namew);
 printf("公開鍵を入力 : "); scanf("%d",&pubkey);
 if((fpr=fopen(namer,"r")) == NULL)
   exit(-1);
 if((fpw=fopen(namew,"w")) == NULL)
   exit(-1);
 while((cc=fgetc(fpr)) != EOF){
   //printf("%c,%d \n",cc,cc);
   cc = convert(n,cc,pubkey);
   B0 = cc % B;
   B1 = (cc -B0)/B;
   //printf("(B1,B0)=(%d,%d) \n",B1,B0);
   fputc(B0,fpw);
   fputc(B1,fpw);
 }
 fclose(fpr);
 fclose(fpw);
}

int decode(int n){
 FILE *fopen(), *fpr, *fpw;
 int B = 256;
 int B0,B1;
 int seckey;
 int cc,result;
 char namer[20], namew[20];

 printf("復号化するファイル名を入力 : "); scanf("%s",namer);
 printf("出力するファイル名を入力 : "); scanf("%s",namew);
 printf("秘密鍵を入力 : "); scanf("%d",&seckey);
 if((fpr=fopen(namer,"r")) == NULL)
   exit(-1);
 if((fpw=fopen(namew,"w")) == NULL)
   exit(-1);
 while((cc=fgetc(fpr)) != EOF){
   B0 = cc;
   B1 = fgetc(fpr);
   cc = (B1 * B) + B0;
   //printf("cc : %d \n",cc);
   cc = convert(n,cc,seckey);
   fputc(cc,fpw);
 }
 fclose(fpr);
 fclose(fpw);
}

int main(void){
 int flag;
 int n;

 start:
 printf("暗号化:1,復号化:2,終了:3 : ");
 scanf("%d",&flag);
 if(flag == 1){
   n = ex_euclid(flag);
   encode(n);
 }
 else if(flag == 2){
   n = ex_euclid(flag);
   decode(n);
 }
 else if(flag == 3){
   printf("終了します \n");
 }
 else{
   goto start;
 }
}

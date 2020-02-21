#include<iostream>
#include<omp.h>
#include<math.h>

using namespace std;

void print(int *a,int n)
{
    for(int i=0;i<n;i++)
    {
        cout<<a[i]<<" ";
    }
    cout<<"\n";
}

void printR(int **r,int n)
{
    for(int i=0;i<2*n-1;i++)
    {
        print(r[i],n);
    }
}


void comprimare(int **r,int n,int j)
{
    int k;
    int m=log2(n);
    int i;
    for(k=m-1;k>=0;k--)
    {
        #pragma omp paralle for
        for(i=(1<<k)-1;i<=(1<<(k+1))-2;i++)
        {
            r[i][j]=r[2*i+1][j]+r[2*i+2][j];
        }
    }
}

void sortareMullerPreparata(int *a,int **r,int n)
{
    int i,j;
    int *p=new int[n]{0};
    #pragma omp paralle for
    for(i=0;i<n;i++)
    {
        for(j=0;j<n;j++)
        {
            if(a[i]<a[j])
            {
                r[i+n-1][j]=1;
            }
            else
            {
                r[i+n-1][j]=0;
            }
        }
    }
    cout<<"creare R:\n";
    printR(r,n);
    for(i=0;i<n;i++)
    {
        comprimare(r,n,i);
        p[i]=r[0][i];
        cout<<"creare R dupa comprimare pasul "<<i<<" :\n";
        printR(r,n);
    }
    cout<<"\n\n";
    int *aux=new int[n]{0};
    print(p,n);
    for(i=0;i<n;i++)
    {
        aux[i]=a[p[i]];
    }
    print(aux,n);
    for(i=0;i<n;i++)
    {
        a[i]=aux[i];
    }
    delete[] aux;
    delete[] p;
    print(a,n);
}

int main()
{   
    int n=4;
    int *a=new int[n]{2,6,3,8};
    int **r=new int*[2*n-1]{0};
    for(int i=0;i<2*n-1;i++)
    {
        r[i]=new int[n]{0};
    }

    print(a,n);
    cout<<"buna\n";
    printR(r,n);
    omp_set_num_threads(n);
    sortareMullerPreparata(a,r,n);
    cout<<"a sortat:\n";
    print(a,n);
    delete[] a;
    for(int i=0;i<2*n-1;i++)
    {
        delete[] r[i];
    }
    delete[] r;
    cout<<"dealocare";
    return 0;
}
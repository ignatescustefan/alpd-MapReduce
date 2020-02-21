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

void comprimare(int *a,int m)
{
    for(int k=m-1;k>=0;--k)
    {
        omp_set_num_threads(1<<k);
        #pragma omp parallel for
        for(int j=1<<k;j<=(1<<(k+1))-1;j++)
        {
            a[j]=a[2*j]+a[2*j+1];
        }
    }
}

void calculPrefixe(int *a,int *b,int n,int m)
{
    comprimare(a,m);
    print(a,2*n);
    b[1]=a[1];

    int k;
    for(k=1;k<=m;k++)
    {
        omp_set_num_threads(1<<k);
        #pragma omp parallel for
        for(int j=1<<k;j<=(1<<(k+1))-1;j++)
        {
            if(j%2 ==1)
            {
                b[j]=b[(j-1)/2];
            }
            else
            {
                b[j]=b[j/2]-a[j+1];
            }
            
        }
    }
    print(b,2*n);
}

int main()
{
    int n=8;
    int m=log2(n);
    int dim=n*2;
    int *a=new int[dim]{0,0,0,0,0,0,0,0,3,2,1,6,5,4,2,9};
    int *b=new int[dim]{0};

    calculPrefixe(a,b,n,m);


    return 0;
}
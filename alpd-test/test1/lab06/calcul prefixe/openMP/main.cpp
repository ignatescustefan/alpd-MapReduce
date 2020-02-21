#include<iostream>
#include<math.h>
#include<omp.h>

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
    int k,j;
    for(k=m-1;k>=0;--k)
    {   
        omp_set_num_threads(k);
        #pragma omp parallel for
            for(j=(1<<k);j<=(1<<(k+1))-1;j++)
            {
            
                a[j]=a[2*j]+a[2*j+1];
            }
    }
}

void calculPrefixe(int *a,int *b,int m)
{
    comprimare(a,m);
    b[1]=a[1];
    int k,j;
    for(k=1;k<=m;k++)
    {
        omp_set_num_threads(k);
        #pragma omp parallel for
            for(j=(1<<k);j<=(1<<(k+1))-1;j++)
            {
                if(j%2==1)
                {
                    b[j]=b[(j-1)/2];
                }
                else
                {
                    b[j]=b[j/2]-a[j+1];
                }
                
            }
    }
}
int main()
{

    int n=8;
    int m=log2(n);
    int dim=n*2;
    int *a=new int[dim]{0,0,0,0,0,0,0,0,3,2,1,6,5,4,2,9};
    int *b=new int[dim]{0};

    print(a,dim);
    print(b,dim);

    //comprimare(a,m);
    calculPrefixe(a,b,m);
    print(a,dim);
    print(b,dim);

    return 0;
}
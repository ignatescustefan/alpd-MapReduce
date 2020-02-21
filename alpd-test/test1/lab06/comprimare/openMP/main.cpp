#include<iostream>
#include<omp.h>
#include<math.h>

using namespace std;



void print(int *a, int n)
{
    for(int i=0;i<n;i++){
        cout<<a[i]<<" ";
    }
    cout<<"\n";
}

void comprimare(int *a,int m)
{
    int k,j;
    for(k=m-1;k>=0;k--)
    {
        omp_set_num_threads(k);
        #pragma omp parallel for
            for(j=(1<<k);j<=(1<<(k+1))-1;j++)
            {
                a[j]=a[2*j]+a[2*j+1];
            }
    }
}

int main()
{
    int n=8;
    int m=log2(n);
    cout<<m<<"\n";
    int dim=2*n;

    int *a=new int[dim]{0,0,0,0,0,0,0,0,3,7,8,3,9,2,3,1};

    print(a,dim);
    comprimare(a,m);
    print(a,dim);
    cout<<"rezultat comprimare : "<<a[1]<<"\n";
    return 0;
}
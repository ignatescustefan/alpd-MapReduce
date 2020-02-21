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
void printV(int **r,int n)
{
    for(int i=0;i<2*n-1;i++)
        print(r[i],n);
}

void comprimare(int **r,int n,int j)
{
    int k;
    int m=log2(n);
    for(k=m-1;k>=0;k--)
    {
        int x=1<<k;
        int y=1<<(k+1);
        for(int i=x-1;i<=y-2;i++)
        {
            r[i][j]=r[2*i+1][j]+r[2*i+2][j];
        }
    }
}

void sortare(int *a,int **r,int n)
{
    int i,j,k;
    
    int *p=new int[n]{0};
    int *a_n=new int[n]{0};

    for(i=0;i<n;i++)
    {
        for(j=0;j<n;j++)
        {
            if(a[i]<a[j])
            {
                r[i+n-1][j]=1;
                r[i+n-1][j]=1;
            }
            else
            {
                r[i+n-1][j]=0;
            }
            
        }
    }
    cout<<"\n";
    printV(r,n);

    // do some magiic

    for(j=0;j<n;j++)
    {
        //comprim
        comprimare(r,n,j);
        p[j]=r[0][j];
        //si pun pozitie
    }
    for(i=0;i<n;i++)
    {
        a_n[i]=a[p[i]];
    }
    print(a_n,n);
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
    printV(r,n);

    sortare(a,r,n);
    return 0;
}
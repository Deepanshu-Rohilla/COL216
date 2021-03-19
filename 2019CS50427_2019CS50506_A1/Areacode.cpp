#include<iostream>
using namespace std;

int main(){

    int n;
    cin>>n;
    if(n<=0){
        cout<<"n valus should be greater than 0"<<"\n";
    }else if(n==1){
       int a,b;
       cin>>a>>b;
       cout<<"0.00"<<"\n"; 
    }
    else{
       int a,b;
       cin>>a>>b;
       int temp1,temp2;
       int num=n-1;
       double area=0.00;
       while(num>0){
           int x,y;
           cin>>x>>y;
           temp1=x;
           temp2=y;
           if(y*b<0){
               double ZeroPoint=(a*y-b*x)/((y-b)*1.00);
               area+=((ZeroPoint-a)*abs(b) + (x-ZeroPoint)*abs(y))/2.0;

           }else{
                area+=(0.5)*abs((y+b))*(x-a);
           }
           a=temp1;
           b=temp2;
           num--;
       }
       cout<<area<<"\n";

    }







    return 0;
}
#include<iostream>
#include<stack>
#include<math.h>
using namespace std;


int main(){

    string s;
    cin>>s;
    stack<int>st;
    int i=0;
    int j=0;
    while(i<s.length()){
        
         if(s[i]=='+'){
             if(st.size()<2){
                 cout<<"Wrong postfix Expression+"<<"\n";
                 j=-1;
                 break;
             }else{
             int x=st.top();
             st.pop();
             int y=st.top();
             st.pop();
             st.push(x+y);
             }
         }else if(s[i]=='-'){
             if(st.size()<2){
                 cout<<"Wrong postfix Expression-"<<"\n";
                 j=-1;
                 break;
             }else{
             int x=st.top();
             st.pop();
             int y=st.top();
             st.pop();
             st.push(y-x);
             }

         }else if(s[i]=='*'){
             if(st.size()<2){
                 cout<<"Wrong postfix Expression*"<<"\n";
                 j=-1;
                 break;
             }else{
             int x=st.top();
             st.pop();
             int y=st.top();
             st.pop();
             st.push(x*y);
             }
         }else if((s[i]-48)>=0 && (s[i]-48)<=9){
             st.push(s[i]-48);
         }else{
             cout<<"Other Character which was not allowed"<<"\n";
             j=-1;
             break;
         }
         i++;

    }

    if(j==0 && st.size()==1){
        cout<<st.top()<<"\n";
    }else if (j==0){
        cout<<"Wrong postfix Expression"<<"\n";
    }






    return 0;
}
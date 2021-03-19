string s

stack stk

i <- 0
j <- 0

while (i < s.length) do 
   
   if(s[i] = '+') do 

     if (stk.size() <2 ) do 
        print("Wrong Postfix Expression")
        j <- -1
        break

     else do 

      int x=stk.top();
      stk.pop();
      int y=stk.top();
      stk.pop();
      stk.push(x+y);

   else if (s[i] ='-') do

     if (stk.size() <2 ) do 
        print("Wrong Postfix Expression")
        j <- -1
        break

     else do 

      int x=stk.top();
      stk.pop();
      int y=stk.top();
      stk.pop();
      stk.push(y-x);

   else if (s[i] = '*') do

     if (stk.size() <2 ) do 
        print("Wrong Postfix Expression")
        j <- -1
        break

     else do 

      int x=stk.top();
      stk.pop();
      int y=stk.top();
      stk.pop();
      stk.push(x*y);

   else if ( (s[i] -48 )>=0 and (s[i] -48 )<= 9  ) do

       stk.push(s[i]-48)


   else do 

     print("Wrong Expression as it contain character which is not allowed")
     j <- -1
     break
   i <- i+1

if( j = 0 and stk.size()==1) do 
  ans <- stk.top()
  print (ans) 

else if(j==0) do
   print("Wrong Postfix Expression")


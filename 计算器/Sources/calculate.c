#include "math.h"
#include "stdio.h"
#include "string.h"
#include "1602.h"
#include "delay.h"


double calc(char s[],int l,int r){
  int fh=0, kh=0;
  int i, j;
  double temp=0, temps=0;
  double t=1;
  /***********处理优先级************/
  for (i=r;i>=l;i--){
    if (s[i]==')')
      kh++;                  //当括号标志位不等于0，即只有单括号时提示错误输入
    if (s[i]=='(')
      kh--;
    if ((s[i]=='+' || s[i]=='-') && kh==0){
      fh = i;
      break;
    }
    if ((s[i]=='*' || s[i]=='/') && (kh==0) && (fh==0 || s[fh]=='^' || s[fh]=='#'))
      fh = i;
    if ((s[i]=='^'||s[i]=='#') && kh==0 && fh==0)
      fh = i;
  //  else write_str_1602("error 1",0xc1,7);
  }                                             
  /***********处理符号位***********/
  if (fh==0){
    if (s[l]=='(' && s[r]==')')
      return calc(s,l+1,r-1);
    if (s[l]=='(')
      return calc(s,l+1,r);
    if (s[r]==')')
      return calc(s,l,r-1);
    if (l==r)
      return s[l]-'0';
    for (j=l;j<=r;j++){
      if(s[j]!='.')
        temp = temp * 10 + s[j] - '0';
        if(s[j]=='.'){
           j++;
           for(;j<=r;j++){
             temps = temps * 10 + s[j] - '0';
             t = t * 10;
           }
           temps = temps / t;
       }
    }
    temp = temp + temps;
    return temp;
  }
  else{
    if (s[fh]=='+')
      return calc(s,l,fh-1) + calc(s,fh+1,r);
    if (s[fh]=='-')
      return calc(s,l,fh-1) - calc(s,fh+1,r);
    if (s[fh]=='*')
      return calc(s,l,fh-1) * calc(s,fh+1,r);
    if (s[fh]=='/') {
      if(calc(s,fh+1,r) == 0) {
        write_str_1602("error 1",0xc1,7);
        return;
      } 
      else
        return calc(s,l,fh-1) / calc(s,fh+1,r);
    }
    if (s[fh]=='^')
      return pow(calc(s,l,fh-1),calc(s,fh+1,r));
    if (s[fh]=='#'){
      if(calc(s,l,fh-1)<0) {
        write_str_1602("error 2",0xc1,7);
      } 
      else
        return pow(calc(s,l,fh-1),1.0/calc(s,fh+1,r));
    }
  }
}

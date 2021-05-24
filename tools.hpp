#ifndef TOOLS_HPP
#define TOOLS_HPP
#include"include.hpp"

inline long long Multi(const long long &a,const long long &b,const long long &Mod)
{
    long long d=(long long)floor(a*(long double)b/Mod+0.5);
    long long ret=a*b-d*Mod;
    if(ret<0)   ret+=Mod;
    return   ret;
}

void hc(int x){std::cerr<<x<<std::endl;}
const long long mod1=999999999999999989;
const long long mod2=1ll*19260817*(1e9+7);

template<typename T>
T max(const T &x1,const T &x2){
    return (x1>x2)?x1:x2;}
template<typename T>
T min(const T &x1,const T &x2){
    return (x1<x2)?x1:x2;}
std::pair<long long,long long> pksHash(const char *S){
    std::pair<long long,long long> tmp=std::make_pair(0,0);
    for (int i=0;S[i]!='\0';i++)
    {
        tmp.first=(Multi(tmp.first,1000000007,mod1)+S[i])%mod1;
        tmp.second=(tmp.second*11+S[i])%mod2;
    }
    return tmp;
}
long long pksHash1(const char *S){
    long long tmp=0;
    for (int i=0;S[i]!='\0';i++)
        tmp=(Multi(tmp,1000000007,mod1)+S[i])%mod1;
    return tmp;
}
int pksSplit(const std::string &S,std::string tmp[],const char &splitsig){
    int cnt=1;
    tmp[0]="";
    int len=(int)(S.length());
    // exit(0);
    for (int i=0;i<len;i++)
    {
        if (S[i]!=splitsig)
            tmp[cnt-1]+=S[i];
        else {
            cnt++;
            tmp[cnt-1]="";    
            }
    }
    return cnt;
}
int StringCompare(const char S1[],const char S2[]){
    
    for (int i=0;;i++)
    {
        if (S1[i]<S2[i]) return -1;
        if (S1[i]>S2[i]) return 1;
        if (S1[i]=='\0') return 0;
    }
    return 0;
}
bool xiangdeng(const char S1[],const char S2[])
{
    for (int i=0;;i++)
    {
        if (S1[i]!=S2[i]) return 0;
        if (S1[i]=='\0') return 1;
    }
    return 0;
}
int StrToInt(const std::string &S)
{
    int x=0,len=(int)(S.length());
    for (int i=0;i<len;i++) x=x*10+S[i]-'0';
    return x;
}
int StrToMin(const std::string &S){
    std::string tmp[3];
    pksSplit(S,tmp,':');
    return StrToInt(tmp[0])*60+StrToInt(tmp[1]);
}
int StrToDate(const std::string &S){
    std::string tmp[3];
    pksSplit(S,tmp,'-');
    int mon=StrToInt(tmp[0]),day=StrToInt(tmp[1]);
    if(mon<6) return -66666;
    if (mon==6) return day-1;
    if (mon==7) return 30+day-1;
    if (mon==8) return 61+day-1;    
    if (mon==9) return 92+day-1;
    return 66666;
}
//1440min=1day
struct Date{
    int Time;
    Date(const int &_day,const int &_min):Time(_day*1440+_min){}
    Date(const int &time):Time(time){}
    Date(){}
    Date operator +(const int &x){
        return Date(Time+x); }
    Date& operator +=(const int &x){
        Time+=x;return (*this);
    }
    friend int operator - (const Date &X,const Date &Y){
        return X.Time-Y.Time;}
    int Day()const{
        return Time/1440;
    }
    int Min()const{
        return Time%1440;
    }
	
    friend std::ostream& operator << (std::ostream& os, const Date& cc)
	{
        int nowtian;
        int Day=cc.Day(),Min=cc.Min();
        if (Day<=29) {os<<"06-";nowtian=Day+1;}
        else if (Day<=60) {os<<"07-";nowtian=Day-30+1;}
        else if (Day<=91) {os<<"08-";nowtian=Day-30-31+1;}
        else {os<<"09-";nowtian=Day-30-31-31+1;}
        if (nowtian<10) os<<'0';
        os<<nowtian;
        os<<" ";
        int nowshi=Min/60,nowfen=Min%60;
        if (nowshi<10) os<<'0';
        os<<nowshi;
        os<<":";
        if (nowfen<10) os<<'0';
        os<<nowfen;
        return os;
	}
};
void SC(const std::string &X){
    std::cout<<X<<"\n";
}
/*
const int MX=50000;
template<typename T>
class Hash{//long long -->T
    sjtu::vector<T> ARR;
    
    
}*/
#endif
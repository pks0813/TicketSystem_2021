#ifndef CORN_HPP
#define CORN_HPP

#include"include.hpp"
const long long M=1e10;
int timeID=0;
int pksuseless;
int TMPLENGTH;
class Corn{
    public:
    class User{
        public:
        char Username[22],Password[31],Name[21],MailAddr[31];
        int Privilege;
        User( const char _Username[],
        const char _Password[],
        const char _Name[],
        const char _MailAddr[],
            int _Privilege):Privilege(_Privilege)
        {
            strcpy(Username,_Username);
            strcpy(Password,_Password);
            strcpy(Name,_Name);
            strcpy(MailAddr,_MailAddr);
        }
        User(){}
        ~User(){}
    };
    class TrainInfo{
        public:
        char TrainID[22];
        int StationNum;
        char Station[100][31];
        int Pricepre[100];
        int Seatnumber,Stoptime[100],Salestart,Saleending;
        Date Arrive[100];
        char Type;
        bool Release=0;
        TrainInfo(){}
        ~TrainInfo(){}
    };
    class Seat{
        public:
        int A[100];
    };
    class Order{
        public:
        int Day;//起始站的Day
        int TrainID;
        char TrainName[22];
        int SStation,TStation;
        char SStationName[31],TStationName[31];
        Date Sdate,Tdate;
        char type;//P=pending S=success R=refunded
        int Price,Num;
        int time;
    };
    class Ticket{
        public:
        char TrainID[22];
        Date SDate,TDate;
        char Sname[31],Tname[31];
        int price,time,Seatmax;
        
        friend void SC(const Ticket&X)
        {
            printf("%s %s ",X.TrainID,X.Sname);
            std::cout<<X.SDate;
            printf(" -> %s ",X.Tname);
            std::cout<<X.TDate;
            printf(" %d %d\n",X.price,X.Seatmax);
        }
        
        friend void Qsort(const int &L,const int &R,int P[],Ticket ARR[],bool (*Compare)(const Ticket&,const Ticket&)){
            int i,j,x;
            Ticket Midvalue=ARR[P[(L+R)/2]];
            i=L;j=R;
            do
            {
                while(Compare(ARR[P[i]],Midvalue) && i<=j)
                    i++;
                while(Compare(Midvalue,ARR[P[j]]) && i<=j)
                    j--;
                if(i<=j)
                {
                    x=P[i];
                    P[i]=P[j];
                    P[j]=x;
                    i++;
                    j--;
                }
            }while(i<=j); 
            if(L<j)
                Qsort(L,j,P,ARR,Compare);
            if(i<R)
                Qsort(i,R,P,ARR,Compare);
        }

    };
    BPT UserBPT,TrainBPT,StationTrainBPT,UserTimeBPT,TrainTimeBPT,TrainSeatBPT;
    StoragePoolPlus<User> Userpool;
    StoragePool<TrainInfo> Trainpool;
    StoragePoolPlus<Order> Orderpool;
    StoragePoolPlus<Seat> Seatpool;
    QQHash<int> Useronline;
    QQHash<std::pair<int,Date>> MPS;
    int Usernumber=0;
    int Ordertime=0;
    Corn():UserBPT("User.in"),TrainBPT("TrainBPT.in"),StationTrainBPT("StationTrainBPT.in"),UserTimeBPT("UserTimeBPT.in"),
            TrainTimeBPT("TrainTimeBPT.in"),TrainSeatBPT("TrainSeatBPT.in"),
            Userpool("Userpool.in",20000),Trainpool("Trainpool.in",5000),Orderpool("Orderpool.in",20000),Seatpool("Seatpool.in",10000),Useronline(200009),MPS(10007)
        {
            std::fstream info;
            std::string X="Useless.in";
            info.open(X,std::ios::in | std::ios::out | std::ios::binary);
            if (info.fail())
            {
                info.clear();
                std::ofstream fout(X);
                fout.close();
                info.open(X,std::ios::in | std::ios::out | std::ios::binary);
                info.seekp(0);
                info.write(reinterpret_cast<char *> (&Usernumber),sizeof(int));
                info.write(reinterpret_cast<char *> (&Ordertime),sizeof(int));  
            }     
            info.seekp(0);
            info.read(reinterpret_cast<char *> (&Usernumber),sizeof(int));
            info.read(reinterpret_cast<char *> (&Ordertime),sizeof(int)); 
            info.close(); 
        }
    ~Corn(){ 
        std::fstream info;
        info.open("Useless.in",std::ios::in | std::ios::out | std::ios::binary);
        info.seekp(0);
        info.write(reinterpret_cast<char *> (&Usernumber),sizeof(int));
        info.write(reinterpret_cast<char *> (&Ordertime),sizeof(int));  
        info.close();
        for (auto i=MP.begin();i!=MP.end();i++)
            std::cerr<<i->first<<' '<<i->second<<std::endl;
    }
    //UserController
    void add_user(std::string tmp[]){
        User tem;
        long long Q=pksHash1(tmp[2].c_str());
        auto Curiter=Useronline.Find(Q);
        if (Usernumber!=0 && Curiter.first==-1) {printf("-1\n");return;}
        strcpy(tem.Username,tmp[4].c_str());
        strcpy(tem.Password,tmp[6].c_str());
        strcpy(tem.Name,tmp[8].c_str());
        strcpy(tem.MailAddr,tmp[10].c_str());
        tem.Privilege=StrToInt(tmp[12]);
        bool flat=1;
        int IDcur;
        if (Usernumber==0) IDcur=-1;
        else IDcur=Curiter.second;
        if (IDcur==-1) flat=0;
        if (flat)
        {
            User CurUser;
            Userpool.Copy(IDcur,CurUser);
            if (CurUser.Privilege<=tem.Privilege) flat=0;
        }
        if (Usernumber==0) {flat=1; tem.Privilege=10;}
        if (flat)   
        {
            int Suc=UserBPT.Insert(pksHash(tem.Username),Userpool.cnt+1);
            if (Suc==-1) {printf("-1\n");}
            else {Userpool.Insert(tem);Usernumber++;printf("0\n");}
        }
        else printf("-1\n");    
    }
    void login(std::string tmp[]){
        std::pair<long long,long long> Q=pksHash(tmp[2].c_str());
        if (Useronline.Find(Q.first).first!=-1) {printf("-1\n");return;}
        int IDcur=UserBPT.Find(Q);
        if (IDcur==-1) {printf("-1\n");return;}
        User Curuser;
        Userpool.Copy(IDcur,Curuser);
        if (xiangdeng(tmp[4].c_str(),Curuser.Password)==0) printf("-1\n");
        else{
            Useronline.Insert(std::make_pair(Q.first,IDcur));
            printf("0\n");
        }
    }
    void logout(std::string tmp[]){
        long long Q=pksHash1(tmp[2].c_str());
        auto Curiter=Useronline.Find(Q);
        if (Curiter.first==-1) {printf("-1\n");return;}
        Useronline.Erase(Q);
        printf("0\n");
    }
    void query_profile(std::string tmp[]){
        long long CurQ=pksHash1(tmp[2].c_str());
        std::pair<long long ,long long> QueQ=pksHash(tmp[4].c_str());
        auto CurIter=Useronline.Find(CurQ);
        if (CurIter.first==-1) {printf("-1\n");return;}
        int IDCur=CurIter.second;
        int IDQue=UserBPT.Find(QueQ);
        if (IDQue==-1) {printf("-1\n");return;}
        User Curuser,Queuser;
        Userpool.Copy(IDCur,Curuser);
        Userpool.Copy(IDQue,Queuser);
        if (IDCur==IDQue || Curuser.Privilege>Queuser.Privilege)
            printf("%s %s %s %d\n",Queuser.Username,Queuser.Name,Queuser.MailAddr,Queuser.Privilege);
        else printf("-1\n");
    }    
    void modify_profile(std::string tmp[]){
        long long CurQ=pksHash1(tmp[2].c_str());
        std::pair<long long ,long long> QueQ=pksHash(tmp[4].c_str());
        auto CurIter=Useronline.Find(CurQ);
        if (CurIter.first==-1) {printf("-1\n");return;}
        int IDCur=CurIter.second;
        int IDQue=UserBPT.Find(QueQ);
        if (IDQue==-1) {printf("-1\n");return;}
        User Curuser,Queuser;
        Userpool.Copy(IDCur,Curuser);
        Userpool.Copy(IDQue,Queuser);
        if (Curuser.Privilege<=Queuser.Privilege && IDCur!=IDQue) {printf("-1\n");return;}
        for (int i=6;i<TMPLENGTH;i+=2)
        {
            if (tmp[i-1][1]=='p') strcpy(Queuser.Password,tmp[i].c_str());
            if (tmp[i-1][1]=='n') strcpy(Queuser.Name,tmp[i].c_str());
            if (tmp[i-1][1]=='m') strcpy(Queuser.MailAddr,tmp[i].c_str());
            if (tmp[i-1][1]=='g') 
            {
                Queuser.Privilege=StrToInt(tmp[i]);
                if (Queuser.Privilege>=Curuser.Privilege) {printf("-1\n");return;}
            }  
        }
        Userpool.Rewrite(IDQue,Queuser);    
        printf("%s %s %s %d\n",Queuser.Username,Queuser.Name,Queuser.MailAddr,Queuser.Privilege);
    }
    void add_train(std::string tmp[]){
        TrainInfo now;
        strcpy(now.TrainID,tmp[2].c_str());
        now.StationNum=atoi(tmp[4].c_str());
        now.Seatnumber=atoi(tmp[6].c_str());
        std::string Q[200];
        pksSplit(tmp[8],Q,'|');
        for (int i=0;i<now.StationNum;i++)
            strcpy(now.Station[i],Q[i].c_str());
        pksSplit(tmp[10],Q,'|');
        now.Pricepre[0]=0;
        for (int i=1;i<=now.StationNum-1;i++)
            now.Pricepre[i]=now.Pricepre[i-1]+StrToInt(Q[i-1]);
        now.Arrive[0]=StrToMin(tmp[12]);
        pksSplit(tmp[16],Q,'|');
        now.Stoptime[0]=0;
        now.Stoptime[now.StationNum-1]=0;
        for (int i=1;i<=now.StationNum-2;i++)
            now.Stoptime[i]=StrToInt(Q[i-1]);
        pksSplit(tmp[14],Q,'|');
        for (int i=1;i<now.StationNum;i++)
            now.Arrive[i]=now.Arrive[i-1]+(now.Stoptime[i-1]+StrToInt(Q[i-1]));
        
        pksSplit(tmp[18],Q,'|');
        now.Salestart=StrToDate(Q[0]);
        now.Saleending=StrToDate(Q[1]);
        now.Type=tmp[20][0];
        int CurID=Trainpool.Insert(now);
        std::pair<long long,long long> key(pksHash(now.TrainID));
        int flat=TrainBPT.Insert(key,CurID);
        if (flat==-1) {printf("-1\n");Trainpool.Erase(CurID);}
        else printf("0\n");
    }
    void delete_train(std::string tmp[]){
        std::pair<long long,long long> key(pksHash(tmp[2].c_str()));
        int flat=TrainBPT.Find(key);
        if (flat==-1) {printf("-1\n");return;}
        TrainInfo curTrain;
        Trainpool.Copy(flat,curTrain);
        if (curTrain.Release) {printf("-1\n");return;}
        Trainpool.Erase(flat);
        TrainBPT.Erase(key);
        printf("0\n");
    }
    void query_train(std::string tmp[]){
        std::pair<long long,long long> key(pksHash(tmp[4].c_str()));
        int flat=TrainBPT.Find(key);
        if (flat==-1){printf("-1\n");return;}
        TrainInfo curTrain;
        Trainpool.Copy(flat,curTrain);
        int Num=curTrain.StationNum;
        long long day=StrToDate(tmp[2]);
        if (day<curTrain.Salestart || day>curTrain.Saleending) {printf("-1\n");return;}
        if (curTrain.Release)
        {
            int SeatID=TrainSeatBPT.Find(std::make_pair(key.first,(long long)day));
            Seat TrainSeat;
            Seatpool.Copy(SeatID,TrainSeat);
            printf("%s %c\n",curTrain.TrainID,curTrain.Type);
            printf("%s xx-xx xx:xx -> ",curTrain.Station[0]);
            std::cout<<(curTrain.Arrive[0]+day*1440);
            printf(" 0 %d\n",TrainSeat.A[0]);
            for (int i=1;i<=curTrain.StationNum-2;i++)
            {
                printf("%s ",curTrain.Station[i]);
                std::cout<<(curTrain.Arrive[i]+day*1440);
                printf(" -> ");
                std::cout<<(curTrain.Arrive[i]+day*1440+curTrain.Stoptime[i]);
                printf(" %d %d\n",curTrain.Pricepre[i],TrainSeat.A[i]);
            }
            printf("%s ",curTrain.Station[Num-1]);
            std::cout<<(curTrain.Arrive[Num-1]+day*1440);
            printf(" -> xx-xx xx:xx ");
            printf("%d x\n",curTrain.Pricepre[Num-1]);
        }
        else
        {
            printf("%s %c\n",curTrain.TrainID,curTrain.Type);
            printf("%s xx-xx xx:xx -> ",curTrain.Station[0]);
            std::cout<<(curTrain.Arrive[0]+day*1440);
            printf(" 0 %d\n",curTrain.Seatnumber);
            for (int i=1;i<=curTrain.StationNum-2;i++)
            {
                printf("%s ",curTrain.Station[i]);
                std::cout<<(curTrain.Arrive[i]+day*1440);
                printf(" -> ");
                std::cout<<(curTrain.Arrive[i]+day*1440+curTrain.Stoptime[i]);
                printf(" %d %d\n",curTrain.Pricepre[i],curTrain.Seatnumber);
            }
            printf("%s ",curTrain.Station[Num-1]);
            std::cout<<(curTrain.Arrive[Num-1]+day*1440);
            printf(" -> xx-xx xx:xx ");
            printf("%d x\n",curTrain.Pricepre[Num-1]);
        
        }
    }
    void release_train(std::string tmp[]){
        std::pair<long long,long long> key(pksHash(tmp[2].c_str()));
        int flat=TrainBPT.Find(key);
        if (flat==-1) {printf("-1\n");return;}
        TrainInfo curTrain;
        Trainpool.Copy(flat,curTrain);
        if (curTrain.Release) {printf("-1\n");return;}
        curTrain.Release=1;
        Trainpool.Rewrite(flat,curTrain);
        long long key1=pksHash1(curTrain.TrainID);
        for (int i=0;i<curTrain.StationNum;i++)
            StationTrainBPT.Insert(std::make_pair(pksHash1(curTrain.Station[i]),key1),flat);
        Seat now;
        for (int i=0;i<curTrain.StationNum-1;i++) now.A[i]=curTrain.Seatnumber;
        for (int i=curTrain.StationNum-1;i<100;i++) now.A[i]=0;
        for (int i=curTrain.Salestart;i<=curTrain.Saleending;i++)
        {
            int x=Seatpool.Insert(now);
            TrainSeatBPT.Insert(std::make_pair(key.first,(long long)i),x);
        }
        printf("0\n");
    }
    static bool PriceCompare(const Ticket &X1,const Ticket &X2){
        if (X1.price!=X2.price) return X1.price<X2.price;
        return StringCompare(X1.TrainID,X2.TrainID)==-1;
    }
    static bool TimeCompare(const Ticket &X1,const Ticket &X2){
        if (X1.time!=X2.time) return X1.time<X2.time;
        return StringCompare(X1.TrainID,X2.TrainID)==-1;
    }
    Date ArriveTime(const TrainInfo & CurTrain,const int &X){
        return CurTrain.Arrive[X];
    }
    void query_ticket(std::string tmp[]){
        char Sname[31],Tname[31];
        strcpy(Sname,tmp[2].c_str());
        strcpy(Tname,tmp[4].c_str());
        long long SStationkey=pksHash1(Sname);
        int Sday=StrToDate(tmp[6]);
        sjtu::vector<int> TrainVec=StationTrainBPT.Findinterval(std::make_pair(SStationkey,-1),std::make_pair(SStationkey,1ll<<62));
        Ticket ans[5000];
        int ansnum=0;
        for (int i=0;i<(int)(TrainVec.size());i++)
        {          
            TrainInfo CurTrain;
            Trainpool.Copy(TrainVec[i],CurTrain);
            Ticket now;
            int Snum=-1;
            for (int j=0;j<CurTrain.StationNum;j++)
                if (xiangdeng(Sname,CurTrain.Station[j])==1)
                {
                    Snum=j;
                    break;
                }
            if (Snum==-1 || Snum==CurTrain.StationNum-1) continue;
            now.SDate=CurTrain.Arrive[Snum]+CurTrain.Stoptime[Snum];
            now.price=0;now.time=0;
            strcpy(now.Sname,Sname);
            strcpy(now.Tname,Tname);
            int Neday=Sday-now.SDate.Day();
            now.SDate=now.SDate+Neday*1440;
            if (Neday<CurTrain.Salestart || Neday>CurTrain.Saleending) continue;
            int Tnum=-1;
            for (int j=Snum+1;j<CurTrain.StationNum;j++)
                if (xiangdeng(Tname,CurTrain.Station[j])==1)
                {
                    Tnum=j;
                    break;
                }
            if (Tnum==-1) continue;
            now.Seatmax=1e9;
            int SeatID=TrainSeatBPT.Find(std::make_pair(pksHash1(CurTrain.TrainID),Neday));
            Seat TrainSeat;
            Seatpool.Copy(SeatID,TrainSeat);
            for (int j=Snum;j<Tnum;j++)
                now.Seatmax=min(now.Seatmax,TrainSeat.A[j]);
            strcpy(now.TrainID,CurTrain.TrainID);
            now.TDate=CurTrain.Arrive[Tnum]+Neday*1440;
            now.time=now.TDate-now.SDate;
            now.price=CurTrain.Pricepre[Tnum]-CurTrain.Pricepre[Snum];
            ans[ansnum++]=now;
        }
        int p[5000];
        printf("%d\n",ansnum);
        if (ansnum==0) return;
        for (int i=0;i<ansnum;i++) p[i]=i;
        if (TMPLENGTH==9 && tmp[8]=="cost")
        Qsort(0,ansnum-1,p,ans,PriceCompare);
        else Qsort(0,ansnum-1,p,ans,TimeCompare);
        for (int i=0;i<ansnum;i++)
            SC(ans[p[i]]);
    }
    const int inf=2e9;
    void query_transfer(std::string tmp[]){
        bool TimePriceflat=0;
        if (TMPLENGTH==9 && tmp[8]=="cost") TimePriceflat=1;
        std::pair<Ticket,Ticket> Bestans;
        int ansPrice=inf,ansTime1=inf,ansTimesum=inf;    
        char Sname[31],Tname[31];
        strcpy(Sname,tmp[2].c_str());
        strcpy(Tname,tmp[4].c_str());
        long long SStationkey=pksHash1(Sname);
        long long TStationkey=pksHash1(Tname);
        int Sday=StrToDate(tmp[6]);
        sjtu::vector<int> STrainVec=StationTrainBPT.Findinterval(std::make_pair(SStationkey,-1),std::make_pair(SStationkey,1ll<<62));
        sjtu::vector<int> TTrainVec=StationTrainBPT.Findinterval(std::make_pair(TStationkey,-1),std::make_pair(TStationkey,1ll<<62));
        TrainInfo TrainInfoTvec[1000];
        int Tvecsize=TTrainVec.size();
        if (Tvecsize>1000)
        {
            while (1) printf("666");
        }
        for (int i=0;i<Tvecsize;i++)
            Trainpool.Copy(TTrainVec[i],TrainInfoTvec[i]);
            
        
        /*sjtu::vector<TrainInfo> TrainInfoTvec;
        for (int i=0;i<(int)(TTrainVec.size());i++)
        {
            TrainInfo x;
            Trainpool.Copy(TTrainVec[i],x);
            TrainInfoTvec.push_back(x);
        }*/
        int ansTrain1=-1,ansTrain2=-1,ansS1=-1,ansS2=-1,ansT1=-1,ansT2=-1,ansDay1=-1,ansDay2=-1;
        for (int i=0;i<(int)(STrainVec.size());i++)
        {
            MPS.clean();
            TrainInfo CurTrain;
            Trainpool.Copy(STrainVec[i],CurTrain);
            int Snum=-1;
            for (int j=0;j<CurTrain.StationNum;j++)
                if (xiangdeng(Sname,CurTrain.Station[j])==1)
                {
                    Snum=j;
                    break;
                }
            if (Snum==-1 || Snum==CurTrain.StationNum-1) continue;
            Date NowDate;
            NowDate=CurTrain.Arrive[Snum]+CurTrain.Stoptime[Snum];
            int Neday=Sday-NowDate.Day();
            NowDate=NowDate+Neday*1440;
            if (Neday<CurTrain.Salestart || Neday>CurTrain.Saleending) continue;
            for (int j=Snum+1;j<CurTrain.StationNum;j++)
                MPS.Insert(std::make_pair(pksHash1(CurTrain.Station[j]),std::make_pair(j,CurTrain.Arrive[j]+Neday*1440)));
            for (int j=0;j<(int)(TTrainVec.size());j++)
            if (STrainVec[i]!=TTrainVec[j])
            {
                TrainInfo CurArriveTrain=TrainInfoTvec[j];
                int Tnum=-1;
                for (int k=0;k<CurArriveTrain.StationNum;k++)
                    if (xiangdeng(Tname,CurArriveTrain.Station[k])==1)
                    {
                        Tnum=k;break;
                    }
                if (Tnum==-1 || Tnum==0) continue;
                Date ArriveT=CurArriveTrain.Arrive[Tnum];
                for (int k=0;k<Tnum;k++)
                {
                    long long MidKey=pksHash1(CurArriveTrain.Station[k]);
                    auto Miditer=MPS.Find(MidKey);
                    if (Miditer.first==-1) continue;
                    Date FirstDate=Miditer.second.second;
                    int SecondDay=CurArriveTrain.Salestart;
                    Date ActualStartDay=CurArriveTrain.Arrive[k]+(CurArriveTrain.Stoptime[k]+SecondDay*1440);
                    while ((ActualStartDay-FirstDate)<0) {SecondDay++;ActualStartDay=ActualStartDay+1440;}
                    if (SecondDay>CurArriveTrain.Saleending) continue;
                    Date ActualArriveDay=ArriveT+SecondDay*1440;
                    int nowPrice=CurTrain.Pricepre[Miditer.second.first]-CurTrain.Pricepre[Snum]+CurArriveTrain.Pricepre[Tnum]-CurArriveTrain.Pricepre[k];
                    int nowTime1=FirstDate-NowDate;
                    int nowTimesum=ActualArriveDay-NowDate;
                    bool Chanflat=0;
                    if (TimePriceflat)
                        Chanflat=(ansPrice>nowPrice || (nowPrice==ansPrice && nowTime1<ansTime1));
                    else    Chanflat=(ansTimesum>nowTimesum || (nowTimesum==ansTimesum && nowTime1<ansTime1));
                    if (Chanflat)
                    {
                        ansTimesum=nowTimesum;ansTime1=nowTime1;ansPrice=nowPrice;
                        ansTrain1=i;ansS1=Snum;ansT1=Miditer.second.first;ansTrain2=j;ansS2=k;ansT2=Tnum;
                        ansDay1=Neday;                        ansDay2=SecondDay;
                    }
                    }   
                }
            }
        if (ansPrice<inf){
                TrainInfo CurTrain;
                Trainpool.Copy(STrainVec[ansTrain1],CurTrain);
                TrainInfo CurArriveTrain;
                Trainpool.Copy(TTrainVec[ansTrain2],CurArriveTrain);
                
                strcpy(Bestans.first.TrainID,CurTrain.TrainID);
                strcpy(Bestans.second.TrainID,CurArriveTrain.TrainID);
                Bestans.first.SDate=CurTrain.Arrive[ansS1]+(ansDay1*1440+CurTrain.Stoptime[ansS1]);Bestans.first.TDate=CurTrain.Arrive[ansT1]+(ansDay1*1440);
                Bestans.second.SDate=CurArriveTrain.Arrive[ansS2]+(ansDay2*1440+CurArriveTrain.Stoptime[ansS2]);Bestans.second.TDate=CurArriveTrain.Arrive[ansT2]+(ansDay2*1440);
                strcpy(Bestans.first.Sname,CurTrain.Station[ansS1]);
                strcpy(Bestans.first.Tname,CurTrain.Station[ansT1]);
                strcpy(Bestans.second.Sname,CurArriveTrain.Station[ansS2]);
                strcpy(Bestans.second.Tname,CurArriveTrain.Station[ansT2]);
                Bestans.first.price=CurTrain.Pricepre[ansT1]-CurTrain.Pricepre[ansS1];
                Bestans.second.price=CurArriveTrain.Pricepre[ansT2]-CurArriveTrain.Pricepre[ansS2];
                Bestans.first.Seatmax=inf;Bestans.second.Seatmax=inf;
                int SeatID=TrainSeatBPT.Find(std::make_pair(pksHash1(CurTrain.TrainID),(long long)ansDay1));
                Seat TrainSeat;
                Seatpool.Copy(SeatID,TrainSeat);
                for (int l=ansS1;l<ansT1;l++) Bestans.first.Seatmax=min(Bestans.first.Seatmax,TrainSeat.A[l]);
                SeatID=TrainSeatBPT.Find(std::make_pair(pksHash1(CurArriveTrain.TrainID),(long long)ansDay2));
                Seatpool.Copy(SeatID,TrainSeat);
                for (int l=ansS2;l<ansT2;l++) Bestans.second.Seatmax=min(Bestans.second.Seatmax,TrainSeat.A[l]);
                SC(Bestans.first);SC(Bestans.second);
            }
        else printf("0\n");
    }

    void buy_ticket(std::string tmp[]){
        long long Userkey=pksHash1(tmp[2].c_str());
        Order CurOrder;
        auto Curiter=Useronline.Find(Userkey);
        if (Curiter.first==-1) {printf("-1\n");return;} 
        std::pair<long long,long long> Trainkey=pksHash(tmp[4].c_str());
        CurOrder.TrainID=TrainBPT.Find(Trainkey);
        if (CurOrder.TrainID==-1) {printf("-1\n");return;}
        TrainInfo CurTrain;
        Trainpool.Copy(CurOrder.TrainID,CurTrain);
        strcpy(CurOrder.TrainName,CurTrain.TrainID);
        CurOrder.Num=StrToInt(tmp[8]);
        if (CurTrain.Release==0 || CurOrder.Num>CurTrain.Seatnumber) {printf("-1\n");return;}
        strcpy(CurOrder.SStationName,tmp[10].c_str());
        strcpy(CurOrder.TStationName,tmp[12].c_str());
        CurOrder.SStation=-1;
        for (int i=0;i<CurTrain.StationNum;i++)
        if (xiangdeng(CurOrder.SStationName,CurTrain.Station[i])==1)
        {
            CurOrder.SStation=i;
            break;
        }
        CurOrder.TStation=-1;
        for (int i=CurOrder.SStation+1;i<CurTrain.StationNum;i++)
        if (xiangdeng(CurOrder.TStationName,CurTrain.Station[i])==1)
        {
            CurOrder.TStation=i;
            break;
        }
        if ( CurOrder.SStation==-1 ||  CurOrder.TStation==-1)  {printf("-1\n");return;}
        Date Stime=CurTrain.Arrive[CurOrder.SStation]+CurTrain.Stoptime[CurOrder.SStation];        
        CurOrder.Day=StrToDate(tmp[6])-Stime.Day();
        if (CurOrder.Day>CurTrain.Saleending || CurOrder.Day<CurTrain.Salestart) {printf("-1\n");return;}
        CurOrder.Sdate=Stime+CurOrder.Day*1440;
        CurOrder.Tdate=CurTrain.Arrive[CurOrder.TStation]+CurOrder.Day*1440;
        CurOrder.Price=CurTrain.Pricepre[CurOrder.TStation]-CurTrain.Pricepre[CurOrder.SStation];
        bool flat=1;
        int SeatID=TrainSeatBPT.Find(std::make_pair(Trainkey.first,(long long)CurOrder.Day));
        Seat TrainSeat;
        Seatpool.Copy(SeatID,TrainSeat);
        for (int i=CurOrder.SStation;i<CurOrder.TStation;i++)
        {
            TrainSeat.A[i]-=CurOrder.Num;
            if (TrainSeat.A[i]<0) {flat=0;break;}
        }

        if (flat)
        {
            printf("%lld\n",1ll*CurOrder.Num*CurOrder.Price);
            CurOrder.type='S';
            CurOrder.time=++Ordertime;
            Seatpool.Rewrite(SeatID,TrainSeat);
            int ID=Orderpool.Insert(CurOrder);
            pksuseless=UserTimeBPT.Insert(std::make_pair(Userkey,Ordertime),ID);
        }
        else
        {
            if (TMPLENGTH==13 || tmp[14]=="false") {printf("-1\n");return;}
            CurOrder.type='P';
            CurOrder.time=++Ordertime;
            int ID=Orderpool.Insert(CurOrder);
            pksuseless=UserTimeBPT.Insert(std::make_pair(Userkey,Ordertime),ID);
            pksuseless=TrainTimeBPT.Insert(std::make_pair(Trainkey.first,CurOrder.Day*M+Ordertime),ID);  
            printf("queue\n");
        }
    }

    void query_order(std::string tmp[]){
        long long Userkey=pksHash1(tmp[2].c_str());
        auto Useriter=Useronline.Find(Userkey);
        if (Useriter.first==-1) {printf("-1\n");return;}
        sjtu::vector<int> Q=UserTimeBPT.Findinterval(std::make_pair(Userkey,-1),std::make_pair(Userkey,Ordertime+1));
        printf("%d\n",(int)Q.size());
        for (int i=(int)(Q.size())-1;i>=0;i--)
        {
            Order Curorder;
            Orderpool.Copy(Q[i],Curorder);
            if (Curorder.type=='P') printf("[pending]");
            if (Curorder.type=='S') printf("[success]");
            if (Curorder.type=='R') printf("[refunded]");
            printf(" %s %s ",Curorder.TrainName,Curorder.SStationName);
            std::cout<<Curorder.Sdate;
            printf(" -> %s ",Curorder.TStationName);
            std::cout<<Curorder.Tdate;
            printf(" %d %d\n",Curorder.Price,Curorder.Num);
        }
    }   

    void refund_ticket(std::string tmp[]){
        long long Userkey=pksHash1(tmp[2].c_str());
        auto Useriter=Useronline.Find(Userkey);
        if (Useriter.first==-1) {printf("-1\n");return;}
        sjtu::vector<int> Q=UserTimeBPT.Findinterval(std::make_pair(Userkey,-1),std::make_pair(Userkey,Ordertime+1));
        int x=1;
        if (TMPLENGTH==5) x=StrToInt(tmp[4]);
        x=(int)Q.size()-x;
        if (x<0 || x>=(int)Q.size()) {printf("-1\n");return;}
        Order Curorder;
        Orderpool.Copy(Q[x],Curorder);
        if (Curorder.type=='R') {printf("-1\n");return;}
        if (Curorder.type=='P') 
        {
            printf("0\n");
            Curorder.type='R';  
            Orderpool.Rewrite(Q[x],Curorder);
            pksuseless=TrainTimeBPT.Erase(std::make_pair(pksHash1(Curorder.TrainName),Curorder.Day*M+Curorder.time)); 
            return;
        }
        if (Curorder.type=='S') 
        {
            printf("0\n");
            Curorder.type='R';
            Orderpool.Rewrite(Q[x],Curorder); 
            long long Trainkey=pksHash1(Curorder.TrainName);
            int SeatID=TrainSeatBPT.Find(std::make_pair(Trainkey,(long long)Curorder.Day));
            Seat TrainSeat;
            Seatpool.Copy(SeatID,TrainSeat);
            for (int i=Curorder.SStation;i<Curorder.TStation;i++) 
                TrainSeat.A[i]+=Curorder.Num;
            sjtu::vector<int> Ordervec=TrainTimeBPT.Findinterval(std::make_pair(Trainkey,Curorder.Day*M-1),std::make_pair(Trainkey,Curorder.Day*M+Ordertime+1));
            for (int i=0;i<(int)(Ordervec.size());i++)
            {
                Order NowOrder;
                Orderpool.Copy(Ordervec[i],NowOrder);
                bool flat=1;
                for (int j=NowOrder.SStation;j<NowOrder.TStation;j++) 
                if (TrainSeat.A[j]<NowOrder.Num) {flat=0;break;}
                if (!flat) continue;
                for (int j=NowOrder.SStation;j<NowOrder.TStation;j++) 
                    TrainSeat.A[j]-=NowOrder.Num;
                NowOrder.type='S';
                pksuseless=TrainTimeBPT.Erase(std::make_pair(pksHash1(NowOrder.TrainName),NowOrder.Day*M+NowOrder.time));
                Orderpool.Rewrite(Ordervec[i],NowOrder);
            }
            Seatpool.Rewrite(SeatID,TrainSeat);
        }
    }
    void clean(){
        Userpool.clean();Trainpool.clean();Orderpool.clean();Seatpool.clean();
        UserBPT.clean();TrainBPT.clean();StationTrainBPT.clean();UserTimeBPT.clean();TrainTimeBPT.clean();TrainSeatBPT.clean();
        Ordertime=0;Usernumber=0;
    }
    std::map<std::string,long long> MP;
    void game(){
        // int last=clock();
        std::string S;
        while (1){
            // printf("%d   ",++timeID);
            getline(std::cin,S);
            if (S[S.length()-1]==13 || S[S.length()-1]==10) S = S.substr(0, S.length() -1);
            std::string tmp[50];
            TMPLENGTH=pksSplit(S,tmp,' ');         
            if (tmp[0]=="add_user"){add_user(tmp);} 
            if (tmp[0]=="login"){login(tmp);}
            if (tmp[0]=="logout"){logout(tmp);}
            if (tmp[0]=="query_profile"){query_profile(tmp);}
            if (tmp[0]=="modify_profile"){modify_profile(tmp);}
            if (tmp[0]=="add_train"){add_train(tmp);}
            if (tmp[0]=="release_train"){release_train(tmp);}
            if (tmp[0]=="query_train"){query_train(tmp);}
            if (tmp[0]=="delete_train"){delete_train(tmp);}
            if (tmp[0]=="query_ticket"){query_ticket(tmp);} 
            if (tmp[0]=="query_transfer"){query_transfer(tmp);}
            if (tmp[0]=="buy_ticket"){buy_ticket(tmp);}
            if (tmp[0]=="query_order"){query_order(tmp);}
            if (tmp[0]=="refund_ticket"){refund_ticket(tmp);}
            if (tmp[0]=="clean"){clean();}
            if (tmp[0]=="exit"){printf("bye\n");return;}
            // int now=clock();
            // MP[tmp[0]]=MP[tmp[0]]+now-last;
            // last=now;
            }
        }

};

#endif
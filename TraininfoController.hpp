#ifndef TRAININFOCONTROLLER_HPP
#define TRAININFOCONTROLLER_HPP

#include"include.hpp"

class TrainInfo{
    public:
    char TrainID[35];
    int StationNum;
    char Station[100][35];
    int Pricepre[100];
    int Seatnumber,Starttime,Traveltime[100],Stoptime[100],Salestart,Saleending;
    char Type;
    int Seat[95][100];
    bool Release=0;
    TrainInfo(){}
    ~TrainInfo(){}
};

//-i 2 -n 4 -m 6 -s 8 -p 10 -x 12 -t 14 -o 16 -d 18 -y 20
void add_train(std::vector<std::string> tmp){
    TrainInfo now;
    strcpy(now.TrainID,tmp[2]);
    now.StationNum=atoi(tmp[4].c_str());
    now.Seatnumber=atoi(tmp[6].c_str());
    std::vector<std::string> Q;
    pksSplit(tmp[8],Q,'|');
    for (int i=0;i<now.StationNum;i++)
        strcpy(now.Q[i],station.c_str());
    pksSplit(tmp[10],Q,'|');
    now.Pricepre[0]=0;
    for (int i=1;i<=now.StationNum-1;i++)
        now.Pricepre[i]=now.Pricepre[i-1]+StrToInt(Q[i-1]);
    now.Starttime=StrToMin(tmp[12]);
    pksSplit(tmp[16],Q,'|');
    now.Stoptime[0]=0;
    now.Stoptime[now.StationNum-1]=0;
    for (int i=1;i<=now.StationNum-2;i++)
        now.Stoptime[i]=StrToInt(Q[i-1]);
    pksSplit(tmp[14],Q,'|');
    for (int i=0;i<=now.Station-2;i++)
        now.Arrivetime[i]=StrToMin(Q[i]);
    pksSplit(tmp[18],Q,'|');
    now.Salestart=StrToDate(Q[0]);
    now.Saleending=StrToDate(Q[1]);
    for (int i=now.Salestart;i<=now.Saleending;i++)
        for (int j=0;j<now.StationNum-1;j++)
            now.Seat[i][j]=now.Seatnumber;
    now.Type=tmp[20][0];
    int CurID=Trainpool.insert(now);
    pair<long long,long long> key(pksHash(now.TrainID));
    int flat=TrainBPT.insert(make_pair(key,CurID));
    if (flat==-1) {printf("-1\n");Trainpool.erase(CurID);}
    else printf("0\n");
}
void delete_train(std::vector<std::string> tmp){
    std::pair<long long,long long> key(pksHash(tmp[2]));
    int flat=TrainBPT.erase(key);
    if (flat==-1) {printf("-1\n");return;}
    TrainInfo curTrain;
    Trainpool.copy(flat,curTrain)
    if (curTrain.Release) {printf("-1\n");return;}
    Trainpool.erase(flat);
    printf("0\n");
}

void query_train(std::vector<std::string> tmp){
    std::pair<long long,long long> key(pksHash(tmp[2]));
    int flat=find(key);
    if (flat==-1){printf("-1\n");return;}
    TrainInfo curTrain;
    Trainpool.copy(flat,curTrain);
    int Num=curTrain;
    int day=StrToDate(tmp[4]);
    if (day<curTrain.Salestart || day>curTrain.Saleending) {printf("-1\n");return;}
    Date Curtime(day,curTrain.Starttime);
    printf("%s xx-xx xx:xx -> ",curTrain.Station[0]);
    std::cout<<Curtime;
    printf(" 0 %d\n",curTrain.seat[day][0]);
    for (int i=1;i<=curTrain.StationNum-2;i++)
    {
        printf("%s ",curTrain.Station[i]);
        Curtime=Curtime+curTrain.Traveltime[i-1];
        std::cout<<Curtime;
        printf(" -> ");
        Curtime=Curtime+curTrain.Stoptime[i];
        std::cout<<Curtime;
        printf("%d %d\n",curTrain.Pricepre[i],curTrain.seat[day][i]);
    }
    printf("%s ",curTrain.Station[Num-1]);
    Curtime=Curtime+curTrain.Traveltime[Num-1];
    std::cout<<Curtime;
    printf(" -> xx-xx xx:xx ");
    printf("%d %d\n",curTrain.Pricepre[i],curTrain.seat[day][Num-1]);
}
void Release_train(std::vector<std::string> tmp){
    std::pair<long long,long long> key(pksHash(tmp[2]));
    int flat=TrainBPT.erase(key);
    if (flat==-1) {printf("-1\n");return;}
    TrainInfo curTrain;
    Trainpool.copy(flat,curTrain)
    if (curTrain.Release) {printf("-1\n");return;}
    curTrain.Release=1;
    long long key=pksHash1(curTrain.TrainID);
    for (int i=0;i<curTrain.stationNum;i++)
        StationTrainBPT.insert(make_pair(pksHash1(curTrain.Station[i]),key),flat);
    printf("0\n");
}



#endif
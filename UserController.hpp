#ifndef USERCONTROLLER_HPP
#define USERCONTROLLER_HPP

#include"include.hpp"
/*

class User{
    public:
    char Username[21],Password[31],Name[20],MailAddr[31];
    int Privilege;
    bool Online=0;
    int Ordernumber=0;
    int Lastorder=-1;
};
*/
#define ll long long
class UserController{
    public:
    int Usernumber;
    StoragePool<User> *Store_file;
    BPT<pair<ll,ll> -> int> TREE;//不可重复元素
    UserController()
    void add_user();
    void login();
    void logout();
    void query_profile();
    void modify_profile();   
};
//pair<long long,long long>
void UserController::add_user(vector<std::string> tmp){
    User tem;
    std::string Curuser;
    Curuser=tmp[2];
    std::pair<long long,long long> Q=pksHash(Curuser.c_str());
    int IDcur=TREE.Find(Q);
    strcpy(tem.Username,tmp[4].c_str());
    strcpy(tem.Password,tmp[6].c_str());
    strcpy(tem.Name,tmp[8].c_str());
    strcpy(tem.MailAddr,tmp[10].c_str());
    tem.Privilege=atoi(tmp[12].c_str());
    
    bool flat=1;
    if (IDcur==-1) flat=0;
    if (flat)
    {
        User CurUser;
        Store_file.copy(IDcur,CurUser);
        if (CurUser.Online==0 || CurUser.Privilege<=tem.Privilege) flat=0;
    }
    if (Usernumber==0) {flat=1; tem.Privilege=10;}
    if (flat)   
    {
        bool Suc=TREE.insert(tem);
        if (!Suc) printf("-1\n");
        else printf("0\n");
    }
    else printf("-1\n");    
}
void UserController::login(std::vector<std::string> tmp){
    std::pair<long long,long long> Q=pksHash(tmp[2].c_str());
    int IDcur=TREE.Find(Q);
    if (IDcur==-1) {printf("-1\n");return;}
    User Curuser;
    Store_file.Copy(IDcur,Curuser);
    if (Curuser.Online==1 || xiangdeng(tmp[4].c_str(),Curuser.Password)==0) printf("-1\n");
    else{
        Curuser.Online=1;
        Store_file.Rewrite(IDcur,Curuser);
        printf("0\n");
    }
}
void UserController::logout(std::vector<std::string> tmp){
    std::pair<long long,long long> Q=pksHash(tmp[2].c_str());
    int IDcur=TREE.Find(Q);
    if (IDcur==-1) {printf("-1\n");return;}
    User Curuser;
    Store_file.Copy(IDcur,Curuser);
    if (Curuser.Online==0 || xiangdeng(tmp[4].c_str(),Curuser.Password)==0) printf("-1\n");
    else{
        Curuser.Online=0;
        Store_file.Rewrite(IDcur,Curuser);
        printf("0\n");
    }
}

void UserController::query_profile(std::vector<std::string> tmp){
    std::pair<long long ,long long> CurQ=pksHash(Curusername.c_str());
    std::pair<long long ,long long> QueQ=pksHash(Queryusername.c_str());
    bool flat=1;
    int IDCur=TREE.Find(CurQ);
    if (IDcur==-1) {printf("-1\n");return;}
    int IDQue=TREE.Find(QueQ);
    if (IDQue==-1) {printf("-1\n");return;}
    User Curuser,Queuser;
    Store_file(IDCur,Curuser);
    Store_file(IDQue,Queuser);
    if (Curuser.Privilege>Queuser.Privilege)
        printf("%s %s %s %d\n",Queuser.Username,Queuser.Name,Queuser.MailAddr,Queuser.Privilege);
    else printf("-1\n");
}
    
void UserController::modify_profile(std::vector<std::string> tmp){
    std::pair<long long ,long long> CurQ=pksHash(tmp[2].c_str());
    std::pair<long long ,long long> QueQ=pksHash(tmp[4].c_str());
    bool flat=1;
    int IDCur=TREE.Find(CurQ);
    if (IDcur==-1) flat=0;
    int IDQue=TREE.Find(QueQ);
    if (IDQue==-1) flat=0;
    User Curuser,Queuser;
    Store_file(IDCur,Curuser);
    Store_file(IDQue,Queuser);
    if (Curuser.Privilege<=Queuser.Privilege && IDCur!=IDQue) flat=0;
    for (int i=6;i<tmp.size();i+=2)
    {
        if (tmp[i-1][1]=='p') strcpy(Queuser.Password,tmp[i].c_str());
        if (tmp[i-1][1]=='n') strcpy(Queuser.Username,tmp[i].c_str());
        if (tmp[i-1][1]=='m') strcpy(Queuser.MailAddr,tmp[i].c_str());
        if (tmp[i-1][1]=='g') 
        {
            Queuser.Privilege=atoi(tmp[i].c_str());
            if (Queuser.Privilege>Curuser.Privilege) flat=0;
        }                
    }
    if (flat)
    {
        Store_file(IDQue,Queuser);
        printf("%s %s %s %d\n",Queuser.Username,Queuser.Name,Queuser.MailAddr,Queuser.Privilege);
    }
    else printf("-1\n");
}
#endif
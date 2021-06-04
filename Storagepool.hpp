#ifndef STORAGEPOOL_HPP
#define STORAGEPOOL_HPP
#include"include.hpp"
extern int sumCheck;
int CI=0;

template<class T>
class QQHash{// long long->T
    public:
    class Zhizhen{
        public:
        std::pair<long long,T> value;
        Zhizhen *nex;
        Zhizhen():nex(nullptr){}
        Zhizhen(const std::pair<long long,T> &X,Zhizhen *Y):value(X),nex(Y){}
    };
    int Len;
    Zhizhen **ARR;
    T Rubish;
    QQHash(const int &len):Len(len){
        ARR=new Zhizhen*[Len];
        for (int i=0;i<len;i++) ARR[i]=nullptr;
    }
    std::pair<long long,T> Find(const long long &X){
        int ID=X%Len;
        for (auto iter=ARR[ID];iter!=nullptr;iter=iter->nex)
        if ((iter->value).first==X)
            return iter->value;
        return std::make_pair(-1,Rubish);      
    }
    void Erase(const long long &X){
        int ID=X%Len;
        if (ARR[ID]==nullptr) return;
        if ((ARR[ID]->value).first==X)
        {
            Zhizhen *last=ARR[ID];
            ARR[ID]=ARR[ID]->nex;
            delete last;
            return;
        }

        for (auto Iter=ARR[ID];Iter->nex!=nullptr;Iter++)
        {
            if (((Iter->nex->value).first)==X)
            {
                Zhizhen *last=Iter->nex;
                Iter->nex=last->nex;
                delete last;
                return;
            }
        }
    }
    void Insert(const std::pair<long long ,T> &X){
        int ID=X.first%Len;    
        Zhizhen *Iter=new Zhizhen(X,ARR[ID]);
        ARR[ID]=Iter;
    }
    void clean(){
        for (int i=0;i<Len;i++)
        while (ARR[i]!=nullptr)
        {
            Zhizhen *last=ARR[i];
            ARR[i]=last->nex;
            delete last;
        }
    }
    ~QQHash(){
        clean();
        delete []ARR;
    }
};


template <typename T>
class StoragePool{
    public:
    int Indoor;
    std::fstream info;
    std::string File; 
    int cnt=-1,FirstFail=-1,BPTint=-1;
    int Offset,Blsize;
    int *index;
    T *Storage;
    bool *Boo;
    QQHash<int> MP;
    int NowDele;
    StoragePool(const std::string &X,const int &len);
    int Insert(const T &X);
    void Erase(const int &id);
    void Copy(const int &id,T &X);
    void InsideRewrite(const int &id,const T &X);
    void Rewrite(const int &id,const T &X);
    int Readint();
    void Rewriteint(const int &x);
    void clean();
    ~StoragePool();
};

template<typename T>
StoragePool<T>::StoragePool(const std::string &X,const int &Len):File(X),MP(Len){
    Indoor=Len;
    index=new int[Indoor];
    Boo=new bool[Indoor];
    Storage=new T[Indoor];
    info.open(X,std::ios::in | std::ios::out | std::ios::binary);
    if (info.fail())
    {
        info.clear();
        std::ofstream fout(X);
        fout.close();
        info.open(X,std::ios::in | std::ios::out | std::ios::binary);
        info.seekp(0);
        info.write(reinterpret_cast<char *> (&cnt),sizeof(int));
        info.write(reinterpret_cast<char *> (&FirstFail),sizeof(int));  
        info.write(reinterpret_cast<char *> (&BPTint),sizeof(int));  
    }     
    info.seekp(0);
    info.read(reinterpret_cast<char *> (&cnt),sizeof(int));
    info.read(reinterpret_cast<char *> (&FirstFail),sizeof(int));  
    info.read(reinterpret_cast<char *> (&BPTint),sizeof(int));  
    Offset=3*sizeof(int);
    Blsize=sizeof(T)+sizeof(int);   
    for (int i=0;i<Indoor;i++) index[i]=-1;
    NowDele=0;   
}
template<typename T>
int StoragePool<T>::Insert(const T &X){
    if (FirstFail==-1)
    {
        cnt++;
        info.seekp(Offset+cnt*Blsize);
        info.write(reinterpret_cast<const char *> (&X),sizeof(T));
        int tmp=-1;
        info.write(reinterpret_cast<char *> (&tmp),sizeof(int));
        return cnt;
    }
    else{
        info.seekp(Offset+FirstFail*Blsize);
        info.write(reinterpret_cast<const char *> (&X),sizeof(T));   
        int nex;
        info.read(reinterpret_cast<char *> (&nex),sizeof(int));
        int last=FirstFail;
        FirstFail=nex;
        return last;
    }
}
template<typename T> 
void StoragePool<T>::Erase(const int &id){
    std::pair<long long,int> Q=MP.Find((long long)id);
    if (Q.first!=-1)
    {
        index[Q.second]=-1;
        MP.Erase(Q.first);
    }
    info.seekp(Offset+id*Blsize+sizeof(T));
    info.write(reinterpret_cast<const char *> (&FirstFail),sizeof(int));
    FirstFail=id;
}
template<typename T> 
void StoragePool<T>::InsideRewrite(const int &id,const T &X)
{
    // int last=clock();
    CI++;
    info.seekp(Offset+id*Blsize);
    info.write(reinterpret_cast<const char *> (&X),sizeof(T));
    // sumCheck+=clock()-last;
} 
template<typename T> 
void StoragePool<T>::Copy(const int &id,T &X){
    std::pair<long long,int> Q=MP.Find((long long)id);
    if (Q.first!=-1)
    {
        X=Storage[Q.second];
        return;
    }
    if (index[NowDele]!=-1)
    {
        if (Boo[NowDele])
            InsideRewrite(index[NowDele],Storage[NowDele]);

        MP.Erase((long long)(index[NowDele]));
    }
    Boo[NowDele]=0;
    index[NowDele]=id;
    info.seekp(Offset+id*Blsize);
    info.read(reinterpret_cast<char *> (&Storage[NowDele]),sizeof(T));
    X=Storage[NowDele];
    MP.Insert(std::make_pair((long long)(index[NowDele]),NowDele));
    NowDele=(NowDele+1)%Indoor;
}

template<typename T> 
void StoragePool<T>::Rewrite(const int &id,const T &X)
{
    std::pair<long long,int> Q=MP.Find((long long)id);
    if (Q.first!=-1)
    {
        Storage[Q.second]=X;
        Boo[Q.second]=1;
        return;
    }
    if (index[NowDele]!=-1)
    {
        if (Boo[NowDele])
        InsideRewrite(index[NowDele],Storage[NowDele]);
        MP.Erase((long long)(index[NowDele]));
    }
    Boo[NowDele]=1;
    index[NowDele]=id;
    Storage[NowDele]=X;
    MP.Insert(std::make_pair((long long)(index[NowDele]),NowDele));
    NowDele=(NowDele+1)%Indoor;
} 
template<typename T>
int StoragePool<T>::Readint(){
    return BPTint;
}

template<typename T>
void StoragePool<T>::Rewriteint(const int &x){
    BPTint=x;  
}

template<typename T>
void StoragePool<T>::clean(){
    info.close();
    info.open(File,std::ios::in | std::ios::out | std::ios::binary | std::ios::trunc);
    cnt=-1;
    FirstFail=-1;
    BPTint=-1;
    for (int i=0;i<Indoor;i++) index[i]=-1;
    info.write(reinterpret_cast<char *> (&cnt),sizeof(int));
    info.write(reinterpret_cast<char *> (&FirstFail),sizeof(int));  
    info.write(reinterpret_cast<char *> (&BPTint),sizeof(int));  
}

template<typename T>
StoragePool<T>::~StoragePool(){
    for (int i=0;i<Indoor;i++)
    if (index[i]!=-1 && Boo[i])
        InsideRewrite(index[i],Storage[i]);
    delete []index;
    delete []Boo;
    delete []Storage;
    info.seekp(0);
    info.write(reinterpret_cast<char *> (&cnt),sizeof(int));
    info.write(reinterpret_cast<char *> (&FirstFail),sizeof(int));  
    info.write(reinterpret_cast<char *> (&BPTint),sizeof(int)); 
    info.close();
    // std::cerr<<CI<<std::endl;
}






template <typename T>
class StoragePoolPlus{
    public:
    int Indoor;
    std::fstream info;
    std::string File; 
    int cnt=-1;
    int Offset,Blsize;
    int *index;
    T *Storage;
    bool *Boo;
    QQHash<int> MP;
    int NowDele;
    StoragePoolPlus(const std::string &X,const int &len);
    int Insert(const T &X);
    void Erase(const int &id);
    void Copy(const int &id,T &X);
    void InsideRewrite(const int &id,const T &X);
    void Rewrite(const int &id,const T &X);
    int Readint();
    void Rewriteint(const int &x);
    void clean();
    ~StoragePoolPlus();
};

template<typename T>
StoragePoolPlus<T>::StoragePoolPlus(const std::string &X,const int &Len):File(X),MP(Len){
    Indoor=Len;
    index=new int[Indoor];
    Boo=new bool[Indoor];
    Storage=new T[Indoor];
    info.open(X,std::ios::in | std::ios::out | std::ios::binary);
    if (info.fail())
    {
        info.clear();
        std::ofstream fout(X);
        fout.close();
        info.open(X,std::ios::in | std::ios::out | std::ios::binary);
        info.seekp(0);
        info.write(reinterpret_cast<char *> (&cnt),sizeof(int));
    }     
    info.seekp(0);
    info.read(reinterpret_cast<char *> (&cnt),sizeof(int));
    Offset=sizeof(int);
    Blsize=sizeof(T);   
    for (int i=0;i<Indoor;i++) index[i]=-1;
    NowDele=0;   
}
template<typename T>
int StoragePoolPlus<T>::Insert(const T &X){
    cnt++;
    info.seekp(Offset+cnt*Blsize);
    info.write(reinterpret_cast<const char *> (&X),sizeof(T));
    return cnt;
}
template<typename T> 
void StoragePoolPlus<T>::InsideRewrite(const int &id,const T &X)
{
    // int last=clock();
    info.seekp(Offset+id*Blsize);
    info.write(reinterpret_cast<const char *> (&X),sizeof(T));
    // sumCheck+=clock()-last;
} 
template<typename T> 
void StoragePoolPlus<T>::Copy(const int &id,T &X){
    std::pair<long long,int> Q=MP.Find((long long)id);
    if (Q.first!=-1)
    {
        X=Storage[Q.second];
        return;
    }
    if (index[NowDele]!=-1)
    {
        if (Boo[NowDele])
            InsideRewrite(index[NowDele],Storage[NowDele]);
        MP.Erase((long long)(index[NowDele]));
    }
    Boo[NowDele]=0;
    index[NowDele]=id;
    info.seekp(Offset+id*Blsize);
    info.read(reinterpret_cast<char *> (&Storage[NowDele]),sizeof(T));
    X=Storage[NowDele];
    MP.Insert(std::make_pair((long long)(index[NowDele]),NowDele));
    NowDele=(NowDele+1)%Indoor;
}

template<typename T> 
void StoragePoolPlus<T>::Rewrite(const int &id,const T &X)
{
    std::pair<long long,int> Q=MP.Find((long long)id);
    if (Q.first!=-1)
    {
        Storage[Q.second]=X;
        Boo[Q.second]=1;
        return;
    }
    if (index[NowDele]!=-1)
    {
        if (Boo[NowDele])
        InsideRewrite(index[NowDele],Storage[NowDele]);
        MP.Erase((long long)(index[NowDele]));
    }
    Boo[NowDele]=1;
    index[NowDele]=id;
    Storage[NowDele]=X;
    MP.Insert(std::make_pair((long long)(index[NowDele]),NowDele));
    NowDele=(NowDele+1)%Indoor;
} 
template<typename T>
int StoragePoolPlus<T>::Readint(){
    info.seekp(2*sizeof(int));
    int x;
    info.read(reinterpret_cast<char *>(&x),sizeof(int));
    return x;
}

template<typename T>
void StoragePoolPlus<T>::Rewriteint(const int &x){
    info.seekp(2*sizeof(int));
    info.write(reinterpret_cast<char *>(&x),sizeof(int));    
}

template<typename T>
void StoragePoolPlus<T>::clean(){
    info.close();
    info.open(File,std::ios::in | std::ios::out | std::ios::binary | std::ios::trunc);
    cnt=-1;
    for (int i=0;i<Indoor;i++) index[i]=-1;
    info.seekp(0);
    info.write(reinterpret_cast<char *> (&cnt),sizeof(int)); 
}

template<typename T>
StoragePoolPlus<T>::~StoragePoolPlus(){
    for (int i=0;i<Indoor;i++)
    if (index[i]!=-1 && Boo[i])
        InsideRewrite(index[i],Storage[i]);
    delete []index;
    delete []Boo;
    delete []Storage;
    info.seekp(0);
    info.write(reinterpret_cast<char *> (&cnt),sizeof(int));
    info.close();
}

#endif
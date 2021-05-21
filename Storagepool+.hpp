#ifndef STORAGEPOOLPlus_HPP
#define STORAGEPOOLPlus_HPP
#include"include.hpp"
const int Indoor=32;
template <typename T>
class StoragePool{
    public:
    std::fstream info;
    std::string File; 
    int cnt=-1,FirstFail=-1,BPTint=-1;
    int Offset,Blsize;
    int index[Indoor];
    T Storage[Indoor];
    int x;
    StoragePool(const std::string &X);
    int Insert(const T &X);
    void Erase(const int &id);
    void Copy(const int &id,T &X);
    void Rewrite(const int &id,const T &X);
    int Readint();
    void Rewriteint(const int &x);
    void clean();
    ~StoragePool();
};

template<typename T>
StoragePool<T>::StoragePool(const std::string &X):File(X){
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
    info.seekp(Offset+id*Blsize+sizeof(T));
    info.write(reinterpret_cast<const char *> (&FirstFail),sizeof(int));
    FirstFail=id;
}
template<typename T> 
void StoragePool<T>::Copy(const int &id,T &X){
    info.seekp(Offset+id*Blsize);
    info.read(reinterpret_cast<char *> (&X),sizeof(T));
}
template<typename T> 
void StoragePool<T>::Rewrite(const int &id,const T &X)
{
    info.seekp(Offset+id*Blsize);
    info.write(reinterpret_cast<const char *> (&X),sizeof(T));
} 

template<typename T>
int StoragePool<T>::Readint(){
    info.seekp(2*sizeof(int));
    int x;
    info.read(reinterpret_cast<char *>(&x),sizeof(int));
    return x;
}

template<typename T>
void StoragePool<T>::Rewriteint(const int &x){
    info.seekp(2*sizeof(int));
    info.write(reinterpret_cast<char *>(&x),sizeof(int));    
}

template<typename T>
void StoragePool<T>::clean(){
    info.close();
    info.open(File,std::ios::in | std::ios::out | std::ios::binary | std::ios::trunc);
    cnt=-1;
    FirstFail=-1;
    BPTint=-1;
    info.write(reinterpret_cast<char *> (&cnt),sizeof(int));
    info.write(reinterpret_cast<char *> (&FirstFail),sizeof(int));  
    info.write(reinterpret_cast<char *> (&BPTint),sizeof(int));  
}

template<typename T>
StoragePool<T>::~StoragePool(){
    info.seekp(0);
    info.write(reinterpret_cast<char *> (&cnt),sizeof(int));
    info.write(reinterpret_cast<char *> (&FirstFail),sizeof(int));  
    info.write(reinterpret_cast<char *> (&BPTint),sizeof(int)); 
    info.close();
}


#endif
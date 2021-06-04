#ifndef MAP_HPP
#define MAP_HPP

int size;
int *nowans;
std::pair<long long,long long> nowKeyleft,nowKeyright;
std::pair<std::pair<long long,long long>,int> *arr;
class pksBPT{
    public:
    class node{
        public:
        node *son[2];
        std::pair<long long,long long> key;
        int value;     
        node(node *iter0,node *iter1,const std::pair<long long,long long> &Key,const int &Value):key(Key),value(Value){
            son[0]=iter0;son[1]=iter1;
        }
    };
    std::string File;
    int num=0;
    node *root=nullptr;
    std::fstream info;
    void dfsinsert(const int &l,const int &r){
        if (l>r) return;
        int mid=(l+r)/2;
        int x=Insert(arr[mid].first,arr[mid].second);
        if(x==-1) exit(0);
        dfsinsert(l,mid-1);
        dfsinsert(mid+1,r);
    }
    pksBPT(const std::string &X){
            info.open(X,std::ios::in | std::ios::out | std::ios::binary);
            if (info.fail())
            {
                info.clear();
                std::ofstream fout(X);
                fout.close();
                info.open(X,std::ios::in | std::ios::out | std::ios::binary);
                info.seekp(0);
                info.write(reinterpret_cast<char *> (&num),sizeof(int)); 
            }     
            int x;
            info.seekp(0);
            info.read(reinterpret_cast<char *> (&x),sizeof(int));
            arr=new std::pair<std::pair<long long,long long>,int>[x];
            for (int i=0;i<x;i++)
            {
                info.read(reinterpret_cast<char *> (&arr[i].first),sizeof(std::pair<long long,long long>));
                info.read(reinterpret_cast<char *> (&arr[i].second),sizeof(int));
            }
            dfsinsert(0,x-1);
            delete []arr;
    }
    void dfs(node *now,const std::pair<long long,long long> &nowl,const std::pair<long long,long long> &nowr){
        if (nowl>nowKeyright || nowKeyleft>nowr) return;
        if (now->son[0]!=nullptr)
            dfs(now->son[0],nowl,min(nowr,now->key));
        if (now->value!=-1 && now->key<=nowKeyright && now->key>=nowKeyleft) nowans[size++]=now->value;
        if (now->son[1]!=nullptr)
            dfs(now->son[1],max(nowl,now->key),nowr);        
    }
    int Insert(const std::pair<long long,long long> &Key,const int &Value)
    {
        // std::cerr<<"insert"<<Key.first<<' '<<Key.second<<' '<<Value<<std::endl;
        if (root==nullptr)
        {
            root=new node(nullptr,nullptr,Key,Value);
            num++;
            return 0;
        }   
        node *pks=root;
        if (pks->key==Key)
        {
            if (pks->value==-1)pks->value=Value;
            else return -1;
            num++;
            return 0;
        }
        int Dir=(Key>pks->key);
        while (pks->son[Dir]!=nullptr)
        {
            pks=pks->son[Dir];
            if (pks->key==Key)
            {
                if (pks->value==-1)pks->value=Value;
                else return -1;
                num++;
                return 0;
            }
            Dir=(Key>pks->key);
        }
        pks->son[Dir]=new node(nullptr,nullptr,Key,Value);
        num++;
        return 0;
    }
    int Erase(const std::pair<long long,long long> &Key){
        // std::cerr<<"Erase"<<Key.first<<' '<<Key.second<<std::endl;
        node *now=root;
        while (now!=nullptr && now->key!=Key)
            now=now->son[Key>now->key];
        if (now==nullptr || now->value==-1) return -1;
        int x=now->value;
        now->value=-1;
        num--;
        return x;
    }
    int Find(const std::pair<long long,long long> &Key){
        node *now=root;
        while (now!=nullptr && now->key!=Key)
            now=now->son[Key>now->key];
        if (now==nullptr) return -1;
        return now->value;
    }
    int Findinterval(const std::pair<long long,long long> &Keyleft,const std::pair<long long,long long> &Keyright,int *ans){
        size=0;
        nowKeyleft=Keyleft;
        nowKeyright=Keyright;
        nowans=ans;
        if (root!=nullptr)
        dfs(root,std::pair<long long,long long>(-1000000,-10000000),std::pair<long long ,long long>(8e18,8e18));
        // std::cerr<<"size"<<size<<std::endl;
        return size;
    }
    void dfswrite(node *now){
        if (now->son[0]!=nullptr) dfswrite(now->son[0]);
        if (now->value!=-1)
        {
            info.write(reinterpret_cast<const char *> (&now->key),sizeof(now->key));
            info.write(reinterpret_cast<const char *> (&now->value),sizeof(now->value));
        }
        if (now->son[1]!=nullptr) dfswrite(now->son[1]);       
    }
    void qing(node *X){
        if (X->son[0]!=nullptr) {qing(X->son[0]);delete X->son[0];}
        if (X->son[1]!=nullptr) {qing(X->son[1]);delete X->son[1];}        
    }
    void clean(){
        if(root!=nullptr){
            qing(root);
            delete root;
            root=nullptr;
        }
        num=0;
    }
    ~pksBPT(){
        info.seekp(0);
        info.write(reinterpret_cast<const char *> (&num),sizeof(num));
        if (root!=nullptr)
        {
            dfswrite(root);
            qing(root);
            delete root;
            root=nullptr;
        }
        info.close();
    }
};



#endif
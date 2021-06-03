#ifndef BPT_HPP
#define BPT_HPP
#include"include.hpp"

class BPT{
    public:
        std::map<std::pair<long long,long long> ,int> TREE;
        int num=0;
        std::string File;
        // BPT(){TREE.clear();}
        BPT(const std::string &X):File(X){
            TREE.clear();
            std::fstream info;
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
            for (int i=1;i<=x;i++)
            {
                std::pair<long long,long long> key;
                int value;
                info.read(reinterpret_cast<char *> (&key),sizeof(key));
                info.read(reinterpret_cast<char *> (&value),sizeof(value));
                Insert(key,value);
            }
            info.close();
        }
        int Insert(const std::pair<long long,long long> &key,const int &ID){
            if (TREE.find(key)!=TREE.end()) return -1;
            num++;
            TREE[key]=ID;return 0;        }
        int Erase(const std::pair<long long,long long> &key){
            auto iter=TREE.find(key);
            if (iter==TREE.end()) return -1;
            int x=iter->second;
            num--;
            TREE.erase(iter);return x;
        }
        int Find(const std::pair<long long,long long> &key){
            auto iter=TREE.find(key);
            if (iter==TREE.end()) return -1;
            return iter->second;
        }
        sjtu::vector<int> Findinterval(const std::pair<long long,long long> &keyleft,const std::pair<long long,long long> &keyright){
            auto iter=TREE.lower_bound(keyleft);
            sjtu::vector<int> ans;
            while (iter!=TREE.end() && iter->first<=keyright)  
            {
                ans.push_back(iter->second);
                iter++;
            }
            return ans;
        }
        int Findinterval(const std::pair<long long,long long> &keyleft,const std::pair<long long,long long> &keyright,int ans[]){
            int len=0;
            auto iter=TREE.lower_bound(keyleft);
            while (iter!=TREE.end() && iter->first<=keyright)  
            {
                ans[len++]=iter->second;
                iter++;
            }
            return len;
        }
        void clean(){
            TREE.clear();
            num=0;
        }
        ~BPT(){
            std::fstream Info;
            Info.open(File,std::ios::in | std::ios::out | std::ios::binary);
            Info.write(reinterpret_cast<char *> (&num),sizeof(num));
            for (auto iter=TREE.begin();iter!=TREE.end();iter++)
            {
                Info.write(reinterpret_cast<const char *> (&iter->first),sizeof(iter->first));
                Info.write(reinterpret_cast<const char *> (&iter->second),sizeof(iter->second));
            }
        }
};

#endif
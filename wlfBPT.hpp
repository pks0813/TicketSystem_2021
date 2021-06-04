//
// Created by Snoopy on 2021/5/19.
//
// 如果插入重复元素要不要报错？
// 好家伙，到时候可得查一遍有没有改完忘了写到文件里的

#ifndef WLFBPT_HPP
#define WLFBPT_HPP

#include <iostream>
#include "Storagepool.hpp"
const int Huancun=800;
/*
 * max_size此B+树的度数
 * 除根结点外所有节点的孩子数量k满足 (max_size + 1) / 2 <= k <= max_size
 * 有k个孩子的结点保存了k-1个关键字引导查找，关键字k代表了子树k+1中关键字的最小值
 * 叶节点的孩子指针指向数据块的地址(int)
 * 每个数据块的记录数量[(max_size + 1) / 2, max_size]
 * 所有叶节点顺序连成一个单链表
 */
const int max_size = 200, min_size = (max_size + 1) / 2;

class BPT {
private:
    using Key = std::pair<long long, long long>;
    struct bpt_node{
        int length, next, type; //type: 0下一层是索引节点，1下一层是数据结点
        Key key[max_size + 5];
        int value[max_size + 5] = {0};

        bpt_node() :length(0), next(-1), type(-1){}

        ///注意,以下函数仅仅修改内存里(*this)对象,并没有修改外存文件上的信息!
        /*
         * 在type_1结点里插入(_key, value), 插入失败-1, 插入成功0
         * todo 2021/5/31 检查合格
         */
        int data_insert(const Key &_key, int _value) {
            int hole, i;
            //找到插入的位置
            for (i = 1; i <= length; ++i) {
                if (_key == key[i])  return -1;
                else {
                    if (_key > key[i]) continue;
                    else {hole = i; break;}
                }
            }
            if (i == length + 1) hole = length + 1;
            //实施插入
            for (i = length; i >= hole; --i) {
                key[i + 1] = key[i];
                value[i + 1] = value[i];
            }
            key[hole] = _key;
            value[hole] = _value;
            ++length;
            return 0;
        }
        /*
         * 在type_0结点里, 插入value[offset]指向的子树所分裂出的, 位于id的新结点.
         * todo 2021/5/31 检查合格
         */
        void index_insert(int offset, int id, StoragePool<bpt_node> &storage){
            bpt_node new_node;
            storage.Copy(id, new_node);
            for (int i = length - 1; i > offset; --i) {
                key[i + 1] = key[i];
                value[i + 1]  = value[i];
            }
            if (new_node.type == 0) key[offset + 1] = new_node.key[0];
            else key[offset + 1] = new_node.key[1];
            value[offset + 1] = id;
            ++length;
        }
        /*
         * 在type_1结点里删除关键字为_key的元素, 删除失败则返回-1,成功则返回value
         * todo 2021/5/31 检查合格
         */
        int data_erase(const Key &_key) {
            int hole = data_locate(_key);
            if (hole == -1) return -1;
            else {
                int erased_value = value[hole];
                for (int i = hole + 1; i <= length; ++i) {
                    key[i - 1] = key[i];
                    value[i - 1] = value[i];
                }
                --length;
                return erased_value;
            }
        }
        /*
         * type_0结点的子节点是type_0结点, 且value[sub]叶节点的元素个数为min_size,
         * 如果sub < length, 如果value[sub + 1]元素个数 > min_size就搬过来一个元素,
         * 否则value[sub]与value[sub + 1]直接合并成一棵子树(注意维护next)
         * 如果sub == length, 考虑value[sub - 1], 与上同
         *
         * type_0结点的子节点是type_1结点, 且value[sub]叶节点的元素个数为min_size - 1,
         * 如果sub < length, 如果value[sub + 1]元素个数 > min_size就搬过来一个元素,
         * 否则value[sub]与value[sub + 1]直接合并成一棵子树
         * 如果sub == length, 考虑value[sub - 1], 与上同
         *
         * todo 注意,(*this)对象的元素个数将由回溯到父结点时进行调整
         * todo 注意,这个函数将会直接在文件里修改value[]的信息,但是(*this)仍然没有被修改!
         */
        void merge(int sub, StoragePool<bpt_node> &storage) {
            bpt_node node1, node2;
            storage.Copy(value[sub], node1);
            if (node1.type == 0) {
                if (node1.length >= min_size) return;
                if (sub == length - 1) {
                    storage.Copy(value[sub - 1], node2);
                    if (node2.length == min_size) {
                        for (int i = 0; i < node1.length; ++i) {
                            node2.key[i + node2.length] = node1.key[i];
                            node2.value[i + node2.length] = node1.value[i];
                        }
                        node2.length += node1.length;
                        storage.Rewrite(value[sub - 1], node2);
                        storage.Erase(value[sub]);
                        --length;
                        return;
                    } else {
                        for (int i = node1.length - 1; i >= 0; --i) {
                            node1.key[i + 1] = node1.key[i];
                            node1.value[i + 1] = node1.value[i];
                        }
                        node1.key[0] = node2.key[node2.length - 1];
                        node1.value[0] = node2.value[node2.length - 1];
                        ++node1.length;
                        --node2.length;
                        key[sub] = node1.key[0];
                        storage.Rewrite(value[sub], node1);
                        storage.Rewrite(value[sub - 1], node2);
                    }
                } else {
                    storage.Copy(value[sub + 1], node2);
                    if (node2.length == min_size) {
                        for (int i = 0; i < node2.length; ++i) {
                            node1.key[i + node1.length] = node2.key[i];
                            node1.value[i + node1.length] = node2.value[i];
                        }
                        node1.length += node2.length;
                        storage.Rewrite(value[sub], node1);
                        storage.Erase(value[sub + 1]);
                        for (int i = sub + 1; i < length; ++i) {
                            key[i] = key[i + 1];
                            value[i] = value[i + 1];
                        }
                        --length;
                    } else {
                        node1.key[node1.length] = node2.key[0];
                        node1.value[node1.length] = node2.value[0];
                        ++node1.length;
                        for (int i = 1; i < node2.length; ++i) {
                            node2.key[i - 1] = node2.key[i];
                            node2.value[i - 1] = node2.value[i];
                        }
                        --node2.length;
                        storage.Rewrite(value[sub], node1);
                        storage.Rewrite(value[sub + 1], node2);
                        key[sub + 1] = node2.key[0];
                    }
                }
            } else {
                if (node1.length >= min_size) return;
                if (sub == length - 1) {
                    storage.Copy(value[sub - 1], node2);
                    if (node2.length == min_size) {
                        for (int i = 1; i <= node1.length; ++i) {
                            node2.key[i + node2.length] = node1.key[i];
                            node2.value[i + node2.length] = node1.value[i];
                        }
                        node2.length += node1.length;
                        node2.next = node1.next;
                        storage.Rewrite(value[sub - 1], node2);
                        storage.Erase(value[sub]);
                        --length;
                        return;
                    } else {
                        for (int i = 1; i <= node1.length; ++i) {
                            node1.key[i + 1] = node1.key[i];
                            node1.value[i + 1] = node1.value[i];
                        }
                        node1.key[1] = node2.key[node2.length];
                        node1.value[1] = node2.value[node2.length];
                        ++node1.length;
                        --node2.length;
                        storage.Rewrite(value[sub], node1);
                        storage.Rewrite(value[sub - 1], node2);
                        key[sub] = node1.key[1];
                        return;
                    }
                } else {
                    storage.Copy(value[sub + 1], node2);
                    if (node2.length == min_size) {
                        for (int i = 1; i <= node2.length; ++i) {
                            node1.key[i + node1.length] = node2.key[i];
                            node1.value[i + node1.length] = node2.value[i];
                        }
                        node1.length += node2.length;
                        storage.Rewrite(value[sub], node1);
                        storage.Erase(value[sub + 1]);
                        for (int i = sub + 1; i < length; ++i) {
                            key[i] = key[i + 1];
                            value[i] = value[i + 1];
                        }
                        node1.next = node2.next;
                        --length;
                        return;
                    } else {
                        node1.key[length + 1] = node2.key[1];
                        node1.value[length + 1] = node2.value[1];
                        ++node1.length;
                        for (int i = 2; i <= node2.length; ++i) {
                            node2.key[i - 1] = node2.key[i];
                            node2.value[i - 1] = node2.value[i];
                        }
                        --node2.length;
                        storage.Rewrite(value[sub], node1);
                        storage.Rewrite(value[sub - 1], node2);
                        key[sub + 1] = node2.key[1];
                        return;
                    }
                }
            }
        }
        /*
         * 为type_0的结点找到下一个叶节点在node.value[i]的序号i
         * (此函数对于insert、find、erase都是相同的)
         * todo 2021/5/31 检查通过
         */
        int find_subtree(const Key &_key) const{
            for (int i = 1; i <= length - 1; ++i) {
                if (_key >= key[i]) continue;
                else return i - 1;
            }
            return length - 1;
        }
        /*
         * 在type_1上定位关键字为_key的元素, 返回key[i]、 value[i]中的i, 如果不存在则返回-1, 为查找与删除功能服务
         * todo 2021/5/31 检查通过
         */
        int data_locate(const Key &_key) const{
            int left = 1, right = length, mid;
            while (left <= right) {
                mid = (left + right) >> 1;
                if (key[mid] == _key) return mid;
                else if (key[mid] > _key) right = mid - 1;
                else left = mid + 1;
            }
            return -1;
        }
    };

    int root;
    StoragePool<bpt_node> storage;

    /*
     * 私有insert函数，如果当前结点被分裂，返回新裂出的子节点的id，否则返回0，插入失败则返回-1
     */
    int insert(int t, const Key &_key, const int _value) {
        bpt_node node;
        storage.Copy(t, node);
        if (node.type == 0) {
            int sub_tree = node.find_subtree(_key);
            int p = insert(node.value[sub_tree], _key, _value);
            if (p == -1) return -1;
            else if (p == 0) return 0;
            else{
                node.index_insert(sub_tree, p, storage);
                storage.Rewrite(t, node);
                if (node.length == max_size) return split_index_node(t);
                else return 0;
            }
        } else {
            int p = node.data_insert(_key, _value);
            if (p == -1) return -1;
            storage.Rewrite(t, node);
            if (node.length == max_size) return split_data_node(t);
            else return 0;
        }
    }
    /*
     * 私有insert函数,失败返回-1,成功返回value
     */
    int erase(int t, const Key &_key) {
        bpt_node node;
        storage.Copy(t, node);
        if (node.type == 0) {
            int sub_tree = node.find_subtree(_key);
            int p = erase(node.value[sub_tree], _key);
            if (p == -1) return -1;
            else {
                bpt_node node1;
                storage.Copy(node.value[sub_tree], node1);
                if (node1.length == min_size) {
                    node.merge(sub_tree, storage);
                    storage.Rewrite(t, node);
                }
                return p;
            }
        } else {
            int p = node.data_erase(_key);
            if (p == -1) return -1;
            else {
                storage.Rewrite(t, node);
                return p;
            }
        }
    }
    /*
     * 将id为t的结点平分裂开，并返回分裂出的新结点的id
     * 原结点node的length = min_size,新结点length = node.length - min_size
     * todo 检查通过1
     */
    int split_index_node(int t){
        bpt_node node, new_node;
        storage.Copy(t, node);
        new_node.type = 0;
        new_node.length = node.length - min_size;
        for (int i = 0; i < node.length - min_size; ++i) {
            new_node.key[i] = node.key[i + min_size];
            new_node.value[i] = node.value[i + min_size];
        }
        node.length = min_size;
        storage.Rewrite(t, node);
        return storage.Insert(new_node);
    };
    /*
     * 将id为t的结点平分裂开，并返回分裂出的新结点的id
     * 原结点node的length = min_size,新结点length = node.length - min_size
     * todo 检查通过1
     */
    int split_data_node(int t) {
        bpt_node node, new_node;
        storage.Copy(t, node);
        new_node.type = 1;
        new_node.length = node.length - min_size;
        for (int i = 1; i <= node.length - min_size; ++i) {
            new_node.key[i] = node.key[i + min_size];
            new_node.value[i] = node.value[i + min_size];
        }
        new_node.next = node.next;
        node.length = min_size;
        node.next = storage.Insert(new_node);
        storage.Rewrite(t, node);
        return node.next;
    }
public:
    BPT(const std::string &f_n) :storage(f_n,Huancun){
        root = storage.Readint();
    }
    void clean(){
        storage.clean();
        root=-1;
    }
    /*
     * 成功返回0， 失败返回-1
     * todo 检查通过1
     */
    int Insert(const Key &_key, int _value){
        if (root == -1) {
            bpt_node node;
            node.type = 1;
            node.length = 1;
            node.key[1] = _key; node.value[1] = _value;
            root = storage.Insert(node);
            storage.Rewriteint(root);
            return 0;
        } else{
            int p = insert(root, _key, _value);
            if (p == -1) return -1;
            else if (p == 0) return 0;
            else{
                bpt_node new_root, split_node;
                storage.Copy(p, split_node);
                new_root.type = 0;
                new_root.length = 2;
                if (split_node.type == 0) new_root.key[1] = split_node.key[0];
                else new_root.key[1] = split_node.key[1];
                new_root.value[0] = root; new_root.value[1] = p;
                root = storage.Insert(new_root);
                storage.Rewriteint(root);
                return 0;
            }
        }
    }

    /*
     * 失败返回-1,成功返回value
     */
    int Erase(const Key &_key){
        if (root == -1) return -1;
        int p = erase(root, _key);
        if (p == -1) return -1;
        else {
            bpt_node node;
            storage.Copy(root, node);
            if (node.length == 1 && node.type == 0){
                int new_root = node.value[0];
                storage.Erase(root);
                root = new_root;
            }
            return p;
        }
    }
    /*
     * 失败-1 成功返回value
     */
    int Find(const Key &_key){
        if (root == -1) return -1;
        bpt_node node;
        int subtree;
        storage.Copy(root, node);
        while (node.type == 0) {
            subtree = node.find_subtree(_key);
            storage.Copy(node.value[subtree], node);
        }
        int index = node.data_locate(_key);
        if (index == -1) return -1;
        else return node.value[index];
    }
    /*
     * 区间查找, 符合条件的元素_key严格满足 left < _key < right
     */
    int Findinterval(const Key &left, const Key &right, int a[]){
        if (root == -1) return 0;
        bpt_node node;
        int sub_tree;
        storage.Copy(root, node);
        while (node.type == 0){
            sub_tree = node.find_subtree(left);
            storage.Copy(node.value[sub_tree], node);
        }
        int it, i = 0, j;
        //指向第一个可以取的元素
        for (j = 1; j <= node.length; ++j) {
            if (left >= node.key[j]) continue;
            else {it = j; break;}
        }
        if (j == node.length + 1) {
            if (node.next == -1) return 0;
            else {
                storage.Copy(node.next, node);
                it = 1;
            }
        }
        //装到a[]里去
        while (node.key[it] < right) {
            a[i++] = node.value[it];
            if (it < node.length) ++it;
            else {
                if (node.next == -1) {
                    break;
                } else {
                    it = 1;
                    storage.Copy(node.next, node);
                }
            }
        }
        return i;
    }
   /* void show_tree() {
        if (root == -1) return;
        bpt_node tmp;
        std::queue<int> node_queue;
        node_queue.push(root);
        while (!node_queue.empty()) {
            int a = node_queue.front();
            node_queue.pop();
            storage.Copy(a, tmp);
            std::cout << "node:\n" << "length: " << tmp.length << " type: " << tmp.type << std::endl;
            if (tmp.type == 0) {
                std::cout << "keys: ";
                for (int i = 1; i < tmp.length; ++i) {
                    std::cout << tmp.key[i].first << '_' << tmp.key[i].second << ' ';
                }
                std::cout << std::endl << "value: ";
                for (int i = 0; i < tmp.length; ++i) {
                    std::cout << tmp.value[i] << ' ';
                }
                std::cout << std::endl;
            } else {
                std::cout << "key: ";
                for (int i = 1; i <= tmp.length; ++i) {
                    std::cout << tmp.key[i].first << '_' << tmp.key[i].second << ' ';
                }
                std::cout << std::endl << "value: ";
                for (int i = 1; i <= tmp.length; ++i) {
                    std::cout << tmp.value[i] << ' ';
                }
                std::cout << std::endl;
            }
            if (tmp.type == 0) {
                for (int i = 0; i < tmp.length; ++i) {
                    node_queue.push(tmp.value[i]);
                }
            }
        }
    }
*//*
    void show_leaves() {
        if (root == -1) return;
        bpt_node node;
        std::queue<int> node_queue;
        node_queue.push(root);
        while (!node_queue.empty()) {
            storage.Copy(node_queue.front(), node);
            node_queue.pop();
            if (node.type == 0) {
                for (int i = 0; i < node.length; ++i) node_queue.push(node.value[i]);
            } else {
                printf("node:\nkey:\n");
                for (int i = 1; i <= node.length; ++i) printf("%d_%d ", node.key[i].first, node.key[i].second);
                printf("\nvalue:\n");
                for (int i = 1; i <= node.length; ++i) printf("%d ", node.value[i]);
                printf("\nnext: %d\n", node.next);
            }
        }
    }*/
};

#endif //TICKETSYSTEM_2021_BPT_HPP

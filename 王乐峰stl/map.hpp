//问题不是测试数据
//真正的问题是设计混乱，不得要领，遂导致种种稀奇古怪的问题发生，要多阅读别人的代码学习经验
//第一，用来代码重用的函数必定是干净简洁的函数，而不是本来就可能有bug的一 段 代 码
//第二，和前两个数据结构一样，测试数据的数据类型有可能没有默认的构造函数，所以结点里保存的应该是value_type *!!!!!!!!!!!!!

//注意平衡性维护：旋转，维护每个结点的子树高度

//疑难问题
//儿子不认爹
//解决：insert函数忘记修改一个节点的父指针

//erase函数异常终止：原因不确定，目前推断有可能是avl_node的析构函数delete value析构函数中的临时指针捣的鬼
//不不不，析构临时指针又不会析构指针指向的对象
/**
 * implement a container like std::map
 */
#ifndef SJTU_MAP_HPP
#define SJTU_MAP_HPP

// only for std::less<T>
#include <functional>
#include <cstddef>
#include <iostream>
#include "utility.hpp"
#include "exceptions.hpp"

namespace sjtu {

    template<
            class Key,
            class T,
            class Compare = std::less<Key>
    > class map {
    public:
        /**
         * the internal type of data.
         * it should have a default constructor, a copy constructor.
         * You can use sjtu::map as value_type by typedef.
         */
        typedef pair<const Key, T> value_type;
        /**
         * see BidirectionalIterator at CppReference for help.
         *
         * if there is anything wrong throw invalid_iterator.
         *     like it = map.begin(); --it;
         *       or it = map.end(); ++end();
         */
    private:
        struct avl_node{
            value_type *value;
            int height;
            avl_node *left, *right, *parent;

            avl_node() : value(nullptr), height(1),left(nullptr), right(nullptr), parent(nullptr){}
            avl_node(const value_type &_value, avl_node *_parent) : left(nullptr), right(nullptr), height(1){
                value = new value_type (_value);
                parent = _parent;
            }
            ~avl_node() {delete value;}
        };

        avl_node *root, *end_ptr;

        Compare cmp;

        void copy(avl_node * &_Dest, const avl_node *_Source, avl_node *_parent) {
            if (!_Source) {
                _Dest = nullptr;
                return;
            }
            _Dest = new avl_node(*(_Source->value), _parent);
            if (_Source->left) copy(_Dest->left, _Source->left, _Dest);
            if (_Source->right) copy(_Dest->right, _Source->right, _Dest);
            _Dest->height = max(get_height(_Dest->left), get_height(_Dest->right)) + 1;
        }

        avl_node *get_max(avl_node *t) const{
            while (t->right) t = t->right;
            return t;
        }

        avl_node *get_min(avl_node *t) const{
            while (t->left) t = t->left;
            return t;
        }

        avl_node *prior(avl_node *t) const{
            if (t == end_ptr) return get_max(root);
            avl_node *p = nullptr;
            if (t->left){
                return get_max(t->left);
            } else{
                p = t;
                while (p->parent && p != p->parent->right) p = p->parent;
                p = p->parent;
                if (!p) throw invalid_iterator();
            }
            return p;
        }

        avl_node *next(avl_node *t) const{
            if (t == end_ptr) throw invalid_iterator();
            avl_node *p = nullptr;
            if (t->right){
                return get_min(t->right);
            } else {
                p = t;
                while (p->parent && p != p->parent->left) p = p->parent;
                p = p->parent;
                if (!p) p = end_ptr;
            }
            return p;
        }

        avl_node *locate(const Key &key) const{
            avl_node *tmp = root;
            while (tmp && (cmp(key, tmp->value->first) || cmp(tmp->value->first, key))) {
                if (cmp(key, tmp->value->first)) tmp = tmp->left;
                else tmp = tmp->right;
            }
            return tmp;
        }

        size_t size_private(avl_node *t) const{
            if (!t) return 0;
            return size_private(t->left) + size_private(t->right) + 1;
        }

        void clear_private(avl_node * &t) {
            if (!t) return;
            clear_private(t->left);
            clear_private(t->right);
            delete t;
            t = nullptr;
        }
    public:
        class const_iterator;
        class iterator {
            friend class map;
        private:
            /**
             * TODO add data members
             *   just add whatever you want.
             */
            map *map_p;
            avl_node *node_p;
        public:
            // TODO
            iterator() {
                map_p = nullptr;
                node_p = nullptr;
            }
            iterator(map *_map_p, avl_node *_node_p) {
                map_p = _map_p;
                node_p = _node_p;
            }
            iterator(const iterator &other) {
                map_p = other.map_p;
                node_p = other.node_p;
            }
            iterator operator=(const iterator &other){
                map_p = other.map_p;
                node_p = other.node_p;
                return *this;
            }
            /**
             * TODO iter++
             */
            iterator operator++(int) {
                iterator it = *this;
                ++(*this);
                return it;
            }
            /**
             * TODO ++iter
             */
            iterator & operator++() {
                if (map_p->root == nullptr) throw invalid_iterator();
                node_p = map_p->next(node_p);
                return *this;
            }
            /**
             * TODO iter--
             */
            iterator operator--(int) {
                iterator it = *this;
                --(*this);
                return it;
            }
            /**
             * TODO --iter
             */
            iterator & operator--() {
                if (map_p->root == nullptr) {
                    throw invalid_iterator();
                } else{//如果不是空map，下面的代码一定能完成任务
                    node_p = map_p->prior(node_p);
                    return *this;
                }
            }
            /**
             * an operator to check whether two iterators are same (pointing to the same memory).
             */
            value_type & operator*() const {
                if (node_p == map_p->end_ptr) throw invalid_iterator();
                return *node_p->value;
            }
            bool operator==(const iterator &rhs) const {
                return map_p == rhs.map_p && node_p == rhs.node_p;
            }
            bool operator==(const const_iterator &rhs) const {
                return map_p == rhs.map_p && node_p == rhs.node_p;
            }
            /**
             * some other operator for iterator.
             */
            bool operator!=(const iterator &rhs) const {
                return map_p != rhs.map_p || node_p != rhs.node_p;
            }
            bool operator!=(const const_iterator &rhs) const {
                return map_p != rhs.map_p || node_p != rhs.node_p;
            }

            /**
             * for the support of it->first.
             * See <http://kelvinh.github.io/blog/2013/11/20/overloading-of-member-access-operator-dash-greater-than-symbol-in-cpp/> for help.
             */
            value_type* operator->() const noexcept {
                return node_p->value;
            }
        };
        class const_iterator {
            // it should has similar member method as iterator.
            //  and it should be able to construct from an iterator.
            friend class iterator;
            friend class map;
        private:
            // data members.
            const map *map_p;
            avl_node *node_p;
        public:
            const_iterator() {
                map_p = nullptr;
                node_p = nullptr;
            }
            const_iterator(const map *_map_p, avl_node *_node_p){
                map_p = _map_p;
                node_p = _node_p;
            }
            const_iterator(const const_iterator &other) {
                map_p = other.map_p;
                node_p = other.node_p;
            }
            const_iterator(const iterator &other) {
                map_p = other.map_p;
                node_p = other.node_p;
            }
            const_iterator operator=(const iterator &other) {
                map_p = other.map_p;
                node_p = other.node_p;
                return *this;
            }
            const_iterator operator=(const const_iterator &other) {
                map_p = other.map_p;
                node_p = other.node_p;
                return *this;
            }
            const_iterator operator++(int) {
                const_iterator it(*this);
                ++(*this);
                return it;
            }
            const_iterator & operator++(){
                if (map_p->root == nullptr) throw invalid_iterator();
                node_p = map_p->next(node_p);
                return *this;
            }
            const_iterator operator--(int) {
                const_iterator it(*this);
                --(*this);
                return it;
            }
            const_iterator operator--() {
                if (map_p->root == nullptr) throw invalid_iterator();
                node_p = map_p->prior(node_p);
                return *this;
            }
            const value_type & operator*() const {
                if (node_p == map_p->end_ptr) throw invalid_iterator();
                return *(node_p->value);
            }
            bool operator==(const iterator &rhs) const{
                return map_p == rhs.map_p && node_p == rhs.node_p;
            }
            bool operator==(const const_iterator &rhs) const{
                return map_p == rhs.map_p && node_p == rhs.node_p;
            }
            bool operator!=(const iterator &rhs) const{
                return map_p != rhs.map_p || node_p != rhs.node_p;
            }
            bool operator!=(const const_iterator &rhs) const{
                return map_p != rhs.map_p || node_p != rhs.node_p;
            }
            const value_type* operator->() const noexcept {
                return node_p->value;
            }
        };
        /**
         * TODO two constructors
         */
        map() {
            root = nullptr;
            end_ptr = new avl_node;
        }
        map(const map &other) {
            copy(root, other.root, nullptr);
            end_ptr = new avl_node;
        }
        /**
         * TODO assignment operator
         */
        map & operator=(const map &other) {
            if (root == other.root) return *this;
            clear();
            copy(root, other.root, nullptr);
            return *this;
        }
        /**
         * TODO Destructors
         */
        ~map() {
            clear();
            delete end_ptr;
        }
        /**
         * TODO
         * access specified element with bounds checking
         * Returns a reference to the mapped value of the element with key equivalent to key.
         * If no such element exists, an exception of type `index_out_of_bound'
         */
        T & at(const Key &key) {
            avl_node *tmp = locate(key);
            if (!tmp) throw index_out_of_bound();
            else return tmp->value->second;
        }
        const T & at(const Key &key) const {
            avl_node *tmp = locate(key);
            if (!tmp) throw index_out_of_bound();
            else return tmp->value->second;
        }
        /**
         * TODO
         * access specified element
         * Returns a reference to the value that is mapped to a key equivalent to key,
         *   performing an insertion if such key does not already exist.
         */
        T & operator[](const Key &key) {
            avl_node *tmp = locate(key);
            if (tmp) return tmp->value->second;
            return insert(value_type(key, T())).first.node_p->value->second;
        }
        /**
         * behave like at() throw index_out_of_bound if such key does not exist.
         */
        const T & operator[](const Key &key) const {
            avl_node *tmp = locate(key);
            if (tmp) return tmp->value->second;
            else throw index_out_of_bound();
        }
        /**
         * return a iterator to the beginning
         */
        iterator begin() {
            if (root) return iterator(this, get_min(root));
            else return iterator(this, end_ptr);
        }
        const_iterator cbegin() const {
            if (root) return const_iterator(this, get_min(root));
            else return const_iterator(this, end_ptr);
        }
        /**
         * return a iterator to the end
         * in fact, it returns past-the-end.
         */
        iterator end() {
            return iterator(this, end_ptr);
        }
        const_iterator cend() const {
            return const_iterator(this, end_ptr);
        }
        /**
         * checks whether the container is empty
         * return true if empty, otherwise false.
         */
        bool empty() const {
            return root == nullptr;
        }
        /**
         * returns the number of elements.
         */
        size_t size() const {
            return size_private(root);
        }
        /**
         * clears the contents
         */
        void clear() {
            clear_private(root);
        }
        /**
         * insert an element.
         * return a pair, the first of the pair is
         *   the iterator to the new element (or the element that prevented the insertion),
         *   the second one is true if insert successfully, or false.
         */
        pair<iterator, bool> insert(const value_type &value) {
            avl_node *tmp = locate(value.first);
            if (tmp) return pair<iterator, bool>(iterator(this, tmp), false);
            else return insert_private(value, root, nullptr);
        }
        /**
         * erase the element at pos.
         *
         * throw if pos pointed to a bad element (pos == this->end() || pos points an element out of this)
         */
        void erase(iterator pos) {
            if (pos.map_p != this || pos.node_p == pos.map_p->end_ptr) throw invalid_iterator();
            erase(root, pos.node_p);
        }

        /**
         * Returns the number of elements with key
         *   that compares equivalent to the specified argument,
         *   which is either 1 or 0
         *     since this container does not allow duplicates.
         * The default method of check the equivalence is !(a < b || b > a)
         */
        size_t count(const Key &key) const {
            if (locate(key)) return 1;
            else return 0;
        }
        /**
         * Finds an element with key equivalent to key.
         * key value of the element to search for.
         * Iterator to an element with key equivalent to key.
         *   If no such element is found, past-the-end (see end()) iterator is returned.
         */
        iterator find(const Key &key) {
            avl_node *tmp = locate(key);
            if (tmp) return iterator(this, tmp);
            else return iterator(this, end_ptr);
        }
        const_iterator find(const Key &key) const {
            avl_node *tmp = locate(key);
            if (tmp) return const_iterator(this, tmp);
            else return const_iterator(this, end_ptr);
        }
    private:
        pair<iterator, bool> insert_private(const value_type &value, avl_node * &t, avl_node *_parent){
            if (!t){
                t = new avl_node(value, _parent);
                return pair<iterator, bool>(iterator(this, t), true);
            }
            if (cmp(value.first, t->value->first)) {
                pair<iterator, bool> to_return = insert_private(value, t->left, t);
                if (get_height(t->left) - get_height(t->right) == 2) {
                    if (cmp(value.first, t->left->value->first))
                        LL(t);
                    else
                        LR(t);
                }
                t->height = max(get_height(t->left), get_height(t->right)) + 1;
                return to_return;
            } else{
                pair<iterator, bool> to_return = insert_private(value, t->right, t);
                if (get_height(t->right) - get_height(t->left) == 2) {
                    if (cmp(value.first, t->right->value->first))
                        RL(t);
                    else
                        RR(t);
                }
                t->height = max(get_height(t->left), get_height(t->right)) + 1;
                return to_return;
            }
        }

        bool erase(avl_node * &t, const avl_node *x) {
            if (!t) return true;
            if (!(cmp(x->value->first, t->value->first) || cmp(t->value->first, x->value->first))) {
                if (t->left == nullptr && t->right == nullptr) {
                    delete t;
                    t = nullptr;
                    return false;
                } else if (t->left && t->right) {
                    avl_node *tmp = get_min(t->right);
                    delete t->value;
                    t->value = new value_type (*(tmp->value));
                    if (erase(t->right, tmp)) return true;
                    else return adjust(t, 1);
                } else {
                    avl_node *tmp = (t->left)? t->left : t->right;
                    tmp->parent = t->parent;
                    delete t;
                    t = tmp;
                    return false;
                }
            } else if (cmp(x->value->first, t->value->first)){
                if (erase(t->left, x)) return true;
                else return adjust(t, 0);
            } else {
                if (erase(t->right, x)) return true;
                else return adjust(t, 1);
            }
        }

        void LL(avl_node * &t) {
            avl_node *t1 = t->left;

            t->left = t1->right;
            if (t1->right) t1->right->parent = t;

            t1->right = t;
            t1->parent = t->parent;

            t->parent = t1;
            t->height = max(get_height(t->left), get_height(t->right)) + 1;
            t1->height = max(get_height(t1->left), get_height(t1->right)) + 1;
            t = t1;
        }

        void LR(avl_node *&t) {
            RR(t->left);
            LL(t);
        }

        void RR(avl_node *&t) {
            avl_node *t1 = t->right;

            t->right = t1->left;
            if (t1->left) t1->left->parent = t;

            t1->left = t;
            t1->parent = t->parent;

            t->parent = t1;

            t->height = max(get_height(t->left), get_height(t->right)) + 1;
            t1->height = max(get_height(t1->left), get_height(t1->right)) + 1;
            t = t1;
        }

        void RL(avl_node *&t) {
            LL(t->right);
            RR(t);
        }

        int get_height(const avl_node *t) const{
            if (t) return t->height;
            return 0;
        }

        int max(int a, int b) {
            return (a > b)? a : b;
        }

        bool adjust(avl_node * &t, int sub_tree) {
            if (!sub_tree) {
                if (get_height(t->right) - get_height(t->left) == 1) return true;
                if (get_height(t->left) == get_height(t->right)) {
                    --(t->height);
                    return false;
                }
                if (get_height(t->right) - get_height(t->left) == 2) {
                    if (get_height(t->right->left) < get_height(t->right->right)) {
                        RR(t);
                        return false;
                    } else if (get_height(t->right->left) > get_height(t->right->right)) {
                        RL(t);
                        return false;
                    } else {
                        RR(t);
                        return true;
                    }
                }
            } else {
                if (get_height(t->left) == get_height(t->right)) {
                    --(t->height);
                    return false;
                }
                if (get_height(t->left) - get_height(t->right) == 1) {
                    return true;
                }
                if (get_height(t->left) - get_height(t->right) == 2) {
                    if (get_height(t->left->left) > get_height(t->left->right)) {
                        LL(t);

                        return false;
                    } else if (get_height(t->left->left) < get_height(t->left->right)) {
                        LR(t);
                        return false;
                    } else {
                        LL(t);
                        return true;
                    }
                }
            }
            exit(100);
        }

    public:
        int check_whole_tree() {
            int i = 0;
            iterator it = begin();
            while (it != end()) {
                if (it.node_p->left) {
                    if (it.node_p->left->parent != it.node_p) ++i;
                }
                if (it.node_p->right) {
                    if (it.node_p->right->parent != it.node_p) ++i;
                }
                ++it;
            }
            return i;
        }
    };

}

#endif

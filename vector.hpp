#ifndef VECTOR_HPP
#define VECTOR_HPP

#include <climits>
#include <cstddef>

namespace sjtu {
    /**
 * a data container like std::vector
 * store data in a successive memory and support random access.
 */
    template<typename T>
    class vector {
    private:
        T **data;
        int length, capacity;
        void doubleSpace(){
            T **replace = new T *[this->capacity * 2];
            for (int i = 0; i < this->length; ++i) {
                replace[i] = new T(*(this->data[i]));
            }
            clear_literally();
            this->data = replace;
            this->capacity *= 2;
        }
        void clear_literally(){
            for (int i = 0; i < this->length; ++i) {
                delete data[i];
            }
            delete [] this->data;
            this->data = nullptr;
        }
    public:
        /**
         * TODO
         * a type for actions of the elements of a vector, and you should write
         *   a class named const_iterator with same interfaces.
         */
        /**
         * you can see RandomAccessIterator at CppReference for help.
         */
        class const_iterator;
        class iterator {
            friend const_iterator;
            friend vector;
        private:
            /**
             * TODO add data members
             *   just add whatever you want.
             */
             T **position;
             vector<T> *pVector;
        public:
            /**
             * return a new iterator which pointer n-next elements
             * as well as operator-
             */
            iterator() = default;
            iterator(const iterator &other){
                this->position = other.position;
                this->pVector = other.pVector;
            }
            iterator &operator=(const iterator &other){
                this->position = other.position;
                this->pVector = other.pVector;
                return *this;
            }
            iterator operator+(const int &n) const {
                //TODO
                iterator tmp;
                tmp.pVector = this->pVector;
                tmp.position = this->position + n;
                return tmp;
            }
            iterator operator-(const int &n) const {
                //TODO
                iterator tmp;
                tmp.pVector = this->pVector;
                tmp.position = tmp.position - n;
                return tmp;
            }
            // return the distance between two iterators,
            // if these two iterators point to different vectors, throw invaild_iterator.
            int operator-(const iterator &rhs) const {
                //TODO
                return this->position - rhs.position;
            }
            iterator& operator+=(const int &n) {
                //TODO
                this->position += n;
                return *this;
            }
            iterator& operator-=(const int &n) {
                //TODO
                this->position -= n;
                return *this;
            }
            /**
             * TODO iter++
             */
            iterator operator++(int) {
                iterator tmp;
                tmp.position = this->position;
                tmp.pVector = this->pVector;
                ++this->position;
                return tmp;
            }
            /**
             * TODO ++iter
             */
            iterator& operator++() {
                ++this->position;
                return *this;
            }
            /**
             * TODO iter--
             */
            iterator operator--(int) {
                iterator tmp;
                tmp.position = this->position;
                tmp.pVector = this->pVector;
                --this->position;
                return tmp;
            }
            /**
             * TODO --iter
             */
            iterator& operator--() {
                --this->position;
                return *this;
            }
            /**
             * TODO *it
             */
            T& operator*() const{
                return *(*(this->position));
            }
            /**
             * a operator to check whether two iterators are same (pointing to the same memory address).
             */
            bool operator==(const iterator &rhs) const {
                return this->position == rhs.position;
            }
            bool operator==(const const_iterator &rhs) const {
                return this->position == rhs.position;
            }
            /**
             * some other operator for iterator.
             */
            bool operator!=(const iterator &rhs) const {
                return this->position != rhs.position;
            }
            bool operator!=(const const_iterator &rhs) const {
                return this->position != rhs.position;
            }
        };
        /**
         * TODO
         * has same function as iterator, just for a const object.
         */
        class const_iterator {
            friend iterator;
            friend vector;
        private:
            T **position;
            const vector<T> *pVector;
        public:
            const_iterator() = default;
            const_iterator(const const_iterator &other){
                this->position = other.position;
                this->pVector = other.pVector;
            }
            const_iterator &operator=(const const_iterator &other){
                this->position = other.position;
                this->pVector = other.pVector;
                return *this;
            }
            const_iterator operator+(const int &n) const{
                const_iterator tmp;
                tmp.position = this->position + n;
                tmp.pVector = this->pVector;
                return tmp;
            }
            const_iterator operator-(const int &n) const{
                const_iterator tmp;
                tmp.position = this->position - n;
                tmp.pVector = this->pVector;
                return tmp;
            }
            int operator-(const const_iterator &rhs) const{
                return this->position - rhs.position;
            }
            const_iterator &operator+=(const int &n){
                this->position += n;
                return *this;
            }
            const_iterator &operator-=(const int &n){
                this->position -= n;
                return *this;
            }
            const_iterator operator++(int){
                const_iterator tmp;
                tmp.position = this->position;
                tmp.pVector = this->pVector;
                ++this->position;
                return tmp;
            }
            const_iterator &operator++(){
                ++this->position;
                return *this;
            }
            const_iterator operator--(int){
                const_iterator tmp;
                tmp.position = this->position;
                tmp.pVector = this->pVector;
                ++this->position;
                return tmp;
            }
            const_iterator operator--(){
                ++this->position;
                return *this;
            }
            const T &operator*() const{
                return *(*(this->position));
            }
            bool operator==(const const_iterator &rhs) const{ return this->position == rhs.position; }
            bool operator==(const iterator &rhs) const{ return this->position == rhs.position;}
            bool operator!=(const const_iterator &rhs) const{ return this->position != rhs.position; }
            bool operator!=(const iterator &rhs) const{ return this->position != rhs.position; }
        };
        /**
         * TODO Constructs
         * At least two: default constructor, copy constructor
         */
        vector() {
            this->length = 0;
            this->capacity = 5;
            this->data = new T *[5];
        }
        vector(const vector &other) {
            this->length = other.length;
            this->capacity = other.capacity;
            this->data = new T *[other.capacity];
            for (int i = 0; i < other.length; ++i) {
                this->data[i] = new T(*(other.data[i]));
            }
        }
        /**
         * TODO Destructor
         */
        ~vector() {
            clear_literally();
        }
        /**
         * TODO Assignment operator
         */
        vector &operator=(const vector &other) {
            if (this->data == other.data) return *this;
            clear_literally();
            this->capacity = other.capacity;
            this->length = other.length;
            this->data = new T *[other.capacity];
            for (int i = 0; i < other.length; ++i) {
                this->data[i] = new T(*(other.data[i]));
            }
            return *this;
        }
        /**
         * assigns specified element with bounds checking
         * throw index_out_of_bound if pos is not in [0, size)
         */
        T & at(const size_t &pos) {
            return *(this->data[pos]);
        }
        const T & at(const size_t &pos) const {
            return *(this->data[pos]);
        }
        /**
         * assigns specified element with bounds checking
         * throw index_out_of_bound if pos is not in [0, size)
         * !!! Pay attentions
         *   In STL this operator does not check the boundary but I want you to do.
         */
        T & operator[](const size_t &pos) {
            return *(this->data[pos]);
        }
        const T & operator[](const size_t &pos) const {
            return *(this->data[pos]);
        }
        /**
         * access the first element.
         * throw container_is_empty if size == 0
         */
        const T & front() const {
            return *(this->data[0]);
        }
        /**
         * access the last element.
         * throw container_is_empty if size == 0
         */
        T & back() const {
            return *(this->data[this->length - 1]);
        }
        /**
         * returns an iterator to the beginning.
         */
        iterator begin() {
            iterator start;
            start.position = this->data;
            start.pVector = this;
            return start;
        }
        const_iterator cbegin() const {
            const_iterator start;
            start.position = this->data;
            start.pVector = this;
            return start;
        }
        /**
         * returns an iterator to the end.
         */
        iterator end() {
            return this->begin() + this->length;
        }
        const_iterator cend() const {
            return this->cbegin() + this->length;
        }
        /**
         * checks whether the container is empty
         */
        bool empty() const {
            if (this->length == 0) return true;
            else return false;
        }
        /**
         * returns the number of elements
         */
        size_t size() const { return this->length; }
        /**
         * clears the contents
         */
        void clear() {
            clear_literally();
            this->length = 0;
            this->capacity = 10;
            this->data = new T *[this->capacity];
        }
        /**
         * inserts value before pos
         * returns an iterator pointing to the inserted value.
         */
        iterator insert(iterator pos, const T &value) {
            int ind = pos - this->begin();
            return insert(ind, value);
        }
        /**
         * inserts value at index ind.
         * after inserting, this->at(ind) == value
         * returns an iterator pointing to the inserted value.
         * throw index_out_of_bound if ind > size (in this situation ind can be size because after inserting the size will increase 1.)
         */
        iterator insert(const size_t &ind, const T &value) {
            if (this->length == this->capacity) doubleSpace();
            for (int i = this->length; i > ind; --i){
                this->data[i] = this->data[i - 1];
            }
            this->data[ind] = new T(value);
            ++this->length;
            return this->begin() + ind;
        }
        /**
         * removes the element at pos.
         * return an iterator pointing to the following element.
         * If the iterator pos refers the last element, the end() iterator is returned.
         */
        iterator erase(iterator pos) {
            int ind = pos - this->begin();
            return erase(ind);
        }
        /**
         * removes the element with index ind.
         * return an iterator pointing to the following element.
         * throw index_out_of_bound if ind >= size
         */
        iterator erase(const size_t &ind) {
            if (ind == this->length - 1){
                delete this->data[ind];
                --this->length;
                return this->end();
            }
            else{
                delete this->data[ind];
                for (int i = ind; i < this->length - 1; ++i){
                    this->data[i] = this->data[i + 1];
                }
                --this->length;
                return this->begin() + ind;
            }
        }
        /**
         * adds an element to the end.
         */
        void push_back(const T &value) {
            if (this->length == this->capacity) doubleSpace();
            this->data[this->length] = new T(value);
            ++this->length;
        }
        /**
         * remove the last element from the end.
         * throw container_is_empty if size() == 0
         */
        void pop_back() {
            delete this->data[this->length - 1];
            --this->length;
        }
    };
}

#endif

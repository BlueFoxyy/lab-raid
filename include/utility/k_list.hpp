#pragma once
#include <memory>
#include <vector>
#include <exception>

template <class T, class A = std::allocator<T> >
class kList {
private:
    struct Node {
        T val;
        size_t anc, l, r;
        size_t sz;

        bool operator == (const Node& node) {
            return anc == node.anc
                && l == node.l
                && r == node.r;
        }
        
        Node() : anc(-1), l(-1), r(-1) {}
    };
    std::vector<Node> nodes;

    size_t getPos(Node node) const {
        int pos = 0;
        Node ancNode;
        while (node.anc != -1) {
            ancNode = nodes[node.anc];
            if (ancNode.r != -1 && nodes[ancNode.r] == node) {
                pos += nodes[ancNode.l].sz + 1;
            }
            node = ancNode;
        }
        return pos;
    }
    
public:
    typedef A allocator_type;
    typedef typename A::value_type value_type;
    typedef typename A::reference reference;
    typedef typename A::const_reference const_reference;
    typedef typename A::difference_type difference_type;
    typedef typename A::size_type size_type;

    class iterator {
    private:
        Node* pnode;
    public:
        typedef typename A::difference_type difference_type;
        typedef typename A::value_type value_type;
        typedef typename A::reference reference;
        typedef typename A::pointer pointer;
        typedef std::random_access_iterator_tag iterator_category; //or another tag

        iterator() : pnode(nullptr) {}
        iterator(const iterator& it) : pnode(it.pnode) {}
        ~iterator() {}

        iterator& operator=(const iterator& it) { pnode.reset(it.pnode); return *this; }
        bool operator==(const iterator& it) const { return pnode == it.pnode; }
        bool operator!=(const iterator& it) const { return pnode != it.pnode; }
        bool operator<(const iterator& it) const { return getPos(*pnode) < getPos(*it.pnode); } //optional
        bool operator>(const iterator& it) const { return getPos(*pnode) > getPos(*it.pnode); } //optional
        bool operator<=(const iterator& it) const { return pnode == it.pnode || getPos(*pnode) < getPos(*it.pnode); } //optional
        bool operator>=(const iterator& it) const { return pnode == it.pnode || getPos(*pnode) > getPos(*it.pnode); } //optional

        iterator& operator++() {
            if (pnode == nullptr)
                throw std::out_of_range("Attempted to increase iterator beyond kList::end().");
            // TODO: continue from this mark.
        }
        iterator operator++(int); //optional
        iterator& operator--(); //optional
        iterator operator--(int); //optional
        iterator& operator+=(size_type); //optional
        iterator operator+(size_type) const; //optional
        friend iterator operator+(size_type, const iterator&); //optional
        iterator& operator-=(size_type); //optional            
        iterator operator-(size_type) const; //optional
        difference_type operator-(iterator) const; //optional

        reference operator*() const;
        pointer operator->() const;
        reference operator[](size_type) const; //optional
    };
    class const_iterator {
    public:
        typedef typename A::difference_type difference_type;
        typedef typename A::value_type value_type;
        typedef typename const A::reference reference;
        typedef typename const A::pointer pointer;
        typedef std::random_access_iterator_tag iterator_category; //or another tag

        const_iterator();
        const_iterator(const const_iterator&);
        const_iterator(const iterator&);
        ~const_iterator();

        const_iterator& operator=(const const_iterator&);
        bool operator==(const const_iterator&) const;
        bool operator!=(const const_iterator&) const;
        bool operator<(const const_iterator&) const; //optional
        bool operator>(const const_iterator&) const; //optional
        bool operator<=(const const_iterator&) const; //optional
        bool operator>=(const const_iterator&) const; //optional

        const_iterator& operator++();
        const_iterator operator++(int); //optional
        const_iterator& operator--(); //optional
        const_iterator operator--(int); //optional
        const_iterator& operator+=(size_type); //optional
        const_iterator operator+(size_type) const; //optional
        friend const_iterator operator+(size_type, const const_iterator&); //optional
        const_iterator& operator-=(size_type); //optional            
        const_iterator operator-(size_type) const; //optional
        difference_type operator-(const_iterator) const; //optional

        reference operator*() const;
        pointer operator->() const;
        reference operator[](size_type) const; //optional
    };

    typedef std::reverse_iterator<iterator> reverse_iterator; //optional
    typedef std::reverse_iterator<const_iterator> const_reverse_iterator; //optional

    kList();
    kList(const kList&);
    ~kList();

    kList& operator=(const kList&);
    bool operator==(const kList&) const;
    bool operator!=(const kList&) const;
    bool operator<(const kList&) const; //optional
    bool operator>(const kList&) const; //optional
    bool operator<=(const kList&) const; //optional
    bool operator>=(const kList&) const; //optional

    iterator begin();
    const_iterator begin() const;
    const_iterator cbegin() const;
    iterator end();
    const_iterator end() const;
    const_iterator cend() const;
    reverse_iterator rbegin(); //optional
    const_reverse_iterator rbegin() const; //optional
    const_reverse_iterator crbegin() const; //optional
    reverse_iterator rend(); //optional
    const_reverse_iterator rend() const; //optional
    const_reverse_iterator crend() const; //optional

    reference front(); //optional
    const_reference front() const; //optional
    reference back(); //optional
    const_reference back() const; //optional
    template<class ...Args>
    void emplace_front(Args&&...); //optional
    template<class ...Args>
    void emplace_back(Args&&...); //optional
    void push_front(const T&); //optional
    void push_front(T&&); //optional
    void push_back(const T&); //optional
    void push_back(T&&); //optional
    void pop_front(); //optional
    void pop_back(); //optional
    reference operator[](size_type); //optional
    const_reference operator[](size_type) const; //optional
    reference at(size_type); //optional
    const_reference at(size_type) const; //optional

    template<class ...Args>
    iterator emplace(const_iterator, Args&&...); //optional
    iterator insert(const_iterator, const T&); //optional
    iterator insert(const_iterator, T&&); //optional
    iterator insert(const_iterator, size_type, T&); //optional
    template<class iter>
    iterator insert(const_iterator, iter, iter); //optional
    iterator insert(const_iterator, std::initializer_list<T>); //optional
    iterator erase(const_iterator); //optional
    iterator erase(const_iterator, const_iterator); //optional
    void clear(); //optional
    template<class iter>
    void assign(iter, iter); //optional
    void assign(std::initializer_list<T>); //optional
    void assign(size_type, const T&); //optional

    void swap(kList&);
    size_type size() const;
    size_type max_size() const;
    bool empty() const;

    A get_allocator() const; //optional
};
template <class T, class A = std::allocator<T> >
void swap(kList<T, A>& a, kList<T, A>& b) {
    swap(a.nodes, b.nodes);
}

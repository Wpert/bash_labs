#ifndef LINKEDLIST_HPP
#define LINKEDLIST_HPP

#include <iostream>
#include <iterator>
#include <initializer_list>

namespace mephi {

// do i really want to do this?
// ok, let it be some kind of beginners realisation of linked list

template <typename T>
class LinkedList {
    struct Node {
        Node() = default;
        Node(Node&& other) = default;
        Node(Node* prev_p, Node* next_p)
        : prev_(prev_p), data_(nullptr),  next_(next_p) {}
        Node(Node* prev_p, T* data, Node* next_p)
        : prev_(prev_p), data_(data),  next_(next_p) {}
        
        ~Node() {
            delete data_;
        }

        Node* prev_;
        T* data_;
        Node* next_;
    };

public:
    struct iterator {
        using iterator_category = std::bidirectional_iterator_tag;

        iterator(Node* node_ptr) : node_ptr_(node_ptr) {}
    
        T& operator*() const { return *node_ptr_->data_; }
        T* operator->() { return node_ptr_->data_; }

        // Prefix
        iterator& operator++() { node_ptr_ = node_ptr_->next_; return *this; }
        iterator& operator--() { node_ptr_ = node_ptr_->prev_; return *this; }
        // Postfix
        iterator operator++(int) { iterator tmp = *this; ++(*this); return tmp; }
        iterator operator--(int) { iterator tmp = *this; --(*this); return tmp; }

        friend bool operator== (const iterator& a, const iterator& b) { return a.node_ptr_ == b.node_ptr_; };
        friend bool operator!= (const iterator& a, const iterator& b) { return a.node_ptr_ != b.node_ptr_; };  
    private:
        Node* node_ptr_;
    };

    LinkedList() : head_(new Node), tail_(new Node), size_(0) {        
        head_->prev_ = nullptr;
        head_->next_ = tail_;

        tail_->prev_ = head_;
        tail_->next_ = nullptr;
    }

    LinkedList(const std::initializer_list<T>& list) : LinkedList() {
        for (auto& elem : list) {
            push_back(elem);
        }
    }

    ~LinkedList() {
        Node* current = head_->next_;
        while (current != tail_) {
            Node* next = current->next_;
            delete current;
            current = next;
        }
        delete head_;
        delete tail_;
    }

    LinkedList& operator=(const LinkedList& list) {
        clear();
        for (auto& elem : list) {
            push_back(elem);
        }
    }

    iterator begin() { return iterator(head_->next_); }
    iterator end() { return iterator(tail_); }

    iterator rbegin() { return iterator(tail_->prev_); }
    iterator rend() { return iterator(head_); }

    void insert(const iterator& position, const T& data) {
        Node* current = position->node_ptr_;
        Node* next = current->next_;
        Node* new_node = new Node(current, new T(data), next);

        current->next_ = new_node;
        next->prev_ = new_node;
    }

    void erase(const iterator& position) {
        Node* current = position->node_ptr_;
        Node* prev = current->prev_;
        Node* next = current->next_;

        prev->next_ = next;
        next->prev_ = prev;

        delete current;
    }

    void push_front(const T& data) {
        Node* new_node = new Node(head_, new T(data), head_->next_);
        Node* first = head_->next_;
        first->prev_ = new_node;
        head_->next_ = new_node;
        ++size();
    }

    void push_back(const T& data) {
        Node* new_node = new Node(tail_->prev_, new T(data), tail_);
        Node* last = tail_->prev_;
        last->next_ = new_node;
        tail_->prev_ = new_node;
        ++size_;
    }

    void pop_front() {
        if (size_ <= 0) {
            throw std::runtime_error("LinkedList::pop_front from empty list");
        }
        Node* first = head_->next_;
        Node* next = first->next_;
        delete first;

        head_->next_ = next;
        next->prev_ = head_;
        --size_;
    }

    void pop_back() {
        if (size_ <= 0) {
            throw std::runtime_error("LinkedList::poop_back from empty list");
        }
        Node* last = tail_->prev_;
        Node* prev = last->prev_;
        prev->next_ = tail_;
        tail_->prev_ = prev;
        delete last;
        --size_;
    }

    void swap(LinkedList& list) {
        std::swap(list.head_, head_);
        std::swap(list.tail_, tail_);
        std::swap(list.size_, size_);
    }

    void clear() {
        while (size_ > 0) {
            pop_front();
        }
    }

    size_t size() const { return size_; }

    void reverse() { std::swap(head_, tail_); }

private:
    Node* head_;
    Node* tail_;
    size_t size_;
};

}

#endif

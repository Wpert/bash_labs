#ifndef SHARED_HPP
#define SHARED_HPP

#include "unique.hpp"
#include <atomic>

namespace mephi {

template <typename T>
class SharedPtr {
    class SharedControlBlock {
    private:
        UniquePtr<T> object_;
        std::atomic<long> ref_count_;
    public:
        SharedControlBlock() : object_(nullptr), ref_count_(0) {}
        SharedControlBlock(T* ptr): object_(ptr), ref_count_(0) {
            if (object_) ref_count_ = 1;
        };

        void addRef() {
            ref_count_.fetch_add(1);
        }

        void decreaseRef() {
            ref_count_.fetch_sub(1);
            if (ref_count_.load() == 0){
                object_.reset();
                delete this;
            }
        }

        T* get() const { return object_.get();}

        long use_count() const {return ref_count_.load(); }
    };

public:
    explicit SharedPtr(T* object = nullptr) : state_(new SharedControlBlock(object)) {}

    SharedPtr(const SharedPtr& other) : state_(other.state_) {
        other.state_->addRef();
    }

    ~SharedPtr() {
        state_->decreaseRef();
    }

    void swap(SharedPtr<T>& other) {
        other.state_ = std::exchange(state_, other.state_);
    }

    T* get() const {
        return state_->get();
    }
    void reset(T* other = nullptr) {
        state_->decreaseRef();
        state_ = new SharedControlBlock(other);
    }

    bool unique() const {
        return state_->use_count() == 1;
    }

    long use_count() const {
        return state_->use_count();
    }

    T& operator*() const {
        return *(state_->get());
    }

    T* operator->() const {return *(state_->get());}

    explicit operator bool() const {
        return static_cast<bool>(state_->get());
    }

    SharedPtr& operator=(const SharedPtr& other) {
        state_->decreaseRef();
        state_ = other.state_;
        state_->addRef();
        return *this;
    }

private:
    SharedControlBlock* state_;
};

}

#endif

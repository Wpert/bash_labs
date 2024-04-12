#include <memory>
#include <vector>
#include <optional>
#include <iostream>

namespace mephi {

const size_t MIN_ELEMENTS = 8;

enum class NodeStatus {
    EMPTY,
    OCCUPIED,
    DELETED
};

template<typename TElement, typename TKey>
struct Node {
    TElement element_;
    TKey key_;
    NodeStatus status_ = NodeStatus::EMPTY;

    Node() = default;
    Node(const Node&) = default;
    Node(const TElement& element, const TKey& key, NodeStatus status)
    : element_(element), key_(key), status_(status) {}
    Node(const TElement& element, const TKey& key) {
        Node(element, key, NodeStatus::OCCUPIED);
    }
    Node& operator=(const Node&) = default;

    bool has_value() const {
        return status_ == NodeStatus::OCCUPIED;
    }
    bool is_empty() const {
        return status_ == NodeStatus::EMPTY;
    }
    bool is_initialized() const {
        return status_ != NodeStatus::EMPTY;
    }
};

// cannot do double hashing ;<
// double hashed -- https://en.wikipedia.org/wiki/Open_addressing
// open address -- https://en.wikipedia.org/wiki/Open_addressing
template<typename TElement, typename TKey, typename THash = std::hash<TKey>>
class HashTable {
using TNode = Node<TElement, TKey>;
public:
    HashTable() : data_(MIN_ELEMENTS) {}

    size_t size() const {
        return realSize_;
    }
    size_t tableCapacity() const {
        return data_.size();
    }
    size_t dataCapacity() const {
        return data_.capacity();
    }

    void insert(const TElement& element, const TKey& key) {
        // check space, resize if needed
        double coeff = static_cast<double>(realSize_) / data_.size();
        if (coeff >= 0.5)
            resize();

        size_t i = look_up(key);
        data_[i] = {element, key, NodeStatus::OCCUPIED};
        ++realSize_;
    }

    void erase(const TKey& key) {
        size_t i = look_up(key);
        data_[i].status_ = NodeStatus::DELETED;
        // data_[i] = {NULL, NULL, NodeStatus::DELETED};
        --realSize_;
    }

    bool contains(const TKey& key) const {
        size_t i = look_up(key);
        if (!data_[i].has_value())
            return false;
        
        return true;
    }

    const TElement& at(const TKey& key) const {
        size_t i = look_up(key);
        if (!data_[i].has_value())
            throw std::out_of_range("key not found");

        return data_[i]->element_;
    }

    void reserve(size_t new_size) {
        if (new_size <= data_.size()) {
            throw std::runtime_error("Attempt to shrink hash table");
        }

        resize(new_size);
    }

    TElement& operator[](const TKey& key) {
        size_t i = look_up(key);
        if (!data_[i].has_value())
            data_[i] = {NULL, NULL, NodeStatus::OCCUPIED};

        return data_[i]->element_;
    }

private:
    inline void probing(size_t& i) const {
        i = (i + 1) % data_.size();
    }

    size_t look_up(const TKey& key) const {
        size_t i = hasher_(key) % data_.size();
        
        while (true) {
            // found it
            if (data_[i].has_value() && data_[i].key_ == key)
                return i;
            // not found and not initialized
            else if (!data_[i].is_initialized())
                return i;
            
            probing(i);
        }
    }

    void resize(size_t new_size) {
        // cannot shrink !!
        if (new_size <= data_.size()) {
            throw std::runtime_error("Attempted to shrink hash table");
        }

        std::vector< TNode > new_data(new_size);
        for (auto& node : data_) {
            if (node.has_value()) {
                size_t i = look_up(node.key_);
                new_data[i] = node;
            }
        }

        data_ = std::move(new_data);
    }

    void resize() {
        resize(data_.size() * 2);
    }

    THash hasher_;
    size_t realSize_ = 0;
    std::vector< TNode > data_;
};

}

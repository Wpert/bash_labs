#include <memory>
#include <vector>
#include <list>
#include <optional>
#include <iostream>

namespace mephi {

const size_t MIN_ELEMENTS = 8;

template<typename TElement, typename TKey>
struct Node {
    TElement element_;
    TKey key_;

    Node() = default;
    Node(const Node&) = default;
    Node(const TElement& element, const TKey& key) {}
    Node& operator=(const Node&) = default;
};

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
        data_[i].push_back({element, key});
        ++realSize_;
    }

    void erase(const TKey& key) {
        size_t i = look_up(key);
        // data_[i].status_ = NodeStatus::DELETED;
        for (auto& node : data[i]) {
            if (node.key_ == key) {
                data_[i].erase(node);
                return;
            }  
        }
        --realSize_;
    }

    bool contains(const TKey& key) const {
        size_t i = look_up(key);
        for (const auto& node : data[i]) {
            if (node.key_ == key)
                return true;
        }
        return true;
    }

    const TElement& at(const TKey& key) const {
        size_t i = look_up(key);

        for (const auto& node : data[i]) {
            if (node.key_ == key)
                return node.element_;
        };

        throw std::out_of_range("key not found");
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

    size_t look_up(const TKey& key) const {
        size_t i = hasher_(key) % data_.size();
        return i;
    }

    void resize(size_t new_size) {
        // cannot shrink !!
        if (new_size <= data_.size()) {
            throw std::runtime_error("Attempted to shrink hash table");
        }

        std::vector< TNode > new_data(new_size);
        for (auto& list_ : data_) {
            if (!list_.empty()) {
                for (auto& node : list_) {
                    size_t i = hasher_(node.key_) % new_size;
                    new_data[i].push_back(node);
                }
                // size_t i = look_up(node.key_);
                // new_data[i] = node;
            }
        }

        data_ = std::move(new_data);
    }

    void resize() {
        resize(data_.size() * 2);
    }

    THash hasher_;
    size_t realSize_ = 0;
    std::vector< std::list<TNode> > data_;
};

}

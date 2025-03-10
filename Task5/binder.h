#ifndef BINDER_H
#define BINDER_H

#include <map>
#include <list>
#include <memory>
#include <stdexcept>

namespace cxx {

template <typename K, typename V>
class binder {
public:
    binder() noexcept = default;

    binder(const binder& other) {
        if (other.non_const_read) {
            data_ = std::make_shared<Data>(*other.data_);
            for (auto it = data_->list.begin(); it != data_->list.end(); ++it) {
                data_->map[it->first] = it;
            }
        } else {
            data_ = other.data_;
        }
    }

    binder(binder&& other) noexcept = default;

    binder& operator=(binder other) noexcept {
        swap(*this, other);
        return *this;
    }

    void insert_front(const K& k, const V& v) {
        auto key_ptr = std::make_shared<K>(k);
        if (data_ != nullptr && data_->map.count(key_ptr)) {
            throw std::invalid_argument("Duplicate key");
        }
        auto pair = std::make_pair(key_ptr, v);
        auto new_data = ensure_unique();
        bool changed = false;
        try {
            new_data->list.emplace_front(pair);
            changed = true;
            new_data->map[key_ptr] = new_data->list.begin();
            data_ = new_data;
            non_const_read = false;
        } catch (const std::bad_alloc& badalloc) {
            if (changed) {
                new_data->list.pop_front();
            }
            throw badalloc;
        }
    }

    void insert_after(const K& prev_k, const K& k, const V& v) {
        auto prev_key_ptr = std::make_shared<K>(prev_k);
        auto key_ptr = std::make_shared<K>(k);
        if (data_->map.count(prev_key_ptr) == 0 || data_->map.count(key_ptr)) {
            throw std::invalid_argument("Invalid key");
        }
        auto pair = std::make_pair(key_ptr, v);
        auto new_data = ensure_unique();
        bool changed = false;
        try {
            auto list_it = std::next(new_data->map[prev_key_ptr]);
            new_data->list.insert(list_it, pair);
            changed = true;
            new_data->map[key_ptr] = std::prev(list_it);
            data_ = new_data;
            non_const_read = false;
        } catch (const std::bad_alloc& badalloc) {
            if (changed) {
                new_data->list.erase(new_data->map[key_ptr]);
            }
            throw badalloc;
        }
    }

    void remove() {
        if (data_ == nullptr || data_->list.empty()) {
            throw std::invalid_argument("Empty binder");
        }
        auto new_data = ensure_unique();
        auto it = new_data->list.begin();
        new_data->map.erase(it->first);
        new_data->list.erase(it);
        data_ = new_data;
        non_const_read = false;
    }

    void remove(const K& k) {
        auto key_ptr = std::make_shared<K>(k);
        if (data_ == nullptr || !data_->map.count(key_ptr)) {
            throw std::invalid_argument("Key not found");
        }
        auto new_data = ensure_unique();
        new_data->list.erase(new_data->map[key_ptr]);
        new_data->map.erase(key_ptr);
        data_ = new_data;
        non_const_read = false;
    }

    V& read(const K& k) {
        auto key_ptr = std::make_shared<K>(k);
        if (data_ == nullptr || !data_->map.count(key_ptr)) {
            throw std::invalid_argument("Key not found");
        }
        data_ = ensure_unique();
        non_const_read = true;
        return data_->map[key_ptr]->second;
    }

    const V& read(const K& k) const {
        auto key_ptr = std::make_shared<K>(k);
        if (data_ == nullptr || !data_->map.count(key_ptr)) {
            throw std::invalid_argument("Key not found");
        }
        return data_->map[key_ptr]->second;
    }

    size_t size() const noexcept {
        return data_ ? data_->list.size() : 0;
    }

    void clear() noexcept {
        data_.reset();
    }

    class const_iterator {
    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = V;
        using difference_type = std::ptrdiff_t;
        using pointer = const value_type*;
        using reference = const value_type&;

        const_iterator() = default;

        const_iterator(typename std::list<std::pair<std::shared_ptr<K>, V>>::const_iterator it) : it_(it) {}

        reference operator*() const noexcept {
            return it_->second;
        }

        pointer operator->() const noexcept {
            return &(it_->second);
        }

        const_iterator& operator++() noexcept {
            ++it_;
            return *this;
        }

        const_iterator operator++(int) noexcept {
            const_iterator tmp = *this;
            ++(*this);
            return tmp;
        }

        friend bool operator==(const const_iterator& a, const const_iterator& b) noexcept {
            return a.it_ == b.it_;
        }

        friend bool operator!=(const const_iterator& a, const const_iterator& b) noexcept {
            return a.it_ != b.it_;
        }

    private:
        typename std::list<std::pair<std::shared_ptr<K>, V>>::const_iterator it_;
    };

    const_iterator cbegin() const noexcept {
        return const_iterator(data_ ? data_->list.cbegin() : typename std::list<std::pair<std::shared_ptr<K>, V>>::const_iterator());
    }

    const_iterator cend() const noexcept {
        return const_iterator(data_ ? data_->list.cend() : typename std::list<std::pair<std::shared_ptr<K>, V>>::const_iterator());
    }

private:
    bool non_const_read = false;

    struct KeyComparator {
        bool operator()(const std::shared_ptr<K>& lhs, const std::shared_ptr<K>& rhs) const {
            return *lhs < *rhs;
        }
    };

    struct Data {
        std::list<std::pair<std::shared_ptr<K>, V>> list;
        std::map<std::shared_ptr<K>, typename std::list<std::pair<std::shared_ptr<K>, V>>::iterator, KeyComparator> map;
    };

    std::shared_ptr<Data> data_ = std::make_shared<Data>();

    std::shared_ptr<Data> ensure_unique() const {
    if (!data_ || !data_.unique()) {
        auto new_data = std::make_shared<Data>();
        if (data_) {
            for (const auto& item : data_->list) {
                new_data->list.push_back(std::make_pair(item.first, item.second));
            }
            for (auto it = new_data->list.begin(); it != new_data->list.end(); ++it) {
                new_data->map[it->first] = it;
            }
        }
        return new_data;
    }
    return data_;
}

    friend void swap(binder& first, binder& second) noexcept {
        using std::swap;
        swap(first.data_, second.data_);
    }
};

} // namespace cxx

#endif // BINDER_H
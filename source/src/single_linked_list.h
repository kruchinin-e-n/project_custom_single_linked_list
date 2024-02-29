#pragma once
#include <cassert>
#include <cstddef>
#include <iterator>
#include <string>
#include <utility>
#include <algorithm>

template <typename Type>
class SingleLinkedList {

    struct Node {
        Node() = default;
        Node(const Type& val, Node* next)
            : value(val)
            , next_node(next) 
        {
        }
        Type value;
        Node* next_node = nullptr;
    };

    template <typename ValueType>
    class BasicIterator {

    public:

        using iterator_category = std::forward_iterator_tag;
        using value_type = Type;
        using difference_type = std::ptrdiff_t;
        using pointer = ValueType*;
        using reference = ValueType&;

        BasicIterator() = default;

        explicit BasicIterator(Node* node) : node_(node) {}

        BasicIterator(const BasicIterator<Type>& other) noexcept : node_(other.node_) {}

        BasicIterator& operator=(const BasicIterator& rhs) = default;

        [[nodiscard]] bool operator==(const BasicIterator<const Type>& rhs) const noexcept {
            return node_ == rhs.node_;
        }

        [[nodiscard]] bool operator!=(const BasicIterator<const Type>& rhs) const noexcept {
            return !(*this == rhs);
        }

        [[nodiscard]] bool operator==(const BasicIterator<Type>& rhs) const noexcept {
            return node_ == rhs.node_;
        }

        [[nodiscard]] bool operator!=(const BasicIterator<Type>& rhs) const noexcept {
            return !(*this == rhs);
        }

        BasicIterator& operator++() noexcept {

            assert(node_ != nullptr);

            node_ = node_->next_node;
            return *this;
        }

        BasicIterator operator++(int) noexcept {
            BasicIterator before_inc(node_);
            ++(*this);
            return before_inc;
        }

        [[nodiscard]] reference operator*() const noexcept {

            assert(node_ != nullptr);

            return node_->value;
        }

        [[nodiscard]] pointer operator->() const noexcept {

            assert(node_ != nullptr);

            return &node_->value;
        }

    private:
        friend class SingleLinkedList;
        Node* node_ = nullptr;
    };

public:

    using value_type = Type;
    using reference = value_type&;
    using const_reference = const value_type&;
    using Iterator = BasicIterator<Type>;
    using ConstIterator = BasicIterator<const Type>;

    SingleLinkedList() = default;

    SingleLinkedList(std::initializer_list<Type> values) {
        FillLinkedList(values.begin(), values.end());
    }

    SingleLinkedList(const SingleLinkedList& other) {       
        FillLinkedList(other.begin(), other.end());
    }

    void PushFront(const Type& value) {
        head_.next_node = new Node(value, head_.next_node);
        ++size_;
    }

    template<typename It>
    void FillLinkedList(const It begin, const It end) {
        SingleLinkedList<Type> tmp;

        Node * * tmp_node = &tmp.head_.next_node;

        for (auto i = begin; i != end; ++i) {
            * tmp_node = new Node(* i, nullptr);
            tmp_node = & ((* tmp_node) -> next_node);
            ++tmp.size_;
        }
        swap(tmp);
    }

    void PopFront() noexcept {

        assert(size_ != 0 && head_.next_node != nullptr);

        Node* ptr_to_del = head_.next_node;
        head_.next_node = ptr_to_del->next_node;
        delete ptr_to_del;
        --size_;
    }

    Iterator InsertAfter(ConstIterator pos, const Type& value) {

        assert(pos.node_ != nullptr);

        Node* ptr_ins = new Node(value, pos.node_->next_node);
        pos.node_->next_node = ptr_ins;
        ++size_;
        return Iterator(ptr_ins);
    }

    Iterator EraseAfter(ConstIterator pos) noexcept {

        assert(size_ != 0 && pos.node_ != nullptr && pos.node_->next_node != nullptr);

        Node* ptr_to_del = pos.node_->next_node;
        Node* ptr_new_after = ptr_to_del->next_node;
        pos.node_->next_node = ptr_new_after;
        delete ptr_to_del;
        --size_;
        return Iterator(ptr_new_after);
    }
 
    SingleLinkedList& operator=(const SingleLinkedList& rhs) {

        //assert(this != &rhs);

        if (this == &rhs) {
            return *this;
        }

        FillLinkedList(rhs.begin(), rhs.end());
        return *this;
    }
    
    void swap(SingleLinkedList& other) noexcept {
 
        std::swap(head_.next_node, other.head_.next_node);
        std::swap(size_, other.size_);
    }

    ~SingleLinkedList() {
        Clear();
    }

    [[nodiscard]] size_t GetSize() const noexcept {
        return size_;
    }

    [[nodiscard]] bool IsEmpty() const noexcept {
        return (!size_);
    }

    void Clear() noexcept {

        while (head_.next_node != nullptr) {
            Node* ptr_to_del = head_.next_node;
            head_.next_node = head_.next_node->next_node;
            delete ptr_to_del;
            --size_;
        }
    }

    [[nodiscard]] Iterator begin() noexcept {
        return Iterator(head_.next_node);
    }

    [[nodiscard]] Iterator end() noexcept {
        return Iterator(nullptr);
    }

    [[nodiscard]] ConstIterator cbegin() const noexcept {
        return ConstIterator(head_.next_node);
    }

    [[nodiscard]] ConstIterator cend() const noexcept {
        return ConstIterator(nullptr);
    }

    [[nodiscard]] ConstIterator begin() const noexcept {
        return cbegin();
    }

    [[nodiscard]] ConstIterator end() const noexcept {
        return cend();
    }

    [[nodiscard]] Iterator before_begin() noexcept {
        return Iterator(&head_);
    }

    [[nodiscard]] ConstIterator before_begin() const noexcept {
        return ConstIterator{ const_cast<Node*>(&head_) };
    }

    [[nodiscard]] ConstIterator cbefore_begin() const noexcept {
        return ConstIterator{ const_cast<Node*>(&head_) };
    }

private:

    Node head_;
    size_t size_ = 0;
};

template <typename Type>
void swap(SingleLinkedList<Type>& lhs, SingleLinkedList<Type>& rhs) noexcept {
    lhs.swap(rhs);
}

template <typename Type>
bool operator==(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return std::equal(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template <typename Type>
bool operator!=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return !(lhs == rhs);
}

template <typename Type>
bool operator<(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template<typename Type>
[[nodiscard]] bool operator>(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) noexcept {       
    return rhs < lhs;
}

template<typename Type>
[[nodiscard]] bool operator>=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) noexcept {
    return !(lhs < rhs);
}

template<typename Type>
[[nodiscard]] bool operator<=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) noexcept {
    return !(rhs < lhs);
}
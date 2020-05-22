#include <iostream>
#include <optional>
#include <utility>
#include <algorithm>
#include <list>
#include <cassert>
#include <any>
#include <sstream>

template <typename T>
class ListElement {
public:
    explicit ListElement (
        ListElement<T>* prev,
        ListElement<T>* next,
        T&& value
    ) {
        prev_ = prev;
        next_ = next;
        value_ = std::move(value);
    }
    explicit ListElement (
        ListElement<T>* prev,
        ListElement<T>* next,
        const T& value
    ) {
        prev_ = prev;
        next_ = next;
        value_ = value;
    }
    explicit ListElement (
        ListElement<T>* prev,
        ListElement<T>* next
    ) {
        prev_ = prev;
        next_ = next;
    }
    ~ListElement() = default;
    ListElement<T>* prev_;
    ListElement<T>* next_;
    std::optional<T> value_;
};


template <typename T>
class List{
public:
    typedef T value_type;

    class ConstListIterator : public std::iterator <std::bidirectional_iterator_tag, T> {

        friend class List<T>;

    public:

        ConstListIterator (const ConstListIterator &it);

        ConstListIterator (ListElement<T>* element);

        bool operator != (ConstListIterator const& other) const;

        bool operator == (ConstListIterator const& other) const;

        T& operator * ();

        ConstListIterator& operator ++();

        ConstListIterator& operator --();

        ConstListIterator operator ++ (int);

        ConstListIterator operator -- (int);

    private:

        ListElement<T>* getListElement () const {
            return element_;
        }

        ListElement<T>* element_;
    };

    class ListIterator : public std::iterator <std::bidirectional_iterator_tag, T> {

        friend class List<T>;

    public:

        typedef T value_type;

        ListIterator (const ListIterator &it);

        bool operator != (ListIterator const& other) const;

        bool operator == (ListIterator const& other) const;

        T& operator * ();

        ListIterator& operator ++ ();

        ListIterator& operator -- ();

        ListIterator operator ++ (int);

        ListIterator operator -- (int);

        operator ConstListIterator() {
            return ConstListIterator(element_);
        }

    private:

        ListIterator (ListElement<T>* element);

        ListElement<T>* getListElement () const {
            return element_;
        }

        ListElement<T>* element_;
    };

    typedef ListIterator iterator;

    explicit List ();

    List(const List &other);

    List(List &&other) noexcept;

    List& operator = (const List &other);

    List& operator = (List &&other) noexcept;

    ~List();

    explicit List (size_t count, const T& value = T());

    size_t size ();

    T& front ();

    T& back ();

    void clear ();

    bool empty();

    void push_front (T&& value);

    void push_front (const T& value);

    void pop_front ();

    void push_back (T&& value) ;

    void push_back (const T& value);

    ListIterator insert (ListIterator it, T&& value);

    ListIterator insert (ListIterator it, const T& value);

    template <typename ... Args>
    ListIterator emplace(ConstListIterator it, Args&& ... args) {

        ListElement<T>* tmp = it.getListElement();
        auto new_element = new ListElement<T>(nullptr, nullptr, T(std::forward<Args ...>(args...)));

        if (size_ == 0) {
            first_ = new_element;
            last_ = new_element;
            first_->next_ = end_fictive_element_;
            end_fictive_element_->prev_ = first_;
        } else if (tmp == end_fictive_element_) {
            new_element->prev_ = last_;
            new_element->next_ = end_fictive_element_;
            end_fictive_element_->prev_ = new_element;
            last_->next_ = new_element;
            last_ = new_element;
        } else if (tmp == first_) {
            new_element->next_ = first_;
            first_->prev_ = new_element;
            first_ = new_element;
        } else {
            new_element->next_ = tmp;
            new_element->prev_ = tmp->prev_;
            tmp->prev_->next_ = new_element;
            tmp->prev_ = new_element;
        }
        size_++;
        return ListIterator(new_element);
    }

    template <typename ... Args>
    void emplace_back(Args&& ... args) {
        ++size_;
        ListElement<T>* tmp = last_;
        if (first_ != nullptr) {
            last_= new ListElement<T>(tmp, end_fictive_element_, T(std::forward<Args ...>(args...)));
            tmp->next_ = last_;
        } else {
            first_= new ListElement<T>(nullptr, end_fictive_element_, T(std::forward<Args ...>(args...)));
            last_ = first_;
        }
    }

    template <typename ... Args>
    void emplace_front(Args&& ... args) {
        ++size_;
        ListElement<T>* tmp = first_;
        if (tmp != nullptr) {
            first_= new ListElement<T>(nullptr, tmp, T(std::forward<Args ...>(args...)));
            tmp->prev_ = first_;
        } else {
            first_= new ListElement<T>(nullptr, end_fictive_element_, T(std::forward<Args ...>(args...)));
            last_ = first_;
        }
    }

    void pop_back ();

    void reverse();

    void erase (ListIterator it);

    void erase (ListIterator it_start, ListIterator it_end);

    void unique();

    ListIterator end() {
        return ListIterator(end_fictive_element_);
    }

    ListIterator begin() {
        if (size_ == 0) {
            return end();
        }
        return ListIterator(first_);
    }

    ConstListIterator cend() const {
        return ConstListIterator(end_fictive_element_);
    }

    ConstListIterator cbegin() const {
        if (size_ == 0) {
            return cend();
        }
        return ConstListIterator(first_);
    }

    std::reverse_iterator<ListIterator> rend() {
        return std::reverse_iterator<ListIterator>(begin());
    }

    std::reverse_iterator<ListIterator> rbegin() {
        return std::reverse_iterator<ListIterator>(end());
    }

    std::reverse_iterator<ConstListIterator> crend() {
        return std::reverse_iterator<ConstListIterator>(begin());
    }

    std::reverse_iterator<ConstListIterator> crbegin() {
        return std::reverse_iterator<ConstListIterator>(end());
    }

private:
    ListElement<T>* first_;
    ListElement<T>* last_;
    ListElement<T>* end_fictive_element_;
    size_t size_;

    void removeElement(ListElement<T>* element);

};

/* List methods */

template <typename T>
List<T>::List () {
    first_ = nullptr;
    last_ = nullptr;
    end_fictive_element_ = new ListElement<T>(nullptr, nullptr);
    end_fictive_element_->prev_ = last_;
    size_ = 0;
}

template <typename T>
List<T>::List(const List &other) {
    first_ = nullptr;
    last_ = nullptr;
    size_ = 0;
    end_fictive_element_ = new ListElement<T>(nullptr, nullptr);
    for (auto it = other.cbegin(); it != other.cend(); ++it) {
        push_back(*it);
    }
    if (last_ != nullptr) {
        last_->next_ = end_fictive_element_;
        end_fictive_element_->prev_ = last_;
    }
}

template <typename T>
List<T>::List(List &&other) noexcept {
    first_ = other.first_;
    last_ = other.last_;
    size_ = other.size_;
    end_fictive_element_ = new ListElement<T>(nullptr, nullptr);
    if (last_ != nullptr) {
        last_->next_ = end_fictive_element_;
        end_fictive_element_->prev_ = last_;
    }
    other.first_ = nullptr;
    other.last_ = nullptr;
    other.size_ = 0;
}

template <typename T>
List<T>& List<T>::operator = (const List &other) {
    if (this != &other) {
        clear();
        size_ = 0;
        for (auto it = other.cbegin(); it != other.cend(); ++it) {
            push_back(*it);
        }
        if (!end_fictive_element_) {
            end_fictive_element_ = new ListElement<T>(nullptr, nullptr);
        }
        if (last_ != nullptr) {
            last_->next_ = end_fictive_element_;
            end_fictive_element_->prev_ = last_;
        }
    }
    return *this;
}

template <typename T>
List<T>& List<T>::operator = (List &&other) noexcept {
    if (this != &other) {
        first_ = other.first_;
        last_ = other.last_;
        size_ = other.size_;
        if (!end_fictive_element_) {
            end_fictive_element_ = new ListElement<T>(nullptr, nullptr);
        }
        if (last_ != nullptr) {
            last_->next_ = end_fictive_element_;
            end_fictive_element_->prev_ = last_;
        }
        other.first_ = nullptr;
        other.last_ = nullptr;
        other.size_ = 0;
    }
    return *this;
}

template <typename T>
List<T>::~List(){
    clear();
    delete end_fictive_element_;
}

template <typename T>
List<T>::List(size_t count, const T& value) : List() {
    for (size_t i = 0; i < count; ++i) {
        push_back(value);
    }
}

template <typename T>
size_t List<T>::size () {
    return size_;
}

template <typename T>
T& List<T>::front () {
    return *first_->value_;
}

template <typename T>
T& List<T>::back () {
    return *last_->value_;
}

template <typename T>
void List<T>::clear() {
    if (size_ > 0) {
        auto current = first_;
        while (current != end_fictive_element_ && current != nullptr) {
            auto tmp = current->next_;
            delete current;
            current = tmp;
        }
        first_ = nullptr;
        last_ = nullptr;
        size_ = 0;
    } else {
        first_ = nullptr;
        last_ = nullptr;
        size_ = 0;
    }
    end_fictive_element_->prev_ = nullptr;
}

template <typename T>
bool List<T>::empty () {
    return size_ == 0;
}

template <typename T>
void List<T>::push_back (T&&  value) {
    last_ = new ListElement<T>(last_, nullptr, std::forward<T>(value));
    if (first_ == nullptr) {
        first_ = last_;
    } else {
        last_->prev_->next_ = last_;
    }
    last_->next_ = end_fictive_element_;
    end_fictive_element_->prev_ = last_;
    ++size_;
}

template <typename T>
void List<T>::push_back (const T&  value) {
    last_ = new ListElement<T>(last_, nullptr, value);
    if (first_ == nullptr) {
        first_ = last_;
    } else {
        last_->prev_->next_ = last_;
    }
    last_->next_ = end_fictive_element_;
    end_fictive_element_->prev_ = last_;
    ++size_;
}

template <typename T>
void List<T>::push_front (T&& value) {
    if (first_ == nullptr) {
        first_ = new ListElement<T>(nullptr, first_, std::forward<T>(value));
        last_ = first_;
        last_->next_ = end_fictive_element_;
    } else {
        first_ = new ListElement<T>(nullptr, first_, std::forward<T>(value));
        first_->next_->prev_ = first_;
    }
    ++size_;
}

template <typename T>
void List<T>::push_front (const T& value) {
    if (first_ == nullptr) {
        first_ = new ListElement<T>(nullptr, first_, value);
        last_ = first_;
        last_->next_ = end_fictive_element_;
    } else {
        first_ = new ListElement<T>(nullptr, first_, value);
        first_->next_->prev_ = first_;
    }
    ++size_;
}

template <typename T>
void List<T>::pop_back () {
    if (size_ == 2) {
        delete last_;
        last_ = first_;
        last_->next_ = end_fictive_element_;
        end_fictive_element_->prev_ = last_;
        --size_;
    } else if (size_ > 2) {
        auto last_prev = last_->prev_;
        delete last_;
        last_ = last_prev;
        last_prev->next_ = end_fictive_element_;
        end_fictive_element_->prev_ = last_;
        --size_;
    } else {
        delete first_;
        first_ = nullptr;
        last_ = nullptr;
        --size_;
    }
}

template <typename T>
void List<T>::pop_front () {
    if (size_ > 1) {
        auto first_next = first_->next_;
        delete first_;
        first_ = first_next;
        --size_;
    } else {
        delete first_;
        first_ = nullptr;
        last_ = nullptr;
        --size_;
    }
}

template <typename T>
void List<T>::reverse() {
    auto elements = new ListElement<T>*[size_];
    ListElement<T>* current = first_;
    size_t i = 0;
    while (current != end_fictive_element_) {
        elements[i] = current;
        ++i;
        current = current->next_;
    }
    first_ = elements[size_ - 1];
    last_ = first_;
    for (i = size_ - 1; i > 0; --i ) {
        ListElement<T>* tmp = last_;
        last_ = elements[i - 1];
        last_->prev_ = tmp;
        tmp->next_ = last_;
    }
    last_->next_ = end_fictive_element_;
    end_fictive_element_->prev_ = last_;
    delete [] elements;
}

template <typename T>
void List<T>::erase (ListIterator it) {
    ListElement<T>* tmp = it.getListElement();
    removeElement(tmp);
}

template <typename T>
void List<T>::erase (ListIterator it_start, ListIterator it_end) {
    while (it_start != it_end) {
        ListElement<T>* tmp = it_start.getListElement();
        ++it_start;
        removeElement(tmp);
    }
}

template <typename T>
typename List<T>::ListIterator List<T>::insert (ListIterator it, T&& value) {
    ListElement<T>* tmp = it.getListElement();
    auto new_element = new ListElement<T>(nullptr, nullptr, std::forward<T>(value));
    if (size_ == 0) {
        first_ = new_element;
        last_ = new_element;
        first_->next_ = end_fictive_element_;
        end_fictive_element_->prev_ = first_;
    } else if (tmp == end_fictive_element_) {
        new_element->prev_ = last_;
        new_element->next_ = end_fictive_element_;
        end_fictive_element_->prev_ = new_element;
        last_->next_ = new_element;
        last_ = new_element;
    } else if (tmp == first_) {
        new_element->next_ = first_;
        first_->prev_ = new_element;
        first_ = new_element;
    } else {
        new_element->next_ = tmp;
        new_element->prev_ = tmp->prev_;
        tmp->prev_->next_ = new_element;
        tmp->prev_ = new_element;
    }
    size_++;
    return ListIterator(new_element);
}

template <typename T>
typename List<T>::ListIterator List<T>::insert (ListIterator it, const T& value) {
    ListElement<T>* tmp = it.getListElement();
    auto new_element = new ListElement<T>(nullptr, nullptr, value);
    if (size_ == 0) {
        first_ = new_element;
        last_ = new_element;
        first_->next_ = end_fictive_element_;
        end_fictive_element_->prev_ = first_;
    } else if (tmp == end_fictive_element_) {
        new_element->prev_ = last_;
        new_element->next_ = end_fictive_element_;
        end_fictive_element_->prev_ = new_element;
        last_->next_ = new_element;
        last_ = new_element;
    } else {
        new_element->next_ = tmp;
        new_element->prev_ = tmp->prev_;
        tmp->prev_->next_ = new_element;
        tmp->prev_ = new_element;
        if (tmp == last_) {
            last_ = new_element;
        }
    }
    size_++;
    return ListIterator(new_element);
}

template <typename T>
void List<T>::removeElement(ListElement<T>* element) {
    if (element == first_) {
        first_ = element->next_;
        first_->prev_ = nullptr;
    } else {
        element->prev_->next_ = element->next_;
        element->next_->prev_ = element->prev_;
    }
    --size_;
    if (size_ == 1) {
        last_ = first_;

    }
    delete element;
}

template <typename T>
void List<T>::unique () {
    if (size_ > 1) {
        ListElement<T>* current = first_;
        while (current != last_) {
            if (*current->value_ == *current->next_->value_) {
                ListElement<T>* tmp = current->next_;
                current->next_ = tmp->next_;
                tmp->next_->prev_ = current;
                if (tmp == last_) {
                    last_ = current;
                }
                delete tmp;
                --size_;
            } else {
                current = current->next_;
            }
        }
    }
}

/* iterator methods */

template <typename T>
List<T>::ListIterator::ListIterator (const ListIterator &it) : element_(it.element_) { }

template <typename T>
List<T>::ListIterator::ListIterator (ListElement<T>* element) {
    element_ = element;
}

template <typename T>
typename List<T>::ListIterator& List<T>::ListIterator::operator ++ () {
    element_ = element_->next_;
    return *this;
}

template <typename T>
typename List<T>::ListIterator& List<T>::ListIterator::operator -- () {
    element_ = element_->prev_;
    return *this;
}

template <typename T>
typename List<T>::ListIterator List<T>::ListIterator::operator ++ (int) {
    ListIterator res = ListIterator(element_);
    element_ = element_->next_;
    return res;
}

template <typename T>
typename List<T>::ListIterator List<T>::ListIterator::operator -- (int) {
    ListIterator res = ListIterator(element_);
    element_ = element_->prev_;
    return res;
}

template <typename T>
T& List<T>::ListIterator::operator * () {
    return *element_->value_;
}

template <typename T>
bool List<T>::ListIterator::operator != (const typename List<T>::ListIterator& other) const {
    return element_ != other.element_;
}

template <typename T>
bool List<T>::ListIterator::operator == (const typename List<T>::ListIterator& other) const {
    return element_ == other.element_;
}

/* const iterator methods */

template <typename T>
List<T>::ConstListIterator::ConstListIterator (const ConstListIterator &it) : element_(it.element_) { }

template <typename T>
List<T>::ConstListIterator::ConstListIterator (ListElement<T>* element) {
    element_ = element;
}

template <typename T>
typename List<T>::ConstListIterator& List<T>::ConstListIterator::operator ++ () {
    element_ = element_->next_;
    return *this;
}

template <typename T>
typename List<T>::ConstListIterator List<T>::ConstListIterator::operator ++ (int) {
    ListIterator res = ListIterator(element_);
    element_ = element_->next_;
    return res;
}

template <typename T>
typename List<T>::ConstListIterator& List<T>::ConstListIterator::operator -- () {
    element_ = element_->prev_;
    return *this;
}

template <typename T>
T& List<T>::ConstListIterator::operator * () {
    return *element_->value_;
}

template <typename T>
bool List<T>::ConstListIterator::operator != (const typename List<T>::ConstListIterator& other) const {
    return element_ != other.element_;
}

template <typename T>
bool List<T>::ConstListIterator::operator == (const typename List<T>::ConstListIterator& other) const {
    return element_ == other.element_;
}

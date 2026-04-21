#ifndef SRC_HPP
#define SRC_HPP
#include <iostream>

struct Permutation {
    int* mapping;
    size_t size;

    Permutation(size_t size) {
        this->size = size;
        mapping = size ? new int[size] : nullptr;
        for (size_t i = 0; i < size; ++i) mapping[i] = static_cast<int>(i);
    }

    Permutation(const int* mapping, size_t size) {
        this->size = size;
        this->mapping = size ? new int[size] : nullptr;
        for (size_t i = 0; i < size; ++i) this->mapping[i] = mapping[i];
    }

    ~Permutation() {
        delete[] mapping;
    }

    void apply(int* permutation) const {
        if (size == 0) return;
        int* tmp = new int[size];
        for (size_t i = 0; i < size; ++i) tmp[i] = permutation[i];
        for (size_t i = 0; i < size; ++i) permutation[i] = tmp[mapping[i]];
        delete[] tmp;
    }

    Permutation operator*(const Permutation& other) const {
        Permutation r(size);
        for (size_t i = 0; i < size; ++i) r.mapping[i] = other.mapping[mapping[i]];
        return r;
    }

    Permutation inverse() const {
        Permutation inv(size);
        for (size_t i = 0; i < size; ++i) inv.mapping[mapping[i]] = static_cast<int>(i);
        return inv;
    }

    friend std::ostream& operator<<(std::ostream& os, const Permutation& p) {
        os.put('[');
        for (size_t i = 0; i < p.size; ++i) {
            os << p.mapping[i];
            if (i < p.size - 1) os.put(' ');
        }
        os.put(']');
        return os;
    }
};

struct Transposition {
    int a, b;

    Transposition() { a = 0; b = 0; }
    Transposition(int a, int b)  { this->a = a; this->b = b; }

    void apply(int* permutation, size_t /*size*/) const {
        if (a == b) return;
        int tmp = permutation[a];
        permutation[a] = permutation[b];
        permutation[b] = tmp;
    }

    friend std::ostream& operator<<(std::ostream& os, const Transposition& t) {
        os.put('{'); os << t.a; os.put(' '); os << t.b; os.put('}');
        return os;
    }

    Permutation toPermutation(size_t size) const {
        Permutation p(size);
        if (a != b) {
            int tmp = p.mapping[a];
            p.mapping[a] = p.mapping[b];
            p.mapping[b] = tmp;
        }
        return p;
    }
};

struct Cycle {
    int* elements;
    size_t size;

    Cycle(const int* elements, size_t size) {
        this->size = size;
        this->elements = size ? new int[size] : nullptr;
        for (size_t i = 0; i < size; ++i) this->elements[i] = elements[i];
    }

    ~Cycle() {
        delete[] elements;
    }

    void apply(int* permutation, size_t /*n*/) const {
        if (size == 0) return;
        int first_idx = elements[0];
        int first_val = permutation[first_idx];
        for (size_t j = 0; j + 1 < size; ++j) {
            int from_idx = elements[j + 1];
            int to_idx = elements[j];
            permutation[to_idx] = permutation[from_idx];
        }
        int last_idx = elements[size - 1];
        permutation[last_idx] = first_val;
    }

    friend std::ostream& operator<<(std::ostream& os, const Cycle& c) {
        os.put('{');
        for (size_t i = 0; i < c.size; ++i) {
            os << c.elements[i];
            if (i < c.size - 1) os.put(' ');
        }
        os.put('}');
        return os;
    }

    Permutation toPermutation(size_t n) const {
        Permutation p(n);
        if (size == 0) return p;
        for (size_t j = 0; j < size; ++j) {
            int i = elements[j];
            int nxt = elements[(j + 1) % size];
            p.mapping[i] = nxt;
        }
        return p;
    }
};


#endif

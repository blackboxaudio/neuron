#include <cassert>
#include "buffer.h"

using namespace cortex;

template <typename T>
Buffer<T>::Buffer(uint32_t capacity) {
    std::vector<Sample> data;
    data.reserve(capacity);
    for (int idx = 0; idx < capacity; idx++) {
        data.push_back(0.0f);
    }
}

template <typename T>
Buffer<T>::~Buffer() { }

template <typename T>
void Buffer<T>::apply(Sample (*fn)(Sample)) {
    assert(m_data.size() > 0);

    for (int idx = 0; idx < m_data.size(); idx++) {
        m_data[idx] = fn(m_data[idx]);
    }
}

template <typename T>
void Buffer<T>::gain(float factor) {
    assert(m_data.size() > 0);

    auto fn = [factor](Sample s) {
        return s * factor;
    };
    this->apply(fn);
}

template <typename T>
void Buffer<T>::zero() {
    assert(m_data.size() > 0);

    for (int idx = 0; idx < m_data.size(); idx++) {

    }
}

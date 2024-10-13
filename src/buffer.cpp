#include <cassert>
#include "float.h"
#include "math.h"
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
void Buffer<T>::zero() {
    assert(!m_data.empty());

    for (float & data : m_data) {
        data = ZERO;
    }
}

template <typename T>
void Buffer<T>::apply(Sample (*fn)(Sample)) {
    assert(!m_data.empty());

    for (float& data : m_data) {
        data = fn(data);
    }
}

template <typename T>
void Buffer<T>::gain(float factor) {
    assert(!m_data.empty());

    for (float& data : m_data) {
        data *= clamp(factor, 0.0f, 1.0f);
    }
}

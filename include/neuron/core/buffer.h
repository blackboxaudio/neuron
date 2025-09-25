#pragma once

/**
 * A lightweight view into a contiguous block of data, acting
 * as a non-owning reference to memory.
 */
namespace neuron {

    template<typename DataType>
    class Buffer {
    public:
        constexpr Buffer() noexcept : m_data(nullptr), m_size(0) {}
        constexpr Buffer(DataType* data, int size) noexcept : m_data(data), m_size(size) {}

        constexpr DataType& operator[](int index) noexcept { return m_data[index]; }
        constexpr const DataType& operator[](int index) const noexcept { return m_data[index]; }

        constexpr int size() const noexcept { return m_size; }
        constexpr DataType* data() noexcept { return m_data; }
        constexpr const DataType* data() const noexcept { return m_data; }

        // Iterator support for range-based loops
        constexpr DataType* begin() noexcept { return m_data; }
        constexpr DataType* end() noexcept { return m_data + m_size; }
        constexpr const DataType* begin() const noexcept { return m_data; }
        constexpr const DataType* end() const noexcept { return m_data + m_size; }

        constexpr bool empty() const noexcept { return m_size == 0; }

    private:
        DataType* m_data;
        int m_size;

    };

}

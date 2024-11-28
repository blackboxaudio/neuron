#pragma once

#ifdef NEO_USE_STD_ATOMIC
#include <atomic>
#endif

namespace neuron {

#ifdef NEO_USE_STD_ATOMIC
    template<typename T>
    class Parameter {
    public:
        Parameter() = default;

        explicit Parameter(T value)
        {
            std::atomic_init(m_parameter, value);
        }

        explicit Parameter(std::atomic<T>* ptr)
        {
            m_parameter = ptr;
        }

        ~Parameter()
        {
            m_parameter = nullptr;
        }

        // Attach a parameter (e.g. via an atomic float pointer)
        void AttachSource(std::atomic<T>* ptr)
        {
            m_parameter = ptr;
        }

        operator T() const
        {
            return m_parameter->load();
        }

        Parameter& operator=(T value)
        {
            m_parameter->store(value);
            return *this;
        }

        T operator+(T value) const
        {
            return m_parameter->load() + value;
        }

        T operator-(T value) const
        {
            return m_parameter->load() - value;
        }

        T operator*(T value) const
        {
            return m_parameter->load() * value;
        }

        T operator/(T value) const
        {
            if (value == 0.0) {
                return value;
            } else {
                return m_parameter->load() / value;
            }
        }

    private:
        /**
        * CAUTION: This empty value is used as a safe initializer for the pointer,
        * which is what is used by the JUCE library.
        */
        std::atomic<T> m_value {0};
        std::atomic<T>* m_parameter = &m_value;
    };

#else
    template<typename T>
    class Parameter {
    public:
        explicit Parameter(T value = 0.0)
        {
            m_parameter = value;
        }

        ~Parameter() = default;

        operator float() const
        {
            return m_parameter;
        }

        Parameter& operator=(T value) const
        {
            m_parameter = value;
            return *this;
        }

        T operator+(T value) const
        {
            return m_parameter + value;
        }

        T operator-(T value) const
        {
            return m_parameter - value;
        }

        T operator*(T value) const
        {
            return m_parameter * value;
        }

        T operator/(T value) const
        {
            if (value == 0.0) {
                return value;
            } else {
                return m_parameter / value;
            }
        }

    private:
        T m_parameter;
    };
#endif

}

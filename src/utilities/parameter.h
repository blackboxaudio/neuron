#pragma once

#ifdef NEO_ENABLE_PLUGIN_SUPPORT
#include <atomic>
#endif

namespace neuron {
#ifdef NEO_ENABLE_PLUGIN_SUPPORT
    template<typename T>
    class Parameter {
    public:
        Parameter(std::atomic<T>* ptr = nullptr)
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

        T operator=(T value) const
        {
            m_parameter->store(value);
            return value;
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
        std::atomic<T>* m_parameter = nullptr;
    };
#else
    template<typename T>
    class Parameter {
    public:
        Parameter(T value = 0.0)
        {
            m_parameter = value;
        }

        ~Parameter() = default;

        operator float() const
        {
            return m_parameter;
        }

        T operator=(T value) const
        {
            m_parameter = value;
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

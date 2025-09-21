#pragma once

#ifdef NEO_PLUGIN_SUPPORT
#include <atomic>
#endif

namespace neuron {

#ifdef NEO_PLUGIN_SUPPORT
    /**
     * A read-only parameter used by a DSP component to allow more
     * control and flexibility in shaping its sound.
     */
    template<typename T>
    class Parameter {
    public:
        explicit Parameter(T value)
        {
            m_parameter->store(value);
        }

        explicit Parameter(std::atomic<T>* source)
        {
            m_parameter = source;
        }

        ~Parameter()
        {
            m_parameter = nullptr;
        }

        /**
         * Attaches a new source for this parameter to read data from.
         *
         * CAUTION: If this method is called, the corresponding DSP component's
         * setter method for this parameter will no longer update the variable.
         *
         * @param source The new pointer that this parameter will read from and write to.
         */
        void AttachSource(std::atomic<T>* source)
        {
            m_parameter = source;
        }

        operator T() const
        {
            return m_parameter->load();
        }

        Parameter& operator=(T /* value */)
        {
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
            if (value == 0.0f) {
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
        std::atomic<T> m_initial_source { 0.0f };
        std::atomic<T>* m_parameter = &m_initial_source;
    };

#else
    /**
     * An adjustable parameter used by a DSP component to allow more
     * control and flexibility in shaping its sound.
     */
    template<typename T>
    class Parameter {
    public:
        explicit Parameter(T value = 0.0f)
        {
            m_parameter = value;
        }

        ~Parameter() = default;

        operator float() const
        {
            return m_parameter;
        }

        Parameter& operator=(T value)
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

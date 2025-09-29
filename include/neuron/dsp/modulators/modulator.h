#pragma once

#include "neuron/core/buffer.h"

#include <algorithm>

namespace neuron {

    /**
     * Describes a neuron that produces a stream of data that changes the parameter of another
     * neuron over time.
     */
    template<class M>
    class Modulator {
    public:
        /**
         * Initializes the modulator with a zero-filled buffer.
         */
        Modulator()
            : m_modulationValues{}
            , m_bufferView(m_modulationValues, 0)
        {

        }

        /**
         * Frees any memory allocated by the modulator.
         */
        ~Modulator() = default;

        /**
         * Computes the modulation values, preparing them to be read
         * by another neuron in the future.
         */
        void Modulate() noexcept
        {
            return static_cast<M*>(this)->ModulateImpl();
        }

        /**
         * Exposes this modulator's internal values with a view-only buffer.
         */
        const Buffer<float>& GetModulationValues() const noexcept
        {
            return m_bufferView;
        }

    protected:
        /**
         * Sets the actual size of this modulator's internal buffer. It cannot exceed
         * the maximum number of values, which is 4096.
         */
        void SetBufferSize(int size) noexcept
        {
            m_bufferView = Buffer<float>(m_modulationValues, std::min(size, MAX_BUFFER_SIZE));
        }

        static constexpr int MAX_BUFFER_SIZE = 4096;
        float m_modulationValues[MAX_BUFFER_SIZE];

    private:
        Buffer<float> m_bufferView;
    };

    /**
     * Provides a way of using modulators inside of neurons without needing to know
     * the exact type of modulator at compile time. This class uses a type-erasure
     * technique to do so.
     */
    class ModulationSource {
    public:
        /**
         * Initializes a modulation source unattached to any modulator.
         */
        ModulationSource() = default;

        /**
         * Initializes a modulation source with a given modulator.
         */
        template<class M>
        ModulationSource(Modulator<M>* modulator)
            : m_ptr(modulator)
            , m_get_modulation_values_fn([](const void* ptr) noexcept -> const Buffer<float>& {
                return static_cast<const Modulator<M>*>(ptr)->GetModulationValues();
            })
        {}

        ModulationSource(const ModulationSource&) = default;
        ModulationSource& operator=(const ModulationSource&) = default;

        /**
         * Checks whether this modulation source has been initialized with a modulator
         * or not.
         */
        bool IsValid() const noexcept { return m_ptr != nullptr; }

        /**
         * Exposes this modulator's internal values with a view-only buffer.
         */
        const Buffer<float>& GetModulationValues() const noexcept
        {
            if (m_ptr && m_get_modulation_values_fn) {
                return m_get_modulation_values_fn(m_ptr);
            }

            static constexpr Buffer<float> empty;
            return empty;
        }

        /**
         * Detach the underlying modulator from this modulation source.
         */
        void Detach() noexcept
        {
            m_ptr = nullptr;
            m_get_modulation_values_fn = nullptr;
        }

    private:
        void* m_ptr = nullptr;
        const Buffer<float>& (*m_get_modulation_values_fn)(const void*) noexcept = nullptr;
    };

}

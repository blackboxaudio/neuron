#pragma once

#include "neuron/core/buffer.h"

#include <algorithm>

namespace neuron {

    /**
     * Describes a DSP component that produces a stream of data
     * that changes the parameter of another DSP component over time.
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
         * Creates a modulation value to be used elsewhere.
         *
         * @return float
         */
        void GenerateModulationValues() noexcept
        {
            return static_cast<M*>(this)->GenerateModulationValuesImpl();
        }

        Buffer<float>& GetModulationValues() noexcept
        {
            return m_bufferView;
        }

        void SetBufferSize(int size) noexcept
        {
            m_bufferSize = std::min(size, MAX_BUFFER_SIZE);
            m_bufferView = Buffer<float>(m_modulationValues, m_bufferSize);
        }

    protected:
        static constexpr int MAX_BUFFER_SIZE = 4096;
        float m_modulationValues[MAX_BUFFER_SIZE];
        int m_bufferSize = 0;

    private:
        Buffer<float> m_bufferView;
    };

    class ModulationSource {
    public:
        ModulationSource() = default;

        template<class M>
        ModulationSource(Modulator<M>* modulator)
            : m_get_modulation_values_fn([](const void* ptr) -> const float* {
                return static_cast<const Modulator<M>*>(ptr)->GetModulationValues();
            })
            , m_ptr(modulator)
        {}

        ModulationSource(const ModulationSource&) = default;
        ModulationSource& operator=(const ModulationSource&) = default;

        bool IsValid() const noexcept { return m_ptr != nullptr; }

        const Buffer<float>& GetModulationValues() const noexcept
        {
            if (m_ptr && m_get_modulation_values_fn) {
                return m_get_modulation_values_fn(m_ptr);
            }

            static constexpr Buffer<float> empty;
            return empty;
        }

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

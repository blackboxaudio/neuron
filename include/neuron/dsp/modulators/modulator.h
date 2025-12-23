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
        Modulator() {}

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

        const float GetModulationValue() const noexcept
        {
            return m_modulationValue;
        }

    protected:
        float m_modulationValue = 0.0f;
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
            , m_get_modulation_value_fn([](const void* ptr) noexcept -> const float {
                return static_cast<const Modulator<M>*>(ptr)->GetModulationValue();
            })
        {
        }

        ModulationSource(const ModulationSource&) = default;
        ModulationSource& operator=(const ModulationSource&) = default;

        /**
         * Checks whether this modulation source has been initialized with a modulator
         * or not.
         */
        bool IsValid() const noexcept { return m_ptr != nullptr; }

        const float GetModulationValue() const noexcept
        {
            if (m_ptr && m_get_modulation_value_fn) {
                return m_get_modulation_value_fn(m_ptr);
            }

            return 0.0f;
        }

        /**
         * Detach the underlying modulator from this modulation source.
         */
        void Detach() noexcept
        {
            m_ptr = nullptr;
            m_get_modulation_value_fn = nullptr;
        }

    private:
        void* m_ptr = nullptr;
        const float (*m_get_modulation_value_fn)(const void*) noexcept = nullptr;
    };

}

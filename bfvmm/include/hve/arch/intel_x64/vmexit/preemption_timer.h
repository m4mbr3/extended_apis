//
// Copyright (C) 2019 Assured Information Security, Inc.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#ifndef PREEMPTION_TIMER_INTEL_X64_EAPIS_H
#define PREEMPTION_TIMER_INTEL_X64_EAPIS_H

#include <list>

#include <bfvmm/hve/arch/intel_x64/vmcs.h>
#include <bfvmm/hve/arch/intel_x64/exit_handler.h>

// -----------------------------------------------------------------------------
// Exports
// -----------------------------------------------------------------------------

#include <bfexports.h>

#ifndef STATIC_EAPIS_HVE
#ifdef SHARED_EAPIS_HVE
#define EXPORT_EAPIS_HVE EXPORT_SYM
#else
#define EXPORT_EAPIS_HVE IMPORT_SYM
#endif
#else
#define EXPORT_EAPIS_HVE
#endif

// -----------------------------------------------------------------------------
// Definitions
// -----------------------------------------------------------------------------

namespace eapis::intel_x64
{

class vcpu;

/// VMX Preemption Timer
///
/// Provides an interface for registering handlers for VMX-preemption timer
/// exits.
///
class EXPORT_EAPIS_HVE preemption_timer_handler
{
public:

    using value_t = uint64_t;           ///< Timer value type

    /// Handler delegate type
    ///
    /// The type of delegate clients must use when registering
    /// handlers
    ///
    using handler_delegate_t = delegate<bool(gsl::not_null<vcpu_t *>)>;

    /// Constructor
    ///
    /// @expects
    /// @ensures
    ///
    /// @param vcpu the vcpu object for this VMX-preemption timer handler
    ///
    preemption_timer_handler(gsl::not_null<vcpu *> vcpu);

    /// Destructor
    ///
    /// @expects
    /// @ensures
    ///
    ~preemption_timer_handler() = default;

public:

    /// Add VMX Preemption Timer Handler
    ///
    /// @expects
    /// @ensures
    ///
    /// @param d the handler to call when an exit occurs
    ///
    void add_handler(const handler_delegate_t &d);

    /// Enable exiting
    ///
    /// Example:
    /// @code
    /// this->enable_exiting();
    /// @endcode
    ///
    /// @expects
    /// @ensures
    ///
    void enable_exiting();

    /// Disable exiting
    ///
    /// Example:
    /// @code
    /// this->disable_exiting();
    /// @endcode
    ///
    /// @expects
    /// @ensures
    ///
    void disable_exiting();

    /// Set timer
    ///
    /// @expects
    /// @ensures
    ///
    /// @param val the value to set the timer too
    ///
    void set_timer(value_t val);

    /// Get timer
    ///
    /// @expects
    /// @ensures
    ///
    /// @return the current value of the preemption timer
    ///
    value_t get_timer() const;

public:

    /// @cond

    bool handle(gsl::not_null<vcpu_t *> vcpu);

    /// @endcond

private:

    vcpu *m_vcpu;
    std::list<handler_delegate_t> m_handlers;

public:

    /// @cond

    preemption_timer_handler(preemption_timer_handler &&) = default;
    preemption_timer_handler &operator=(preemption_timer_handler &&) = default;

    preemption_timer_handler(const preemption_timer_handler &) = delete;
    preemption_timer_handler &operator=(const preemption_timer_handler &) = delete;

    /// @endcond
};

}

#endif
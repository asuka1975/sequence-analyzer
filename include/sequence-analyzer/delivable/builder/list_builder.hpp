#ifndef ASUKA1975_SEQUENCE_ANALYZER_DELIVABLE_BUILDER_LIST_BUILDER_HPP
#define ASUKA1975_SEQUENCE_ANALYZER_DELIVABLE_BUILDER_LIST_BUILDER_HPP

#include "result/result.hpp"

namespace asuka1975 {
    template <class TOutput, class TError>
    struct ListBuilder {
        virtual ~ListBuilder() = default;
        virtual void add(TOutput output) = 0;
        virtual Result<TError, TOutput> builder() = 0;
        virtual bool ready() const noexcept = 0;
        virtual void reset() = 0;
    };
}

#endif
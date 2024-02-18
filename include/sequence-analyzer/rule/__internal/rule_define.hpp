#ifndef ASUKA1975_SEQUENCE_ANALYZER_RULE_RULE_DEFINE_HPP
#define ASUKA1975_SEQUENCE_ANALYZER_RULE_RULE_DEFINE_HPP

#include "../rule_declare.hpp"
#include <cstddef>

namespace asuka1975 {
    template <class TItem, class TOutput, class TError>
    inline ReadStatus Rule<TItem, TOutput, TError>::read(const TItem& item) {
        readingStatus = readInternal(item);
        seekBackCountOnError++;

        if(readingStatus == ReadStatus::Complete) {
            seekBackCountOnError = 0;
        }

        return readingStatus;
    }

    template <class TItem, class TOutput, class TError>
    inline ReadStatus Rule<TItem, TOutput, TError>::readLast(const TItem& item) {
        readingStatus = readLastInternal(item);
        seekBackCountOnError++;

        if(readingStatus == ReadStatus::Complete) {
            seekBackCountOnError = 0;
        }

        return readingStatus;
    }

    template <class TItem, class TOutput, class TError>
    inline std::size_t Rule<TItem, TOutput, TError>::getSeekBackCount() const noexcept {
        if(readingStatus == ReadStatus::Reject) {
            return seekBackCountOnError;
        } else {
            return getSeekBackCountInternal();
        }
    }

    template <class TItem, class TOutput, class TError>
    inline void Rule<TItem, TOutput, TError>::reset() {
        seekBackCountOnError = 0;

        this->resetInternal();
    }

    template <class TItem, class TOutput, class TError>
    inline ReadStatus Rule<TItem, TOutput, TError>::readLastInternal(const TItem& item) {
        return readInternal(item);
    }
}

#endif
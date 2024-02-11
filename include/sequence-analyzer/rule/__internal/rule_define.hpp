#ifndef ASUKA1975_SEQUENCE_ANALYZER_RULE_RULE_DEFINE_HPP
#define ASUKA1975_SEQUENCE_ANALYZER_RULE_RULE_DEFINE_HPP

#include "../rule_declare.hpp"

namespace asuka1975 {
    template <class TItem, class TOutput, class TError>
    inline ReadStatus Rule<TItem, TOutput, TError>::read(const TItem& item) {
        readingStatus = readInternal(item);

        if(readingStatus == ReadStatus::Continue) {
            seekBackCountOnError++;
        } else if(readingStatus == ReadStatus::Complete) {
            seekBackCountOnError = 0;
        }

        return readingStatus;
    }
}

#endif
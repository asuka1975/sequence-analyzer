#ifndef ASUKA1975_SEQUENCE_ANALYZER_RULE_RULE_DECLARE_HPP
#define ASUKA1975_SEQUENCE_ANALYZER_RULE_RULE_DECLARE_HPP

#include <cstddef>
#include <cstdint>
#include <memory>

#include "result/result.hpp"
#include "sequence-analyzer/rule/read_status.hpp"

namespace asuka1975 {
    template <class TItem, class TOutput, class TError>
    class Rule {
    public:
        virtual ~Rule() = default;
        ReadStatus read(const TItem& item);
        ReadStatus readLast(const TItem& item);
        virtual Result<TError, TOutput> create() const = 0;
        virtual TError getError() const noexcept = 0;
        std::size_t getSeekBackCount() const noexcept;
        void reset();
    protected:
        virtual ReadStatus readInternal(const TItem& item) = 0;
        virtual ReadStatus readLastInternal(const TItem& item);
        virtual std::size_t getSeekBackCountInternal() const noexcept = 0;
        virtual void resetInternal() = 0;
    private:
        ReadStatus readingStatus = ReadStatus::Continue;
        std::size_t seekBackCountOnError = 0;
    };
}

#endif
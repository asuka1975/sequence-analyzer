#ifndef ASUKA1975_SEQUENCE_ANALYZER_RULE_RULE_SEQUENCE_DECLARE_HPP
#define ASUKA1975_SEQUENCE_ANALYZER_RULE_RULE_SEQUENCE_DECLARE_HPP

#include <cstddef>
#include <list>
#include <memory>

#include "result/result.hpp"

#include "sequence-analyzer/delivable/builder/sequence_builder.hpp"
#include "sequence-analyzer/rule/read_status.hpp"
#include "sequence-analyzer/rule/rule.hpp"

namespace asuka1975 {
    template <class TItem, class TOutput, class TError>
    class RuleSequence : public virtual Rule<TItem, TOutput, TError> {
    public:
        RuleSequence(std::unique_ptr<Rule<TItem, TOutput, TError>> rule, std::unique_ptr<SequenceBuilder<TOutput, TError>> builder);
        Result<TError, TOutput> create() const override;
        TError getError() const noexcept override;
    protected:
        ReadStatus readInternal(const TItem& item) override;
        ReadStatus readLastInternal(const TItem& item) override;
        std::size_t getSeekBackCountInternal() const noexcept override;
        void resetInternal() override;
    private:
        std::unique_ptr<Rule<TItem, TOutput, TError>> rule;
        std::unique_ptr<SequenceBuilder<TOutput, TError>> builder;
        std::size_t seekBackCount = 0;
        std::size_t seekBackCountOnContinue = 0;
    };
}

#endif
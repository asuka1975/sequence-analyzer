#ifndef ASUKA1975_SEQUENCE_ANALYZER_RULE_RULE_LIST_DECLARE_HPP
#define ASUKA1975_SEQUENCE_ANALYZER_RULE_RULE_LIST_DECLARE_HPP

#include <list>
#include <memory>
#include <ranges>

#include "sequence-analyzer/delivable/builder/list_builder.hpp"
#include "sequence-analyzer/rule/rule.hpp"

namespace asuka1975 {
    template <class TItem, class TOutput, class TError>
    class RuleList : public virtual Rule<TItem, TOutput, TError> {
    public:
        template <std::ranges::input_range TRuleList>
            requires std::same_as<std::ranges::range_value_t<TRuleList>, std::unique_ptr<Rule<TItem, TOutput, TError>>>
        RuleList(TRuleList ruleList, std::unique_ptr<ListBuilder<TOutput, TError>> builder);
        ReadStatus read(const TItem& item) override;
        Result<TError, TOutput> create() const override;
        TError getError() const noexcept override;
        std::size_t getSeekBackCount() const noexcept override;
        void reset() override;
    private:
        std::list<Rule<TItem, TOutput, TError>> ruleList;
        typename std::list<Rule<TItem, TOutput, TError>>::iterator focus;
        std::unique_ptr<ListBuilder<TOutput, TError>> builder;
        std::size_t seekBackCount = 0;
    };
}

#endif
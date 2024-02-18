#ifndef ASUKA1975_SEQUENCE_ANALYZER_RULE_RULE_LIST_DEFINE_HPP
#define ASUKA1975_SEQUENCE_ANALYZER_RULE_RULE_LIST_DEFINE_HPP

#include "../rule_list_declare.hpp"
#include "sequence-analyzer/delivable/builder/list_builder.hpp"
#include "sequence-analyzer/rule/read_status.hpp"
#include <iterator>

namespace asuka1975 {
    template <class TItem, class TOutput, class TError>
    template <std::ranges::input_range TRuleList>
        requires std::same_as<std::ranges::range_value_t<TRuleList>, std::unique_ptr<Rule<TItem, TOutput, TError>>>
    inline RuleList<TItem, TOutput, TError>::RuleList(TRuleList ruleList, std::unique_ptr<ListBuilder<TOutput, TError>> builder) : builder(std::move(builder)) {
        std::ranges::move(ruleList, std::back_inserter(this->ruleList));
        focus = this->ruleList.begin();
    }

    template <class TItem, class TOutput, class TError>
    inline ReadStatus RuleList<TItem, TOutput, TError>::readInternal(const TItem& item) {
        auto status = (*focus)->read(item);
        seekBackCount = (*focus)->getSeekBackCount();
        if(status == ReadStatus::Reject) {
            return status;
        } else if(status == ReadStatus::Complete) {
            auto result = (*focus)->create();
            if(result.hasValue()) {
                builder->add(result.get());
            } else {
                return ReadStatus::Reject;
            }
            std::advance(focus, 1);
            if(focus == ruleList.end()) {
                return ReadStatus::Complete;
            } else {
                return ReadStatus::Continue;
            }
        } else {
            return ReadStatus::Continue;
        }
    }

    template <class TItem, class TOutput, class TError>
    inline ReadStatus RuleList<TItem, TOutput, TError>::readLastInternal(const TItem& item) {
        auto status = (*focus)->readLast(item);
        seekBackCount = (*focus)->getSeekBackCount();
        if(status == ReadStatus::Reject) {
            return status;
        } else if(status == ReadStatus::Complete) {
            auto result = (*focus)->create();
            if(result.hasValue()) {
                builder->add(result.get());
            } else {
                return ReadStatus::Reject;
            }
            std::advance(focus, 1);
            if(focus == ruleList.end()) {
                return ReadStatus::Complete;
            } else {
                return ReadStatus::Continue;
            }
        } else {
            return ReadStatus::Continue;
        }
    }

    template <class TItem, class TOutput, class TError>
    inline Result<TError, TOutput> RuleList<TItem, TOutput, TError>::create() const {
        return builder->create();
    }
    
    template <class TItem, class TOutput, class TError>
    inline TError RuleList<TItem, TOutput, TError>::getError() const noexcept {
        return (*focus)->getError();
    }

    template <class TItem, class TOutput, class TError>
    inline std::size_t RuleList<TItem, TOutput, TError>::getSeekBackCountInternal() const noexcept {
        return seekBackCount;
    }

    template <class TItem, class TOutput, class TError>
    inline void RuleList<TItem, TOutput, TError>::resetInternal() {
        for(auto& rule : ruleList) {
            rule->reset();
        }
        focus = ruleList.begin();
        builder->reset();
        seekBackCount = 0;
    }
}

#endif
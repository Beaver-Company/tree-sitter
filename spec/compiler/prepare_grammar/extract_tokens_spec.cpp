#include "spec_helper.h"
#include "prepare_grammar/extract_tokens.h"

START_TEST

using prepare_grammar::extract_tokens;
using namespace rules;

describe("preparing a grammar", []() {
    it("extracts character-based subtrees into a separate grammar", [&]() {
        pair<Grammar, Grammar> result = extract_tokens(Grammar({
            { "rule1", seq({
                character('a'),
                character('b'),
                seq({
                    sym("rule2"),
                    sym("rule3") }),
                seq({
                    character('a'),
                    character('b') }) }) }
        }));
        
        AssertThat(result.first, Equals(Grammar({
            { "rule1", seq({
                sym("1"),
                seq({
                    sym("rule2"),
                    sym("rule3") }),
                sym("1") }) }
        })));

        AssertThat(result.second, Equals(Grammar("", {
            { "1", rules::seq({
                rules::character('a'),
                rules::character('b') }) },
            { "__END__", character('\0') },
        })));
    });
    
    it("turns entire rules into tokens when they contain no symbols", [&]() {
        auto result = extract_tokens(Grammar({
            { "rule1", sym("rule2") },
            { "rule2", seq({
                character('a'),
                character('b') }) }
        }));
        
        AssertThat(result.first, Equals(Grammar({
            { "rule1", sym("rule2") }
        })));
        
        AssertThat(result.second, Equals(Grammar("", {
            { "rule2", seq({
                character('a'),
                character('b') }) },
            { "__END__", character('\0') },
        })));
    });
});

END_TEST
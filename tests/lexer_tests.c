#include "lexer_tests.h"
#include "minunit.h"
#include <re_lexer.h>
#include <stdio.h>

static int TestSimple()
{
	lexer l;
	char* ss = "a";
	init_lexer(&l,ss);
	token t = read_token(&l);
	mu_assert("should be CHAR token", t.type == CHAR_TOK);
	t = read_token(&l);
	mu_assert("should be END token", t.type == END_TOK);
	return 1;
}
static int TestInvalid()
{
	lexer l;
	char* ss = "\\a";
	init_lexer(&l,ss);
	token t = read_token(&l);
	mu_assert("should be INVALID token", t.type == INVALID_TOK);
	t = read_token(&l);
	mu_assert("should be END token", t.type == END_TOK);
	return 1;
}
static int TestOperators()
{
	lexer l;
	char* ops = "*+|()?";
	init_lexer(&l, ops);
	token t = read_token(&l);
	mu_assert("should be STAR", t.type == STAR_TOK);
	t = read_token(&l);
	mu_assert("should be PLUS", t.type == PLUS_TOK);
	t = read_token(&l);
	mu_assert("should be ALT", t.type == ALT_TOK);
	t = read_token(&l);
	mu_assert("should be LPAREN", t.type == LPAREN_TOK);
	t = read_token(&l);
	mu_assert("should be RPAREN", t.type == RPAREN_TOK);
	t = read_token(&l);
	mu_assert("should be QMARK", t.type == QMARK_TOK);
	return 1;
}
static int TestCharClasses()
{
	lexer l;
	char* ss = ".\\s\\d\\w";
	init_lexer(&l, ss);
	token t = read_token(&l);
	mu_assert("should be WILDCARD", t.type == WILDCARD_TOK);
	t = read_token(&l);
	mu_assert("should be WHITESPACE", t.type == WHITESPACE_TOK);
	t = read_token(&l);
	mu_assert("should be DIGIT", t.type == DIGIT_TOK);
	t = read_token(&l);
	mu_assert("should be ALPHA", t.type == ALPHA_TOK);
	return 1;
}
static int TestEscapeSequences()
{
	lexer l;
	char* ss = "\\\\\\+\\*\\?\\%";
	init_lexer(&l, ss);
	token t = read_token(&l);
	mu_assert("should be char", t.type == CHAR_TOK);
	mu_assert("should be \\ value", t.v.char_value == '\\');
	t = read_token(&l);
	mu_assert("should be char", t.type == CHAR_TOK);
	mu_assert("should be + value", t.v.char_value == '+');
	t = read_token(&l);
	mu_assert("should be char", t.type == CHAR_TOK);
	mu_assert("should be * value", t.v.char_value == '*');
	t = read_token(&l);
	mu_assert("should be char", t.type == CHAR_TOK);
	mu_assert("should be ? value", t.v.char_value == '?');
	t = read_token(&l);
	mu_assert("should be INVALID", t.type == INVALID_TOK);
	t = read_token(&l);
	mu_assert("should be END", t.type == END_TOK);
	return 1;
}

static int TestCountedRepetitions()
{
	lexer l;
	char* ts = "a{   2    }";
	init_lexer(&l, ts);
	token t = read_token(&l);
	mu_assert("should be char", t.type == CHAR_TOK);
	mu_assert("should be a value", t.v.char_value == 'a');
	t = read_token(&l);
	mu_assert("should be LCR_TOK", t.type == LCR_TOK);
	t = read_token(&l);
	mu_assert("should be NUM_TOK", t.type == NUM_TOK);
	mu_assert("value should be 2", t.v.num_value==2);
	t = read_token(&l);
	mu_assert("should be RCR_TOK", t.type == RCR_TOK);

	init_lexer(&l, "a{2  ,3 }.*");
	t = read_token(&l);
	mu_assert("should be char", t.type == CHAR_TOK);
	mu_assert("should be a value", t.v.char_value == 'a');
	t = read_token(&l);
	mu_assert("should be LCR_TOK", t.type == LCR_TOK);
	t = read_token(&l);
	mu_assert("should be NUM_TOK", t.type == NUM_TOK);
	mu_assert("value should be 2", t.v.num_value==2);
	t = read_token(&l);
	mu_assert("should be COMMA_TOK", t.type == COMMA_TOK);
	t = read_token(&l);
	mu_assert("should be NUM_TOK", t.type == NUM_TOK);
	t = read_token(&l);
	mu_assert("should be RCR_TOK", t.type == RCR_TOK);
	t = read_token(&l);
	mu_assert("should be WILDCARD_TOK", t.type == WILDCARD_TOK);
	t = read_token(&l);
	mu_assert("should be STAR_TOK", t.type == STAR_TOK);
	return 1;
}

void test_lexer()
{
	printf("Testing Lexer\n");
	mu_run_test(TestSimple);
	mu_run_test(TestOperators);
	mu_run_test(TestCharClasses);
	mu_run_test(TestEscapeSequences);
	mu_run_test(TestInvalid);
	mu_run_test(TestCountedRepetitions);
}

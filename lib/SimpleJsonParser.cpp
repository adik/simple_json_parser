/*
 * Author: smirnov.arkady@gmail.com
 *
 * Copyright (c) 2012 Arkady Smirnov
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 */
#include <SimpleJsonParser.h>



void json_init(json_parser_t *parser) {
	parser->pdata  = parser->data;
	parser->ptoken = parser->tokens;
}

void json_fill_token(json_token_t *& ptoken, size_t addr) {
	ptoken->right = addr;
	ptoken++;
	ptoken->left  = addr;
}

int json_parse(json_parser_t *p, char chr) {

	// skip until find first {
	if (p->level <= 0) {
		if (chr == '{') {
			// Set first start pointer
			p->ptoken->left  = (size_t) p->pdata;
		}
		else { goto skip; }
	}

	// check if this not
	if (chr == '"') {
		p->type ^= JSON_FIELD_MASK;
	}

	//if field closed
	if ( (p->type & JSON_FIELD_MASK) == 0 ) {

		// calculate level
		switch(chr) {
			case '{':
				// save reference to a start group token
				p->ref[p->level] = (size_t) p->ptoken;
				p->level++;
				break;
			case '}':

				// FIXIT:
				if (p->level >= JSON_MAX_DEPTH)
					*p->pdata++ = chr;;

				p->level--;
				// update start group right reference
				((json_token_t*) p->ref[p->level])->right = (size_t) p->pdata;

				// nothing to parse anymore
				// end function
				if (p->level == 0)
					goto finish;
				else
					goto skip;
		}

		if (p->level >= JSON_MAX_DEPTH)
			goto find;

		// parse primitive
		switch(chr) {
			// change type
			case ':': case ',': case '{':
			case '}':
				p->ptoken->right = (size_t) p->pdata;
				p->ptoken++;
				p->ptoken->left  = (size_t) p->pdata;
				goto skip;
				break;

			// remove spaces
			case ' ': case '\r': case '\n':
				goto skip;
				break;
		}
	}

	if (chr == '"')
		goto skip;

find:
	*p->pdata++ = chr;

// wait next character
skip:
	return 0;

// parse finished
finish:
	p->ptoken->right = (size_t) p->pdata;
	// clear other
	p->ptoken++;
	p->ptoken->left = 0;
	p->ptoken->right = 0;

	return 1;
}


size_t json_token_size(json_parser_t *p, json_token_t *tok) {
	return tok->right - tok->left;
}

void * json_get_token(json_parser_t *p, json_token_t *tok, char *buff, size_t len) {
	memcpy(buff, (char *)(tok->left), len);
	buff[len] = '\0';
}



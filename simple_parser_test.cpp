//============================================================================
// Author      : Smirnov Arkady
//============================================================================

#include <iostream>
#include <iomanip>
#include <cstring>
#include <cstdlib>

#include "SimpleJsonParser.h"

using namespace std;


json_parser_t jparser;


void parse_dump() {

	cout << endl << "TokenData: " << jparser.data << endl << endl;

	for (int i=0; i<JSON_MAX_TOKENS; i++ ) {
		json_token_t *token = &jparser.tokens[i];

		if ( !token->left ) break;

		size_t size = json_token_size(&jparser, &jparser.tokens[i]);
		char  *value = (char*) malloc(size);
		json_get_token(&jparser, &jparser.tokens[i], value, size);

		cout << "left: " 	<< (size_t*) token->left 	<< setw(10);
		cout << "right: " 	<< (size_t*) token->right 	<< setw(10);
		cout << "size: " 	<< size << setw(10);
		cout << "value: " 	<< value;
		cout << endl;

		free (value);
	}
	cout << endl;
}



void var_get( json_parser_t *p, char *str) {

	char *start_addr = strstr (p->data, str);
	char *value;
	int   m;

	// find token
	for (int i=1; i<JSON_MAX_TOKENS; i += 2 ) {
		if ( jparser.tokens[i].left == (size_t)start_addr ) {
			size_t size = json_token_size(&jparser, &jparser.tokens[i+1]);
			value = (char*) malloc(size-1);
			json_get_token(&jparser, &jparser.tokens[i+1], value, size);
			cout << value;
			free (value);
			break;
		}
	}
	cout << endl;
}

void recv_data(char chr) {
	if ( json_parse(&jparser, chr) ) {
		parse_dump();
		var_get(&jparser, "event");
		var_get(&jparser, "data");
	}
}

/***********************************************************/

int main() {
	const char *pt = "Host: Server\r\nType: anything\r\n\r\n "
					 "{"
					  "\"data\": { \"t1\" : \"t2\" }, "
					  "\"event\":\"push:heartbet\", "
					  "\"data\": { \"t3\" : 1123 }, "
					  "\"status\":\"ok\" }";

	json_init(&jparser);

	while (*pt) {
		recv_data(*pt);
		pt++;
	}

	return 0;
}


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



void recv_data(char chr) {

	json_token_t 	*token;
	size_t		 	size;
	char 		 	*value;

	if ( json_parse(&jparser, chr) > 0 ) {
		parse_dump();

		if ( (token = json_find_token(&jparser, "event")) ) {
			size = json_token_size(&jparser, token);
			value = (char*) malloc(size);
			json_get_token(&jparser, token, value, size);

			cout << "get_value:" << value << endl;
			free(value);
		}
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


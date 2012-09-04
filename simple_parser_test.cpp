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

		cout << "Parse OK";
		//
		parse_dump();

		//
		if ( (token = json_find_tag_value_token(&jparser, "event")) ) {
			size = json_token_size(&jparser, token);
			value = (char*) malloc(size);
			json_get_token(&jparser, token, value, size);
			cout << "get_value:" << value << endl;
			free(value);
		}

		value = json_get_tag_value(&jparser, "event");
		cout << value << endl;
		free(value);

		value = json_get_tag_value(&jparser, "data");
		cout << value << endl;
		free(value);

		// clean all
		json_clean_tokens(&jparser);
	}
}

/***********************************************************/

int main() {

	/*
	{"event":"pusher:connection_established","data":"{\"socket_id\":\"11796.609249\"}"}�
	{"event":"pusher:error","data":"{\"code\":null,\"message\":\"Auth info required to subscribe to private-cmd\"}"}�
	*/

	//const char json[] = "{\"event\":\"pusher:error\",\"data\":\"{\"code\":null,\"message\":\"Auth info required to subscribe to private-cmd\"}\"}";
	const char json[] = "{\"event\":\"pusher:error\",\"data\":\"{\\\"code\\\":null,\\\"message\\\":\\\"Auth info required to subscribe to private-cmd\\\"}\"}";
	const char *pt = json;

	json_init(&jparser);

	while (*pt) {
		recv_data(*pt);
		pt++;
	}

	json_clean_tokens(&jparser);
	pt = " {\"event\":\\\"pusher:error\\\",\"data\":\"{\\\"code\\\":null,\\\"message\\\":\\\"Auth info required to subscribe to private-cmd\\\"}\"}";

	while (*pt) {
		recv_data(*pt);
		pt++;
	}

	json_clean_tokens(&jparser);
	pt = " {\"event\":\"pusher:error\"}";

	while (*pt) {
		recv_data(*pt);
		pt++;
	}


	return 0;
}


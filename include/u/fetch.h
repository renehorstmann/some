#ifndef U_FETCH_H
#define U_FETCH_H
#ifdef OPTION_FETCH

//
// GET and POST http requests
// uses libcurl or the emscripten fetch api
//

#include "rhc/types.h"

typedef struct uFetch uFetch;


// does an asynchronous GET call and returns the uFetch class handle
uFetch *u_fetch_new_get(const char *url);

// does an asynchronous POST call and returns the uFetch class handle
// data should be key=value string with an & as seperator
uFetch *u_fetch_new_post(const char *url, Str_s data);

// as always, its safe to pass a killed (*self_ptr==NULL) instance
void u_fetch_kill(uFetch **self_ptr);


// checks for a completed transmission or error
// returns a non invalid String on success, containing the data
// if an error occures, opt_error_code is set to the http error (e. g. 404)
// will call u_fetch_kill, so you dont have to
String u_fetch_check_response(uFetch **self_ptr, int *opt_error_code);


#endif //OPTION_FETCH
#endif //U_FETCH_H

#ifndef U_FETCH_H
#define U_FETCH_H
#ifdef OPTION_FETCH

//
// GET and POST http requests
// uses libcurl or the emscripten fetch api
//

typedef struct uFetch uFetch;

// callback for received data
// if an error occures: data==NULL && size==http_error_code
typedef void (*u_fetch_on_receive_cb)(uFetch *fetch,
        const void *data, size_t size, void *ud);


uFetch *u_fetch_new(u_fetch_on_receive_cb callback, void *user_data);

void u_fetch_kill(uFetch **self_ptr);

void u_fetch_get(uFetch *self, const char *url);

// data should be key=value string with an & as seperator
void u_fetch_post(uFetch *self, const char *url, const void *data, size_t data_size);

// checks for a conpleted transmission or error and calls the receive callback (on this thread)
void u_fetch_check_response(uFetch *self);


#endif //OPTION_FETCH
#endif //U_FETCH_H

#ifdef OPTION_FETCH

#include "rhc/error.h"
#include "rhc/log.h"
#include "rhc/alloc.h"
#include "u/fetch.h"


#ifdef __EMSCRIPTEN__

#include <emscripten/fetch.h>

struct uFetch {
    u_fetch_on_receive_cb callback;
    void *user_data;
    
    emscripten_fetch_attr_t attr;
    emscripten_fetch_t *fetch;
    
    void *data;
    size_t data_size;
    
    bool fetch_completed;
}

static ems_fetch_success(emscripten_fetch_t *fetch) {
    log_trace("u_fetch succeded");
    uFetch *self = fetch->userData;
    assume(self->fetch == fetch, "wtf");
    
    self->data = rhc_realloc(fetch->numBytes);
    memcpy(self->data, fetch->data, fetch->numBytes);
    self->data_size = fetch->numBytes;
    
    emscripten_fetch_close(self->fetch);
    self->fetch = NULL;
    
    if(self->fetch_completed)
        log_warn("u_fetch was completed, but overridden");
    self->fetch_completed = true;
}

static ems_fetch_error(emscripten_fetch_t *fetch) {
    log_warn("u_fetch failed with http code: %i", fetch->status);
    uFetch *self = fetch->userData;
    assume(self->fetch == fetch, "wtf");
    
    rhc_free(self->data);
    self->data = NULL;
    self->data_size = fetch->status;
    
    emscripten_fetch_close(self->fetch);
    self->fetch = NULL;
    
    if(self->fetch_completed)
        log_warn("u_fetch was completed, but overridden");
    self->fetch_completed = true;
}

uFetch *u_fetch_new(u_fetch_on_receive_cb callback, void *user_data) {
    uFetch *self = rhc_calloc(sizeof *self);
    
    self->callback = callback;
    self->user_data = user_data;
     
    emscripten_fetch_attr_init(&swlf->attr); 
    self->userData = self;
    self->attr.attributes = EMSCRIPTEN_FETCH_LOAD_TO_MEMORY; 
    self->attr.onsuccess = ems_fetch_success; 
    self->attr.onerror = ems_fetch_error; 
    
    return self;
}

void u_fetch_kill(uFetch **self_ptr) {
    uFetch *self = *self_ptr;
    if(!self)
        return;
    if(self->fetch) {
        log_warn("u_fetch_kill called before fetch was finished?");
        emscripten_fetch_close(self->fetch);
    }
    rhc_free(self->data);
    rhc_free(self);
    *self_ptr = NULL;
}

void u_fetch_get(uFetch *self, const char *url) {
    log_trace("u_fetch_get: %s", url);
    strcpy(self->attr.requestMethod, "GET"); 
    self->attr.requestData = NULL;
    self->attr.requestDataSize = 0;
    self->fetch = emscripten_fetch(&self->attr, url);
    assume(self->fetch->userData == self, "should include user_data");
}

void u_fetch_post(uFetch *self, const char *url, const void *data, size_t data_size) {
    log_trace("u_fetch_post: %s", url);
    self->data = rhc_realloc(data_size);
    self->data_size = data_size;
    memcpy(self->data, data, data_size;)
    
    strcpy(self->attr.requestMethod, "POST"); 
    self->attr.requestData = self->data;
    self->attr.requestDataSize = self->data_size;
    self->fetch = emscripten_fetch(&self->attr, url);
    assume(self->fetch->userData == self, "should include user_data");
}

void u_fetch_check_response(uFetch *self) {
    if(self->fetch_completed) {
        self->fetch_completed = false;
        self->callback(self, self->data, self->data_size, self->user_data);
    }
}

#else

uFetch *u_fetch_new(u_fetch_on_receive_cb callback, void *user_data);

void u_fetch_kill(uFetch **self_ptr);

void u_fetch_get(uFetch *self, const char *url);

void u_fetch_post(uFetch *self, const char *url, const void *data, size_t data_size);

void u_fetch_check_response(uFetch *self);


#endif
#else //OPTION_FETCH
typedef int avoid_iso_c_empty_translation_unit_warning_;
#endif

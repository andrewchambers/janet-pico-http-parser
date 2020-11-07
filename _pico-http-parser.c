#include <janet.h>
#include <assert.h>
#include "picohttpparser/picohttpparser.h"

typedef struct {
    size_t last_len;
    size_t n_headers;
    struct phr_header headers[];
} HTTPParserState;


static struct JanetAbstractType HTTPParserState_jt = {
    "pico-http-parser.state",
    JANET_ATEND_NAME
};

static Janet cfun_new(int32_t argc, Janet *argv) {
    janet_fixarity(argc, 1);
    
    size_t n_headers = janet_getnumber(argv, 0);
    
    /* Avoid integer overflow in allocation. */
    if (n_headers > 100000) {
        janet_panicf("too many headers");
    }

    HTTPParserState *pst = janet_abstract(&HTTPParserState_jt, sizeof(HTTPParserState) + n_headers*sizeof(struct phr_header));
    pst->last_len = 0;
    pst->n_headers = n_headers;

    return janet_wrap_abstract(pst);
}

static Janet cfun_parse(int32_t argc, Janet *argv) {
    janet_fixarity(argc, 2);
    
    HTTPParserState *pst = janet_getabstract(argv, 0, &HTTPParserState_jt);
    JanetBuffer *buf = janet_getbuffer(argv, 1);

    if ((size_t)buf->count > pst->last_len) {
        pst->last_len = 0;
    }

    char *method, *path;
    size_t method_len, path_len, n_headers;
    int pret, minor_version;


    n_headers = pst->n_headers;
    pret = phr_parse_request((const char *)buf->data, (size_t)buf->count, (const char**)&method, &method_len, (const char**)&path, &path_len,
                             &minor_version, pst->headers, &n_headers, pst->last_len);

    pst->last_len = buf->count;

    if (pret > 0) {
        JanetTable *request = janet_table(3);
        JanetTable *header_tab = janet_table(16);
        janet_table_put(request, janet_ckeywordv("method"), janet_stringv((const uint8_t *)method, method_len));
        janet_table_put(request, janet_ckeywordv("path"), janet_stringv((const uint8_t *)path, path_len));
        janet_table_put(request, janet_ckeywordv("headers"), janet_wrap_table(header_tab));
        for (size_t i = 0; i < n_headers; i++) {
            Janet header = janet_stringv((const uint8_t *)pst->headers[i].name, pst->headers[i].name_len);
            Janet value = janet_stringv((const uint8_t *)pst->headers[i].value, pst->headers[i].value_len);
            janet_table_put(header_tab, header, value);
        }

        size_t overshoot = buf->count - pret;
        memmove(buf->data, buf->data + pret, overshoot);
        janet_buffer_setcount(buf, overshoot);
        /* reset for next use */
        pst->last_len = 0;
        return janet_wrap_table(request);
    } else if (pret == -1) {
        pst->last_len = 0;
        return janet_ckeywordv("parse-error");
    } else if (pret == -2) {
        return janet_ckeywordv("want-more");
    } else {
        janet_panicf("BUG: unhandled http parser error");
    }
}

static Janet cfun_reset(int32_t argc, Janet *argv) {
    janet_fixarity(argc, 1);
    HTTPParserState *pst = janet_getabstract(argv, 0, &HTTPParserState_jt);
    pst->last_len = 0;
    return janet_wrap_nil();
}

static const JanetReg cfuns[] = {
    {"new", cfun_new, NULL},
    {"parse", cfun_parse, NULL},
    {"reset", cfun_reset, NULL},
    {NULL, NULL, NULL}
};

JANET_MODULE_ENTRY(JanetTable *env) {
    janet_cfuns(env, "pico-http-parser", cfuns);
}
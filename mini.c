/*
 * Copyright 2014 Andrew Gregory <andrew.gregory.8@gmail.com>
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 *
 * Project URL: http://github.com/andrewgregory/mINI.c
 */

#ifndef MINI_C
#define MINI_C

#include <string.h>
#include <ctype.h> /* isspace */
#include <stdlib.h>
#include <stdio.h>

#include "mini.h"

mini_t *mini_finit(FILE *stream) {
    mini_t *mini;

    if((mini = calloc(sizeof(mini_t), 1)) == NULL) { return NULL; }

    mini->stream = stream;

    mini->_buf_size = 512;
    mini->_buf = malloc(mini->_buf_size);
    if(!mini->_buf) { mini_free(mini); return NULL; }

    return mini;
}

mini_t *mini_init(const char *path) {
    FILE *stream = fopen(path, "r");
    return stream ? mini_finit(stream) : NULL;
}

void mini_free(mini_t *mini) {
    free(mini->_buf);
    free(mini->section);
    if(mini->stream) { fclose(mini->stream); }
    free(mini);
}

static size_t _mini_strtrim(char *str) {
    char *start = str, *end;

    if(!(str && *str)) {
        return 0;
    }

    for(; *start && isspace((int) *start); start++);
    end = start + strlen(start) - 1;
    for(; end > start && isspace((int) *end); end--);

    *(++end) = '\0';
    memmove(str, start, end - start + 1);

    return end - start;
}

mini_t *mini_next(mini_t *mini) {
    char *c = NULL;
    size_t buflen = 0;

    mini->key = NULL;
    mini->value = NULL;

    if(fgets(mini->_buf, mini->_buf_size, mini->stream) == NULL) {
        mini->eof = feof(mini->stream);
        return NULL;
    }

    mini->lineno++;

    if((c = strchr(mini->_buf, '#'))) { *c = '\0'; }

    buflen = _mini_strtrim(mini->_buf);

    if(buflen == 0) { return mini_next(mini); }

    if(mini->_buf[0] == '[' && mini->_buf[buflen - 1] == ']') {
        free(mini->section);
        if(!(mini->section = strndup(mini->_buf + 1, buflen - 2))) {
            return NULL;
        }
    } else {
        mini->key = mini->_buf;
        if((c = strchr(mini->_buf, '='))) {
            *c = '\0';
            mini->value = c + 1;
            _mini_strtrim(mini->key);
            _mini_strtrim(mini->value);
        }
    }

    return mini;
}

mini_t *mini_lookup_key(mini_t *mini, const char *section, const char *key) {
    int in_section = (section == NULL);

    rewind(mini->stream);
    mini->lineno = 0;
    mini->eof = 0;

    if(!key) {
        return NULL;
    }

    while(mini_next(mini)) {
        if(mini->lineno == 0) {
            /* error occurred */
            return NULL;
        } else if(!mini->key) {
            /* starting a new section */
            in_section = (strcmp(mini->section, section) == 0);
        } else if(in_section && strcmp(mini->key, key) == 0) {
            return mini;
        }
    }

    return NULL;
}

int mini_fparse_cb(FILE *stream, mini_cb_t cb, void *data) {
    mini_t *mini = mini_finit(stream);
    int ret = 0;

    if(!mini) {
        return cb(0, NULL, NULL, NULL, data);
    }

    while(ret == 0 && mini_next(mini)) {
        ret = cb(mini->lineno, mini->section, mini->key, mini->value, data);
    }
    if(ret == 0 && !mini->eof) {
        ret = cb(0, NULL, NULL, NULL, data);
    }
    
    mini_free(mini);
    return ret;
}

int mini_parse_cb(const char *path, mini_cb_t cb, void *data) {
    FILE *stream = fopen(path, "r");
    return stream
        ? mini_fparse_cb(stream, cb, data)
        : cb(0, NULL, NULL, NULL, data);
}

#endif /* MINI_C */

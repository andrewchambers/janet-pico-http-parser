# janet-pico-http-parser

Janet bindings to pico-http-parser.

## Example usage

```
(import pico-http-parser)

> (def st (pico-http-parser/new))
> (def buf @"")
> (pico-http-parser/parse st buf)
:want-more
> (buffer/push-string buf "GET /pub/WWW/TheProject.html HTTP/1.1\r\n")
> (pico-http-parser/parse st buf)
:want-more
> (buffer/push-string buf "\r\nabc")
(pico-http-parser/parse st buf)
@{:headers @{} :path "/pub/WWW/TheProject.html" :method "GET"}
> buf
@"abc"
```
(import pico-http-parser :as p)

(def simple-parse-tests
  [[@"GET /pub/WWW/TheProject.html HTTP/1.1\r\nHost: www.w3.org\r\n\r\nxyz"
    @{:method "GET"
      :path "/pub/WWW/TheProject.html"
      :headers @{"Host" "www.w3.org"}}
    @"xyz"]])

(each [raw-req expected-req expected-remainder] simple-parse-tests
  (def st (p/new))
  (def buf (buffer/slice raw-req))
  (def parsed-req (p/parse st buf))
  (unless (deep= parsed-req expected-req)
    (errorf "%j != %j" parsed-req expected-req))
  (unless (deep= buf expected-remainder)
    (errorf "%j != %j" buf expected-remainder)))

# Test partial parse
(do
  (def st (p/new))
  (def buf @"")
  (assert (= :want-more (p/parse st buf)))
  (buffer/push-string buf "GET /pub/WWW/TheProject.html HTTP/1.1\r\n")
  (assert (= :want-more (p/parse st buf)))
  (buffer/push-string buf "\r\nabc")
  (assert (table? (p/parse st buf)))
  (assert (deep= buf @"abc")))

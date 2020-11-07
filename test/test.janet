(import pico-http-parser :as p)

(def tests
  [[@"GET /pub/WWW/TheProject.html HTTP/1.1\r\nHost: www.w3.org\r\n\r\nxyz"
    @{}
    @"xyz"]])

(each [raw-request expected-request expected-remainder] tests
  (def st (p/new))
  (def buf (buffer/slice raw-request))
  (def parsed-req (p/parse st buf))
  (unless (deep= parsed expected-req)
    (errorf "%j != %j" parsed-req expected-req))
  (unless (deep= buf expected-remainder)
    (errorf "%j != %j" buf expected-remainder)))
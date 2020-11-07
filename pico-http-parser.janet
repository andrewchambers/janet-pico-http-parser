(import _pico-http-parser)

(defn new
  [&keys {:max-headers max-headers}]
  (default max-headers 128)
  (_pico-http-parser/new max-headers))

(def parse
  ""
  _pico-http-parser/parse)

(def reset
  ""
  _pico-http-parser/reset)
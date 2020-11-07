(import _pico-http-parser)

(defn new
  "Create a new parser state."
  [&keys {:max-headers max-headers}]
  (default max-headers 128)
  (_pico-http-parser/new max-headers))

(def parse
  "(parse state buf)\n\n

  Parse a buffer into a request map.\n\n

  Returns a table on success or one of :want-mode, :parse-error
  if more data is wanted, or a parse error occurs respectively.\n\n

  On success buf will be truncated and contain any request body 
  bytes that were not part of the header.\n\n

  Because the parser tracks an offset in a given buffer to prevent slow-loris attacks, 
  you must call parser/reset if data in the buffer has changed before a successful parse completes."
  _pico-http-parser/parse)

(def reset
  "Reset a parser state."
  _pico-http-parser/reset)

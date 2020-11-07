(declare-project
  :name "pico-http-parser"
  :license "MIT"
  :url "https://github.com/andrewchambers/janet-pico-http-parser"
  :repo "git+https://github.com/andrewchambers/janet-pico-http-parser.git")

(declare-native
  :name "_pico-http-parser"
  :source ["_pico-http-parser.c" "picohttpparser/picohttpparser.c"])

(declare-source
  :source ["pico-http-parser.janet"])
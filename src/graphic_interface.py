from ctypes import *

cdll.LoadLibrary("/Users/maxfieldherman/Desktop/auto-complete/src/testlib.dylib")
test = CDLL("/Users/maxfieldherman/Desktop/auto-complete/src/testlib.dylib")
test.parse_word("abcde")
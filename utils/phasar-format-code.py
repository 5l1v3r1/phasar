#!/usr/bin/env python3

# author: Philipp D. Schubert
#
# This little script reads all C++ related source files from the project
# and formats them using the clang-format tool according to the default 
# 'google' C++ style scheme.

import os

SRC_DIRS = ("include/phasar/Config",
						"include/phasar/Controller",
						"include/phasar/DB",
						"include/phasar/Experimental",
						"include/phasar/Flex",
						"include/phasar/PhasarClang",
						"include/phasar/PhasarLLVM",
						"include/phasar/Utils",
						"lib/Config",
						"lib/Controller",
						"lib/DB",
						"lib/Experimental",
						"lib/Flex",
						"lib/PhasarClang",
						"lib/PhasarLLVM",
						"lib/Utils",
						"include/phasar")

cpp_extensions = (".cpp",
									".cxx",
									".c++",
									".cc",
									".cp",
									".c",
									".i",
									".ii",
									".h",
									".h++",
									".hpp",
									".hxx",
									".hh",
									".inl",
									".inc",
									".ipp",
									".ixx",
									".txx",
									".tpp",
									".tcc",
									".tpl")

for SRC_DIR in SRC_DIRS:
	for root, dir, files in os.walk(SRC_DIR):
		for file in files:
			if file.endswith(cpp_extensions):
				os.system("clang-format -i " + root + "/" + file)

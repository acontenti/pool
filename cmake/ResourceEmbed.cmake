# Creates C++ resources file from files in given directory
set(header std.hpp)
set(source "${output_dir}/std.cpp")
# Create empty source file
file(WRITE ${source} "")
# Collect input files
file(GLOB bins ${source_dir}/*)
# Iterate through input files
file(APPEND ${source} "#include \"${header}\"\n\n")
foreach (bin ${bins})
	# Get short filename
	get_filename_component(filename ${bin} NAME_WE)
	string(REGEX MATCH "([^/]+)$" filename ${filename})
	# Replace filename spaces & extension separator for C++ compatibility
	string(REGEX REPLACE "\\.| |-" "_" filename ${filename})
	# Read hex data from file
	file(READ ${bin} filedata)
	# Append data to source file
	file(APPEND ${source} "const std::string ${filename}_module = R\"::::(\n${filedata}\n)::::\";\n\n")
endforeach ()
file(APPEND ${source} "const std::unordered_map<std::string, std::string> STD_MODULES = {\n")
foreach (bin ${bins})
	# Get short filename
	get_filename_component(filename ${bin} NAME_WE)
	string(REGEX MATCH "([^/]+)$" filename ${filename})
	# Replace filename spaces & extension separator for C++ compatibility
	string(REGEX REPLACE "\\.| |-" "_" filename ${filename})
	file(APPEND ${source} "\t{\":${filename}\", ${filename}_module}\n")
endforeach ()
file(APPEND ${source} "};\n\n")
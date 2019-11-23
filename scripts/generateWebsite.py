#!/usr/bin/python3

import os
import sys
import fileinput
import re

print("Generating include file for website...")

inputFiles = {
  "index_html": "website/index.html",
  "update_result_html": "website/update_result.html",
  "style_css": "website/style.css",
  "script_js": "website/script.js"
}
outputFile = "src/website.h"

outputFileHandle = open( outputFile, 'w' )

outputFileHandle.write("#pragma once\n")
for inputFile in inputFiles:
  outputFileHandle.write("static const String " + inputFile + " = \"")
  for line in fileinput.input( inputFiles.get(inputFile) ):
    # index.html specific
      line = line.replace('<link rel="stylesheet" type="text/css" href="style.css"/>', '<style type=\'text/css\'>%STYLE%</style>')
      line = line.replace('<script src="script.js"></script>', '<script>%SCRIPT%</script>');
    # generic
      line = line.strip() # Remove leading and trailing spaces
      line = line.split('//', 1)[0] # Remove comments starting with //
      line = line.replace( '\n', '' ) # Remove newlines
      line = line.replace( '"', '\\"') # Escape " characters
      line = re.sub(r"\/\*.*\*\/", "", line) # Remove comments within /* */
      line = re.sub(r"\/\*.*\*\/", "", line) # Remove comments within /* */
      
      outputFileHandle.write( line )
  outputFileHandle.write("\";\n")
outputFileHandle.close()

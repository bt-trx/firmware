#!/usr/bin/python3

import os
import sys
import fileinput

print("Generating include file for website...")

inputFiles = {
  "index_html": "website/index.html",
  "style_css": "website/style.css"
}
outputFile = "src/website.h"

outputFileHandle = open( outputFile, 'w' )

outputFileHandle.write("#pragma once\n")
for inputFile in inputFiles:
  outputFileHandle.write("static const String " + inputFile + " = \"")
  for line in fileinput.input( inputFiles.get(inputFile) ):
      line = line.strip()
      line = line.replace( '\n', '' )
      line = line.replace( '"', '\'')
      outputFileHandle.write( line )
  outputFileHandle.write("\";\n")
outputFileHandle.close()

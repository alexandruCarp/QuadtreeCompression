## QuadTree
A CLI tool for compressing images in PPM format, using quadtrees. The decompressed image
will preserve the original image quality up to a threshold defined by the user (if threshold is 0 then there is no quality loss).
 

## Build & Run
Use `make` to build the *quadtree* binary.
Run the binary in the following form:

`./quadtree [-c factor | -d | -m type] [input_file] [output_file]`

* -c compress image using *factor* as threshold
* -d decompress file
* -m mirror image horizontally (*type* = h) or vertically (*type* = v)

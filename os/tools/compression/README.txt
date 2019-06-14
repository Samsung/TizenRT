Compression Tool
================

	This file is to help understand geneation and usage of this tool. It also explains structure of compressed binary generated using this tool.

Compressed Binary Structure
===========================

Compressed binary structure

   ######################
   #                    #
   #     Size_Header    #
   #     Cmpr. Type     #
   #     Block_size     #   <--- Compression Header
   #     No. blocks     #
   # Uncmpr .Bin. Size  #
   #   Block Offset []  #
   #                    #
   ######################
   #                    #
   #    Cmpr. Block 0   #
   #    Cmpr. Block 1   #
   #    Cmpr. Block 2   #   <--- Compressed Blocks
   #....................#
   #    Cmpr. Block n   #
   #                    #
   ######################

where,

Size_Header = Size of Compression Header
Cmpr. Type = Compression Type (Look at CONFIG_COMPRESSION_TYPE description)
Block_size = Size of blocks compressed separately (Default = 2048)
No. Blocks = Number of blocks compressed separately (based on uncompressed binary size and Block_size)
Uncmpr. Bin. Size = Size of uncompressed binary
Block Offset [] = Contains offset of 'No. Blocks' blocks from start of  Compressed Blocks section. Block Offset[Cmpr. Block 0] = 0.

Generate Compression tool
=========================

To generate compression tool (mkcompressimg):

	./mkcompresstool.sh

To clean generated tool/object files :

	make clean

Usage
=====

./mkcompressimg  block_size  compression_type  input_uncompressed_binary  output_compressed_binary

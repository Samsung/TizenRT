
FILE : README.txt

INTRODUCTION
-------------------------------------------------------------------------
You can link board-specific folder on this folder
For example, Wi-Fi firmware and so on
It can used to mount the folder on filesystem

HOW TO USE
--------------------------------------------------------------------------
Link board-specific folder saving files or folders on this directory
ln -sf <original folder> external/contents/
- original folder : /bcm/external/contents/artik050
- linked results : external/contents/artik050 -> ../../bcm/external/contents/artik050

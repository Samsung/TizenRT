
# Patches for external/* submodules

## Folder structure

	TizenRT
	|
	+--- external
		|
		+--- <submodule1>
		+--- <submodule2>
		+--- ...
		|
		+------- patches
				|
				+--- <submodule1> (consists of patches for related submodule1)
				+--- <submodule2> (consists of patches for related submodule2)
				+--- ...
				
We add submodules from other repositories.
But to make them work well on TizenRT, it's necessary to apply patches of the corresponding submodule.
Refer external/patches/<submodule>/Readme.md for more details.

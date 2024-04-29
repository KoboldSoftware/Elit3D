# Changelog

## [0.6.4] - 2024-04-29 - KoboldSoftware
### Updated
* zlib 1.3.1
* Assimp 5.3
* imgui_markdown
### Fixed
* iTypeVar stores intptr_t instead of int
* class Object destructor made public
* ImGui uses invalid enum GL_CLIP_ORIGIN on OpenGL <= 3.3

## [0.6.3] - 2021-06-21
### Fixed
* Crash on open application

## [0.6.2] - 2021-06-21
### Added
* Export in wavefront obj format

### Fixed
* Name not changed on selecting a layer on Panel Layers

## [0.6.1] - 2021-06-07
### Fixed
* Fix black panel on AMD and Intel GPU

## [0.6.0] - 2021-05-20
### Added
* Export
	* Export all tiles of layer with CSV, Base64 or Base64 with zlib compression format
	* Export tileset info
	* Export tileset3D info
	* Choose to export the map on a JSON or a XML file
	* Begin of export in .obj format (WIP)

## [0.5.0] - 2021-04-24
### Added
* Camera
	* Orthographic perspective
	* Buttons to set top and front view
	* Fit on map
	* Panel configuration

### Changed
* Name from MapTileEditor3D to Elit3D
* Merge Terrain and Object layers

## [0.4.1] - 2021-04-14
### Added
* Object Editor
	* Save and Load
	* Edit Objects
	* Place Object on map
* 3D Terrains
	* Tilemap 3D Terrains editor
	* Place Terrains on Map in Terrain Layers

### Changed
* Update SDL to 2.0.14

### Fixed
* Draw Map
* Selected tile on draw map

## [0.4.0] - 2021-02-22
### Added
* premake5 project generator
* Layer objects type
* 3D objects system
* Load .fbx from Assimp
* Set 3D models in scene
* Transform component with position, rotation and scale for every object
* Textures in 3D models
* 3D models normals
* Save and Load Objects in map file
* Allow multiple viewports
* New camera system to handle multiple viewports
* Object Editor (WIP)
	* Panel to create 3D objects with 2D sprites
	* Save objects in Json

### Changed
* New UI style (WIP)
* Improved Panel Console
	* Better performance for big amount of logs
	* Log type buttons
* Save App version in map file to handle future complications
* Agroup files in its folders
* Split installation folder and project folder

### Fixed
* Tileset scale with panel width on Panel Tileset
* Paint not working after 3D Models version
* Painting in a Object layer crash

## [0.3.0] - 2021-01-04
### Added
* Tools to modify the map
	* Brush
	* Eraser
	* Bucket
	* Eye dropper
* Panel tools
* Brush size
* Brush shape (Rectangle and Circle)
* Shader to know where the brush will paint
* Debug mode to paint layer texture without shader
* Panel about ImGui Markdown

### Changed
* JSON map export csv format, now export an array of id
* Assimp x86 compiled in release

### Fixed
* When select a tileset with height different to 32px it always load 32px
* Create and select a tilesetwithour filewatcher

## [0.2.1] - 2020-12-18
### Added
 * base64 coder and decoder library
 * Parse map in csv, base64 and base64-zlib compressed
 * Added properties on map
 * Edit Name and Type of layer property
 * Option to export on XML and JSON, with csv, base64 and base64-zlib
 * x64 support
 * Load map by clicking it
 * Load last map used on start
 * Program Icon
 * OpenGL class helper
 * Inspector display some info about files
 * Pugixml (XML parser)

### Changed
 * Map paint system
	 * Now I save the id of the tile and not the position on the tileset
 * Improved Panel Resources
	* Icons
	* Zoom
	* Navigate through folders
	* Move files in folders
	* Open on explorer
 * CC-BY License to BSD-2 Clause
 * Filewatcher temporarily disabled due to complexity and bugs
 * Updated MathGeoLib

### Fixed
* mmgr memory leak detector thread safe

### Removed
* Library dependency

## [0.2.0] - 2020-09-25
### Added
* Panel Layers
	* Create Layers
	* Delete Layers
	* Reorder layers
	* Select Layers
	* Hide and Lock Layers
* Layer utilities
	* Layer height edit
	* Layer opacity
	* Layer displacement
* Layer properties
	* Create Layer property as Int, Bool, Float or String
	* Each property has name, type and value
	* Export with map
	* Button to delete property
* Paint on selected layer
* Resize Map support more layers

### Fixed
* Random Number Generator uniform interger distribution

## [0.1.0] - 2020-09-22
### Added
* SDL and OpenGL integration, main structure with modules
* Input handler
* ImGui
* Panels structure
* Viewports with multisampling
* Camera controls
* Logger
* Profiler
* File System
* JSON parser
* Random number generator (PCG)
* Memory Leak detector
* Shaders system
* MathGeoLib
* 3D models importer (not usable in release)
* Resource system
* Basic File Watcher structure (not finished)
* Event System
* System information logger (infoware)
* Paint, save and load a map with only one layer
* Create and resize map
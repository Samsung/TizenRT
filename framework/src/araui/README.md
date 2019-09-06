# AraUI: Light-weight UI Framework for TizenRT

AraUI is a light weight UI framework for TizenRT.
It focuses on the controlling display and user input on the MCU devices.

# AraUI's concept
  - Easy to use and well documented
  -- All AraUI header files have doxygen comment. Developer can refer this.
  - Pretty good performance
  -- Supports DMA2D hardware acceleration on STM devices
  - Easy to implement GUI animations and effects.
  -- All widgets of the AraUI supports tweening animation(Move/Rotate/Scale)
  -- All tweening animation supports various interpolation methods(Linear/EaseIn/Out/InOut)

# AraUI's role
  - Management all user interaction between Touchscreen
  -- Show GUI / Get input from user

# AraUI's Tutorial and Examples
  - Will be added soon

# Linux/Mac Simulator
  - AraUI have Desktop(Linux/Mac) DAL(Driver Abstract Layer)
  -- It uses libSDL2
  -- Refer [README.md](https://github.com/Samsung/TizenRT/blob/master/tools/araui/sim/README.md) in the tools directory.

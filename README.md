# Modular System for CMangos cores
This system allows an easy creation of custom code for cmangos cores without the need of touching the core itself. This is done via hooks strategycally placed all across the core which any module can attach to and listen to particular events happening. You can find the list of all available hooks [here](https://github.com/davidonete/cmangos-modules/blob/main/src/Module.h#60)

# Modules available
Here is a list of compatible modules that you can install with this system:
- [Immersive](https://github.com/davidonete/cmangos-immersive) (All cores): A module to make the game feel more immersive by adding multiple features.
- [Achievements](https://github.com/davidonete/cmangos-achievements) (Classic and TBC): A module to bring the achievement system from WoTLK to earlier versions of the game
- [Hardcore](https://github.com/davidonete/cmangos-hardcore) (All cores): A module to make the game more challenging
- [Transmog](https://github.com/davidonete/cmangos-transmog) (All cores): A module to bring the transmog system from Cataclysm to earlier versions of the game
- [Dual Spec](https://github.com/davidonete/cmangos-dualspec) (Classic and TBC): A module to bring the dual specialization system from WoTLK to earlier versions of the game
- [Boost](https://github.com/celguar/cmangos-boost.git) (TBC): A module to boost a character to be ready to the next expansion
- [Barber](https://github.com/celguar/cmangos-barber.git) (Classic and TBC): A module to add a barber shop in the capital cities
- [Training Dummies](https://github.com/davidonete/cmangos-trainingdummies.git) (Classic and TBC): A module to add training dummies in the capital cities
- [Paladin power](https://github.com/Redbu11dev/cmangos-paladinpower) (Classic only): A module to give paladins their cut spells back

# How to install
NOTE: At the moment there is no officially supported way for installing this system using the stock cmangos source code, however we have prepared two options depending on your skills. This guide assumes that you have knowledge on using git repositories and know how to compile using cmake, if not please refer to the [Beginners Guide](https://github.com/cmangos/issues/wiki/Beginners-Guide-Introduction) for more info.

## Use already merged code
This guide will use the source code where all the module code is already in place and ready to be used, however this source code is not the latest CMangos version and gets synced with the CMangos source code every couple of months.
1. Follow the guide in the [Installation Instructions](https://github.com/cmangos/issues/wiki/Installation-Instructions) until you reach the [Clone CMangos section](https://github.com/cmangos/issues/wiki/Installation-Instructions#clone-cmangos). Instead of using the stock cmangos repository, use one of the following depending on the core version you want:
  - Classic: ```git clone https://github.com/davidonete/mangos-classic.git -b modules```
  - TBC: ```git clone https://github.com/davidonete/mangos-tbc.git -b modules```
  - WoTLK: ```git clone https://github.com/davidonete/mangos-wotlk.git -b modules```
2.  Once you have it cloned continue following the guide until you reach the [Compiling CMangos section](https://github.com/cmangos/issues/wiki/Installation-Instructions#compiling-cmangos-windows). In this section you will have to define the flags for enabling the module system as well as the individual modules that you want to compile. The flag required for each module can be found on the installation instructions for each module. Example:

```
cmake .. -DCMAKE_BUILD_TYPE=RelWithDebInfo -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++ -DBUILD_MODULES=ON -DBUILD_MODULE_ACHIEVEMENTS=ON -DBUILD_MODULE_IMMERSIVE=ON
```

If you want to generate the visual studio solution instead you can do it with either the Cmake UI:
- Select the project folder, click configure and wait for it to load.
- Click on BUILD_MODULES and click configure again and wait for it to load.
- Find the modules you would want to install (e.g. BUILD_MODULE_TRANSMOG) enable them and click configure again.
- After everything is loaded click generate and open the solution and build it from visual studio

You can also generate the solution via command line using the following command:
```
cmake . -G "Visual Studio 16 2019" -A x64 -B bin/BuildDir -DBUILD_MODULES=ON -DBUILD_MODULE_TRANSMOG=ON
```
Where `-G "Visual Studio 16 2019"` is the visual studio version you have, `-B bin/BuildDir` is where the solution will get generated and `-DBUILD_MODULES=ON -DBUILD_MODULE_TRANSMOG=ON` are the modules you want to enable.

3.  Continue with the installation guide up to the [Install Database section](https://github.com/cmangos/issues/wiki/Installation-Instructions#install-databases). After you finish that step, don't run the game yet! You will need to install the required database changes for each module you enabled. Follow the install instructions of each module for more information.
4.  Lastly before finishing the installation guide remember to copy the configuration files of each module (located in `src/modules/<module>/src/<module>.conf.dist.in`) into the binary folder of the cmangos executables (where `mangosd.conf`is) and edit it to enable it and modify the configuration with your needs. After that finish the installation guide and you should be ready to go!

## Use a patch
You can patch the latest cmangos core to have the required modules code added on top of it. Be aware that this patches can fail if cmangos changes parts of code that the module system depends on. 
To apply a patch download one of the following patch files depending on your core version:
  - Classic: https://github.com/davidonete/cmangos-modules/blob/main/patches/classic.patch
  - TBC: https://github.com/davidonete/cmangos-modules/blob/main/patches/tbc.patch
  - WotLK: TBD
    
Open the command line and navigate to the core repository and apply the patch with this command
```
git apply <patch file>
```

# How to write your own module
Creating your own module requires you to have some knowledge of programming, databases and cmake, but don't get discouraged from this as it is not as hard as it seems, once you get used to it becomes way easier. You should take one of the already made modules as a template to follow as it will help you understand it better. Here are the steps you have to do:

1. Create a repository that will hold the required files for the module. The file structure should be like the following:
  - Root Folder

    - CMakeLists.txt
    - README.md
    - sql
      - install
        - world
        - characters
      - uninstall
        - world
        - characters
    - src
      - MyNewModule.cpp
      - MyNewModule.h
      - MyNewModuleConfig.cpp
      - MyNewModuleConfig.h
      - mynewmodule.conf.dist.in

- Root folder: Is the repository folder of your module
- CMakeLists.txt: This will hold the information of your code for cmake to figure out. If you don't know about cmake just copy one of the already made modules and change the places where the name of the module is.
- README.md: This is where you should explain what your module does, for what cores it is, how to install and how to uninstall. You can follow one of the made modules for the formatting.
- sql: This folder will hold all the sql files for installing and uninstalling the module database information, it should be structured by the database it should be installed (e.g. world folder holds world database sql)
- src: Here you will have the source code and configuration files of your module, it is mandatory to follow the naming convention of the files for it to work, changing `MyNew` with the name you want to put for your module.

2. Make a fork of this repository and change the `modules.conf` file and add a new line with the name of your module (it has to match the name you used in `MyNew`) and the repository url where it is located.
3. Modify locally the `CMakeLists.txt` file located in the root folder of your CMangos core source code and look for the following lines:
```
# Modules setup
if(BUILD_MODULES)
  # Download module config file
  include(FetchContent)

  FetchContent_Declare(
    modules
    GIT_REPOSITORY "https://github.com/davidonete/cmangos-modules.git"
    GIT_TAG "main"
  )
```
4. You will have to change the `GIT_REPOSITORY` value to your previously forked version
5. Run cmake and add the `BUILD_MODULES` flag as well as the new flag for your module `BUILD_MODULE_MYNEWMODULE`
6. If all went well your new module should be located in `src/modules/mynewmodule`
7. Open the solution with Visual Studio (or your prefered IDE)
8. Your new module and config classes must inherit from the `Module` and `ModuleConfig` classes 
9. You will need to set up the name of the module in the module constructor and the configuration class that it will use
10. Remember to place a `static MyNewModule myNewModule;` at the very end of the `MyNewModule.h` file, else your module won't work.
11. Add new variables in the config class and in the `mynewmodule.conf.dist.in` file with comments and default values.
12. Once your module is finished and tested, send a pull request using this repository forked version (made on step 2)

# How to add new hooks
TBD

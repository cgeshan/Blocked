Our application uses C++ 17 standard specific features and behaivor, CMake v3.8 minimim, and the “wxWidgets” library, which has dependencies that need to be installed. 

Please see the below installation instructions for wxWidgets dependencies needed **based on your OS**

# **Dependencies**                        

## Linux
-------------------
Install meson and ninja by running the following commands

```
	sudo apt-get install meson ninja-build
```

Install GTK3 (GTK 3 is an old release, unknown if wxWidgets is compatible with GTK4)

```
	sudo apt-get install libgtk-3-dev
```
## MAC OS X
-------------------

No additional wxWidgets dependencies required

## Windows
-------------------

No additional wxWidgets dependencies required

# **Cloning the repository**
Below are common instructions for cloning our repository

Clone the Unoptimized repository

```console
	  $ git clone https://ramennoodle.me.cmu.edu/Bonobo.Git.Server/Unoptimized.git
```
Navigate into Unpotimized subdirectory

```console
	$ cd Unoptimized
```

Run the following command (this will take some time)

```
	git submodule update -–init
```

Navigate into wxWidgets subdirectory

```
	cd wxWidgets
```

Run the update command again to update the submodules

```
	git submodule update –-init
```

**Building the application**               
============
Below are instructions for building our application

Create build directory

```
	mkdir ../../build
```

Navigate to build folder

```
	cd ../../build
```

Generate project make files 

```
	cmake ../Unoptimized
```
Build application (this will take some time)
```
	cmake --build . --config Release --target App
```
Start the application 
#### Linux
-------------------
```
	./src/App
```
#### MAC OS X
-------------------

```
	./src/App.app/Contents/MacOS/App
```

#### Windows
-------------------
```
	src\Release\App.exe
```

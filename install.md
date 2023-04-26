Our application uses C++ 17 standard specific features and behaivor, CMake v3.8 minimim, and the “wxWidgets” library, which has dependencies that need to be installed. 

Please see the below installation instructions for wxWidgets dependencies needed **based on your OS**

# **Dependencies**                        

## Linux
-------------------
Install meson and ninja by running the following commands

```console
    $ sudo apt-get install meson ninja-build
```

Install GTK3 (GTK 3 is an old release, unknown if wxWidgets is compatible with GTK4)

```console
    $ sudo apt-get install libgtk-3-dev
```
## MAC OS X
-------------------

Smile, you don’t have anything to do. Way to go Steve Jobs	

## Windows
-------------------

Smile, you don’t have anything to do. Way to go Bill Gates

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

```console
	  $ git submodule update -–init
```

Navigate into wxWidgets subdirectory

```console
	$ cd wxWidgets
```

Run the update command again to update the submodules

```console
	  $ git submodule update –-init
```

**Building the application**               
============
Below are instructions for building our application

Create build directory

```console
	  $ mkdir ../../build
```

Navigate to build folder

```console
	  $ cd ../../build
```

Generate project make files 

```console
	  $ cmake ../Unoptimized
```
Build application (this will take some time)
```console
	  $ cmake --build . --config Release --target App
```
Start the application 
#### Linux
-------------------
```console
	  $ ./src/App
```
#### MAC OS X
-------------------

```console
	  $ ./src/App.app/Contents/MacOS/App
```

#### Windows
-------------------
```console
	  $ src\Release\App.exe
```

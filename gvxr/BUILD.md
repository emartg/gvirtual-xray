gVirtualXRay build documentation
=

### Revision ###

* 1.0

### Author ###

Dr Franck Vidal  
Senior Lecturer in Visualisation  
Computer Sci & Electronic Eng  
Bangor University, Bangor, Gwynedd, LL57 2DG  
Email: f DOT vidal AT bangor DOT ac DOT uk


## Building gVirtualXRay for Linux and macOS ###

Get a copy of the **gVirtualXRay** repository either from SVN or [gVirtualXRay software resources]

### Trunk from SVN
Check out the working copy from the trunk of the repository:  
```
$ svn checkout https://svn.code.sf.net/p/gvirtualxray/code/trunk gvirtualxray-code
```

Go into the repository directory:  
```
$ cd gvirtualxray-code
```

### Release

Download the latest release from [gVirtualXRay software resources] and extract the archive using:  
```
$ tar zxvfp gVirtualXRay-X.X.X.tar.gz
```
(replace X.X.X with the version corresponding to your download).


Go into the repository directory:  
```
$ cd gvirtualxray-X.X.X
```


### Configure and build the project

Run "build_gVirtualXRay.sh" and follow the instructions:  
```
$ ./build_gVirtualXRay.sh
```




[gVirtualXRay software resources]: http://gvirtualxray.sourceforge.net/downloads.php

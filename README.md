# CEB Press - Arduino Software

Arduino software to power an open source, high-performance Compresssed Earth Block press.

Compresssed Earth Block building is the highest quality natural building method.
See the [CEB Intro Page](http://opensourceecology.org/wiki/CEB_Press) for more introductory
information about compressed earth blocks.

## How to compile (on Ubuntu Linux)

### Install arduino and scons

    sudo apt-get install -y arduino scons

### Get the code

If you're using a release, download it from https://github.com/OSE/ceb-controller/releases and extract it.

If you want to work with the latest development code, check out the code:
    git clone https://github.com/OSE/ceb-controller.git

### Compile with scons
    cd ceb-controller/cebController
    scons

### Upload to attached Arduino

    scons upload

--------------------------------------------------------------------------------------

### Part of the [Global Village Construction Set](http://opensourceecology.org/gvcs.php)

The Global Village Construction Set (GVCS) is an open technological platform that allows
for the easy fabrication of the 50 different Industrial Machines that it takes to build a
small civilization with modern comforts.
